#include "solver.h"
#include "utils.h"

#include <stdbool.h>

solver_t* solver_init(size_t const nb_nodes)
{
    solver_t* solver = malloc(sizeof(solver_t));
    if (!solver) {
        fprintf(stderr,
                "\033[1;31merror:\033[0m failed to allocate `solver`\n");
        exit(EXIT_FAILURE);
    }

    // Allocate vector of visited nodes, all set to false by default
    bool initial_visited_value = false;
    solver->visited_nodes =
        vec_with_value(&initial_visited_value, nb_nodes, sizeof(bool));
    if (!solver->visited_nodes) {
        fprintf(stderr, "\033[1;31merror:\033[0m failed to allocate "
                        "`solver.visited_nodes`\n");
        solver_destroy(solver);
        exit(EXIT_FAILURE);
    }

    // Allocate vector of the taken path, set to -1 by default
    int64_t initial_path_value = -1;
    solver->path_taken =
        vec_with_value(&initial_path_value, nb_nodes + 1, sizeof(int64_t));
    if (!solver->path_taken) {
        fprintf(
            stderr,
            "\033[1;31merror:\033[0m failed to allocate `solver.path_taken`\n");
        solver_destroy(solver);
        exit(EXIT_FAILURE);
    }

    // Starting at vertex #1 so the first vertex visited vertex in `path_taken`
    // is #0
    *(bool*)(vec_peek(solver->visited_nodes, 0)) = true;
    *(int64_t*)(vec_peek(solver->path_taken, 0)) = 0;

    // Set cost to infinity at the start
    solver->minimum_cost = INT64_MAX;

    return solver;
}

void solver_destroy(solver_t* solver)
{
    // Deallocate only if needed
    if (solver) {
        if (solver->visited_nodes) {
            vec_drop(solver->visited_nodes);
        }
        if (solver->path_taken) {
            vec_drop(solver->path_taken);
        }
        free(solver);
    }
}

void solve_tsp(config_t const* config, solver_t* solver)
{
    // Compute the initial lower bound at the root node using the following
    // formula:
    //     1/2 * (sum of first minimum + second minimum)
    int64_t current_bound = 0;
    for (size_t i = 0; i < config->nb_nodes; i++) {
        current_bound += (first_min(config, i) + second_min(config, i));
    }

    // Divide by two and round the lower bound to an integer
    current_bound =
        (current_bound & 1) ? current_bound / 2 + 1 : current_bound / 2;

    // Call to `branch_and_bound` for `current_weight` equal to 0 and level 1
    int64_t current_weight = 0;
    size_t level = 1;
    solve_branch_and_bound(config, solver, current_bound, current_weight,
                           level);
}

void solve_branch_and_bound(config_t const* config, solver_t* solver,
                            int64_t current_bound, int64_t current_weight,
                            size_t const level)
{
    int64_t base_node = *(int64_t*)(vec_peek(solver->path_taken, 0));
    int64_t last_node = *(int64_t*)(vec_peek(solver->path_taken, level - 1));

    // Base case: we reached the last level and have covered all the nodes
    if (level == config->nb_nodes) {
        int64_t loop_vertex = adj_matrix_get(config, last_node, base_node);
        // Check if there is an edge from the last vertex in path back to the
        // first vertex
        if (loop_vertex != 0) {
            int64_t final_weight = current_weight + loop_vertex;

            // Update final result if current result is better.
            if (final_weight < solver->minimum_cost) {
                solver->minimum_cost = final_weight;
                *(int64_t*)(vec_peek(solver->path_taken, level)) =
                    *(int64_t*)(vec_peek(solver->path_taken, 0));
            }
        }
        return;
    }

    // For any other level than the last, iterate on all vertices to build the
    // search space tree recursively
    for (size_t i = 0; i < config->nb_nodes; i++) {
        // Consider next vertex if it is not same (diagonal entry in adjacency
        // matrix and not already visited)
        int64_t new_weight = adj_matrix_get(config, last_node, i);
        bool new_state = *(bool*)(vec_peek(solver->visited_nodes, i));
        if (new_weight != 0 && new_state == false) {
            int64_t tmp = current_bound;
            current_weight += new_weight;

            // Different computation of `curr_bound` for level 1 than for the
            // other levels
            if (level == 1) {
                current_bound -=
                    (first_min(config, last_node) + first_min(config, i)) / 2;
            } else {
                current_bound -=
                    (second_min(config, last_node) + first_min(config, i)) / 2;
            }

            // `current_bound + current_weight` is the actual lower bound for
            // the node that we have reached
            // If `actual_bound < final_res`, we need to explore the node
            // further
            if (current_bound + current_weight < solver->minimum_cost) {
                *(int64_t*)(vec_peek(solver->path_taken, level)) = i;
                *(bool*)(vec_peek(solver->visited_nodes, i)) = true;

                // Call recursively for the next level
                solve_branch_and_bound(config, solver, current_bound,
                                       current_weight, level + 1);
            }

            // Else, we have to prune the node by resetting all changes to
            // `current_weight` and `current_bound`
            current_weight -= new_weight;
            current_bound = tmp;

            // Also reset the visited array
            for (size_t j = 0; j < solver->visited_nodes->len; j++) {
                *(bool*)(vec_peek(
                    solver->visited_nodes,
                    *(size_t*)(vec_peek(solver->path_taken, j)))) = false;
            }
            for (size_t j = 0; j < level; j++) {
                *(bool*)(vec_peek(
                    solver->visited_nodes,
                    *(size_t*)(vec_peek(solver->path_taken, j)))) = true;
            }
        }
    }
}

void solver_print(solver_t const* solver)
{
    printf("\nMinimum cost: %ld\n", solver->minimum_cost);
    printf("Path taken: ");
    printf("%ld", *(int64_t*)(vec_peek(solver->path_taken, 0)));
    for (size_t i = 1; i <= solver->visited_nodes->len; i++) {
        printf(" -> %ld", *(int64_t*)(vec_peek(solver->path_taken, i)));
    }
    printf("\n");
}
