/**
 * @file    utils.h
 * @brief   Declaration of utility functions needed by the program but not
 *          directly related to the branch-and-bound algorithm.
 * @author  Gabriel Dos Santos
 **/

#pragma once

#include "config.h"
#include "vec.h"

/**
 * Get a particular value from the adjacency matrix.
 * This is to simplify the vector's acesses as it stores data in a single
 * dimension.
 * 
 * @param config Configuration of the TSP problem.
 * @param i X coordinate.
 * @param j Y coordinate.
 * @return Value at the target address.
 */
int64_t adj_matrix_get(config_t const* config, size_t i, size_t j);

/**
 * Gets the minimum weight in the adjacency matrix considering a given `i`
 * coordinate.
 * 
 * @param config Config holding the adjacency matrix.
 * @param i X coordinate.
 */
int64_t first_min(config_t const* config, size_t i);

/**
 * Gets the _second_ minimum weight in the adjacency matrix considering a given
 * `i` coordinate.
 * 
 * @param config Config holding the adjacency matrix.
 * @param i X coordinate.
 */
int64_t second_min(config_t const* config, size_t i);
