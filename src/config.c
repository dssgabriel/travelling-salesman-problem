/**
 * @file    config.c
 * @brief   Implementation of `config_t` structure's related functions.
 * @author  Gabriel Dos Santos
 **/

#include "config.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static const uint16_t BUFFER_LEN = 4096;

inline int64_t matrix_get(vec_t* matrix, size_t const n, size_t const i,
                          size_t const j)
{
    return *(int64_t*)(vec_peek(matrix, i * n + j));
}

config_t* config_load(char const* filename)
{
    if (!filename) {
        return NULL;
    }

    FILE* fp = fopen(filename, "r");
    if (!fp) {
        return NULL;
    }

    config_t* config = malloc(sizeof(config_t));
    if (!config) {
        fclose(fp);
        return NULL;
    }

    char buf[BUFFER_LEN];
    fgets(buf, BUFFER_LEN, fp);
    sscanf(buf, "%zu\n", &config->nb_nodes);
    if (!config->nb_nodes) {
        fprintf(stderr,
                "\033[1;31merror:\033[0m %zu is an invalid number of nodes\n",
                config->nb_nodes);
        config_destroy(config);
        fclose(fp);
        exit(EXIT_FAILURE);
    }
    config->adjacency_matrix =
        vec_with_capacity(config->nb_nodes, sizeof(int64_t));

    int64_t value;
    size_t i = 0;
    while (fgets(buf, BUFFER_LEN, fp)) {
        int offset = 0;
        char* scan = buf;
        for (size_t j = 0; j < config->nb_nodes; j++) {
            sscanf(scan, "%ld%n", &value, &offset);
            vec_push(config->adjacency_matrix, &value);
            scan += offset;
        }
        i++;
        if (i > config->nb_nodes) {
            fprintf(stderr,
                    "\033[1;31merror:\033[0m too many lines in config file\n"
                    "-> number of nodes is set to %zu and current row is %zu\n",
                    config->nb_nodes, i);
            config_destroy(config);
            fclose(fp);
            exit(EXIT_FAILURE);
        }
    }

    fclose(fp);
    return config;
}

void config_destroy(config_t* config)
{
    if (config) {
        if (config->adjacency_matrix) {
            vec_drop(config->adjacency_matrix);
        }
        free(config);
    }
}

void config_print(config_t const* config)
{
    if (!config) {
        return;
    }

    printf("Travelling Salesman Problem configuration:\n"
           "  Number of nodes: %zu\n",
           config->nb_nodes);

    printf("  Adjacency matrix:\n");
    for (size_t i = 0; i < config->nb_nodes; i++) {
        printf("  ");
        for (size_t j = 0; j < config->nb_nodes; j++) {
            printf("%4ld ", matrix_get(config->adjacency_matrix,
                                      config->nb_nodes, i, j));
        }
        printf("\n");
    }
}
