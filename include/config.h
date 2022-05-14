/**
 * @file    config.h
 * @brief   Declaration of the `config_t` structure and its related functions.
 * @author  Gabriel Dos Santos
 **/

#pragma once

#include "vec.h"

#include <stddef.h>

typedef struct config_t {
    size_t nb_nodes;
    vec_t* adjacency_matrix;
} config_t;

/**
 * Load a configuration file.
 * The latter should have the following construction:
 * 
 *   NB_NODES
 *        0     WEIGHT_0-1 WEIGHT_0-2 ...
 *   WEIGHT_1-0      0     WEIGHT_1-2 ...
 *   WEIGHT_2-0 WEIGHT_2-1      0     ...
 *   ...
 * 
 * @param filename Path to the configuration file.
 */
config_t* config_load(char const* filename);

/**
 * Deallocates the configuration.
 * 
 * @param config Configuration to deallocate.
 */
void config_destroy(config_t* config);

/**
 * Prints the configuration to the terminal.
 * 
 * @param config Configuration to print.
 */
void config_print(config_t const* config);
