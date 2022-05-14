/**
 * @file    main.c
 * @brief   The program's entry point.
 * @author  Gabriel Dos Santos
 **/

#include "config.h"

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define N 13

// final_path[] stores the final solution ie, the
// path of the salesman.
int64_t final_path[N + 1];

// visited[] keeps track of the already visited nodes
// in a particular path
bool visited[N];

// Stores the final minimum weight of shortest tour.
int64_t final_res = INT64_MAX;

// Function to copy temporary solution to
// the final solution
void copy_to_final(size_t const n, int64_t const* curr_path)
{
    for (size_t i = 0; i < n; i++) {
        final_path[i] = curr_path[i];
    }
    final_path[n] = curr_path[0];
}

// Function to find the minimum edge cost
// having an end at the vertex i
int64_t first_min(config_t const* config, size_t const i)
{
    int64_t min = INT64_MAX;
    for (size_t j = 0; j < config->nb_nodes; j++) {
        int64_t current =
            matrix_get(config->adjacency_matrix, config->nb_nodes, i, j);
        if (current < min && i != j) {
            min = current;
        }
    }
    return min;
}

// function to find the second minimum edge cost
// having an end at the vertex i
int64_t second_min(config_t const* config, size_t const i)
{
    int64_t first = INT64_MAX;
    int64_t second = INT64_MAX;
    for (size_t j = 0; j < config->nb_nodes; j++) {
        if (i == j) {
            continue;
        }

        int64_t current =
            matrix_get(config->adjacency_matrix, config->nb_nodes, i, j);
        if (current <= first) {
            second = first;
            first = current;
        } else if (current <= second && current != first) {
            second = current;
        }
    }
    return second;
}

// function that takes as arguments:
// curr_bound -> lower bound of the root node
// curr_weight-> stores the weight of the path so far
// level-> current level while moving in the search
//         space tree
// curr_path[] -> where the solution is being stored which
//                would later be copied to final_path[]
void branch_and_bound(config_t const* config, int64_t curr_bound,
                      int64_t curr_weight, size_t const level,
                      int64_t* curr_path)
{
    // base case is when we have reached level N which
    // means we have covered all the nodes once
    if (level == N) {
        // check if there is an edge from last vertex in
        // path back to the first vertex
        int64_t current = matrix_get(config->adjacency_matrix, config->nb_nodes,
                                     curr_path[level - 1], curr_path[0]);
        if (current != 0) {
            // curr_res has the total weight of the
            // solution we got
            int64_t curr_res = curr_weight + current;

            // Update final result and final path if
            // current result is better.
            if (curr_res < final_res) {
                copy_to_final(config->nb_nodes, curr_path);
                final_res = curr_res;
            }
        }
        return;
    }

    // for any other level iterate for all vertices to
    // build the search space tree recursively
    for (size_t i = 0; i < config->nb_nodes; i++) {
        // Consider next vertex if it is not same (diagonal
        // entry in adjacency matrix and not visited
        // already)

        int64_t current = matrix_get(config->adjacency_matrix, config->nb_nodes,
                                     curr_path[level - 1], i);
        if (current != 0 && visited[i] == false) {
            int64_t tmp = curr_bound;
            curr_weight += current;

            // different computation of curr_bound for
            // level 2 from the other levels
            if (level == 1) {
                curr_bound -= ((first_min(config, curr_path[level - 1]) +
                                first_min(config, i)) /
                               2);
            } else {
                curr_bound -= ((second_min(config, curr_path[level - 1]) +
                                first_min(config, i)) /
                               2);
            }

            // curr_bound + curr_weight is the actual lower bound
            // for the node that we have arrived on
            // If current lower bound < final_res, we need to explore
            // the node further
            if (curr_bound + curr_weight < final_res) {
                curr_path[level] = i;
                visited[i] = true;

                // call TSPRec for the next level
                branch_and_bound(config, curr_bound, curr_weight, level + 1,
                                 curr_path);
            }

            // Else we have to prune the node by resetting
            // all changes to curr_weight and curr_bound
            curr_weight -= current;
            curr_bound = tmp;

            // Also reset the visited array
            memset(visited, false, sizeof(visited));
            for (size_t j = 0; j <= level - 1; j++) {
                visited[curr_path[j]] = true;
            }
        }
    }
}

// This function sets up final_path[]
void TSP(config_t* const config)
{
    int64_t curr_path[config->nb_nodes + 1];

    // Calculate initial lower bound for the root node
    // using the formula 1/2 * (sum of first min +
    // second min) for all edges.
    // Also initialize the curr_path and visited array
    int64_t curr_bound = 0;
    memset(curr_path, -1, sizeof(curr_path));
    memset(visited, 0, sizeof(visited));

    // Compute initial bound
    for (size_t i = 0; i < config->nb_nodes; i++) {
        curr_bound += (first_min(config, i) + second_min(config, i));
    }

    // Rounding off the lower bound to an integer
    curr_bound = (curr_bound & 1) ? curr_bound / 2 + 1 : curr_bound / 2;

    // We start at vertex 1 so the first vertex
    // in curr_path[] is 0
    visited[0] = true;
    curr_path[0] = 0;

    // Call to TSPRec for curr_weight equal to
    // 0 and level 1
    branch_and_bound(config, curr_bound, 0, 1, curr_path);
}

int main(int argc, char* argv[argc + 1])
{
    if (argc < 2) {
        return printf("Usage: %s <CONFIG_FILE>\n", argv[0]), 1;
    }

    config_t* config = config_load(argv[1]);
    config_print(config);
    TSP(config);

    printf("\nMinimum cost: %ld\n", final_res);
    printf("Path taken: ");
    printf("%ld", final_path[0]);
    for (size_t i = 1; i <= config->nb_nodes; i++) {
        printf(" -> %ld", final_path[i]);
    }
    printf("\n");

    config_destroy(config);
    return 0;
}
