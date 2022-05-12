/**
 * @file    config.c
 * @brief   Implementation of `config_t` structure's related functions.
 * @author  Gabriel Dos Santos
 **/

#include "config.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

static const uint32_t BUF_LEN = 1024;

config_t* config_load(char const* filename)
{
    if (!filename) {
        return NULL;
    }

    FILE* fp = fopen(filename, "rb");
    if (!fp) {
        return NULL;
    }

    config_t* config = malloc(sizeof(config_t));
    if (!config) {
        return NULL;
    }

    char buffer[BUF_LEN];
    uint32_t line = 1;
    size_t read_value = 0;
    while (fgets(buffer, BUF_LEN, fp)) {
        // Comments
        if (buffer[0] == '#') {
            continue;
        }

        // Handle configuration
        if (sscanf(buffer, "number of items = %zu\n", &read_value) == 1) {
            config->nb_items = read_value;
        } else if (sscanf(buffer, "max item value = %zu\n", &read_value) == 1) {
            config->max_item_value = read_value;
        } else if (sscanf(buffer, "max item weight = %zu\n", &read_value) == 1) {
            config->max_item_weight = read_value;
        } else if (sscanf(buffer, "max total weight = %zu\n", &read_value) == 1) {
            config->max_total_weight = read_value;
        } else {
            printf("error: invalid configuration at line %u: %s\n", line, buffer);
            config_destroy(config);
            return NULL;
        }
        line += 1;
    }

    fclose(fp);
    return config;
}

void config_destroy(config_t* config)
{
    free(config);
}

void config_print(config_t config)
{
    printf("Knapsack configuration:\n"
           "  Number of items:  %zu\n"
           "  Max item value:   %zu\n"
           "  Max item weight:  %zu\n"
           "  Max total weight: %zu\n",
           config.nb_items, config.max_item_value, config.max_item_weight,
           config.max_total_weight);
}
