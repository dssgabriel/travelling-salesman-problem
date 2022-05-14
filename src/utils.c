/**
 * @file    utils.c
 * @brief   Implementation of utility functions needed by the program but not
 *          directly related to the branch-and-bound algorithm.
 * @author  Gabriel Dos Santos
 **/

#include "utils.h"

inline int64_t adj_matrix_get(config_t const* config, size_t i, size_t j)
{
    return *(
        int64_t*)(vec_peek(config->adjacency_matrix, i * config->nb_nodes + j));
}

int64_t first_min(config_t const* config, size_t const i)
{
    int64_t min = INT64_MAX;
    for (size_t j = 0; j < config->nb_nodes; j++) {
        int64_t current = adj_matrix_get(config, i, j);
        if (current < min && i != j) {
            min = current;
        }
    }
    return min;
}

int64_t second_min(config_t const* config, size_t const i)
{
    int64_t first = INT64_MAX;
    int64_t second = INT64_MAX;
    for (size_t j = 0; j < config->nb_nodes; j++) {
        if (i == j) {
            continue;
        }

        int64_t current = adj_matrix_get(config, i, j);
        if (current <= first) {
            second = first;
            first = current;
        } else if (current <= second && current != first) {
            second = current;
        }
    }
    return second;
}

void copy_optimal(solver_t* solver)
{
    for (size_t i = 0; i < solver->path_taken->len - 1; i++) {
        *(int64_t*)(vec_peek(solver->optimal_path, i)) =
            *(int64_t*)(vec_peek(solver->path_taken, i));
    }
    *(int64_t*)(vec_peek(solver->optimal_path, solver->path_taken->len - 1)) =
        *(int64_t*)(vec_peek(solver->path_taken, 0));
}
