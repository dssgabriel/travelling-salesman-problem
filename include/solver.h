/**
 * @file    solver.h
 * @brief   Declaration of `solver_t` structure and its related functions.
 * @author  Gabriel Dos Santos
 **/

#pragma once

#include "config.h"
#include "vec.h"

typedef struct solver_t {
    vec_t* visited_nodes;
    vec_t* path_taken;
    int64_t minimum_cost;
} solver_t;

/**
 * Initialize the solver based on the number of nodes set in the configuration.
 * 
 * @param nb_nodes Number of nodes in the problem.
 * @return The initialized solver structure.
 */
solver_t* solver_init(size_t const nb_nodes);

/**
 * Deallocates the solver.
 * 
 * @param solver Solver to deallocate.
 */
void solver_destroy(solver_t* solver);

/**
 * Initialize the TSP solving algorithm and calls the branch-and-bound
 * algorithm.
 *
 * @param config Configuration of the problem.
 * @param solver Pre-initialized solver.
 **/
void solve_tsp(config_t const* config, solver_t* solver);

/**
 * Performs the branch-and-bound algorithm on the given graph to solve the TSP
 * problem.
 * 
 * @param curr_bound Lower bound of the root node.
 * @param curr_weight Weight of the path so far
 * @param level Current level while moving in the search space tree
 * @param curr_path Current solution.
 **/
void solve_branch_and_bound(config_t const* config, solver_t* solver,
                            int64_t current_bound, int64_t current_weight,
                            size_t const level);

/**
 * Prints the solution computed by the solver.
 * 
 * @param Solver to print the found solution.
 */
void solver_print(solver_t const* solver);
