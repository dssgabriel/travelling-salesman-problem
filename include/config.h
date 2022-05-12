/**
 * @file    config.h
 * @brief   Declaration of the `config_t` structure and its related functions.
 * @author  Gabriel Dos Santos
 **/

#pragma once

#include <stddef.h>

typedef struct config_t {
    size_t nb_items;
    size_t max_item_value;
    size_t max_item_weight;
    size_t max_total_weight;
} config_t;

config_t* config_load(char const* filename);
void config_destroy(config_t* config);
void config_print(config_t config);
