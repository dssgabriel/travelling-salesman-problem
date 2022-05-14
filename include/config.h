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

config_t* config_load(char const* filename);
void config_destroy(config_t* config);
void config_print(config_t const* config);

int64_t matrix_get(vec_t* matrix, size_t n, size_t i, size_t j);
