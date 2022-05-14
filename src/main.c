/**
 * @file    main.c
 * @brief   The program's entry point.
 * @author  Gabriel Dos Santos
 **/

#include "config.h"
#include "solver.h"

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

int main(int argc, char* argv[argc + 1])
{
    if (argc < 2) {
        return printf("Usage: %s <CONFIG_FILE>\n", argv[0]), 1;
    }

    config_t* config = config_load(argv[1]);
    config_print(config);
    solver_t* solver = solver_init(config->nb_nodes);

    struct timespec before, after;
    clock_gettime(CLOCK_MONOTONIC_RAW, &before);
    solve_tsp(config, solver);
    clock_gettime(CLOCK_MONOTONIC_RAW, &after);

    solver_print(solver);
    printf("Finished in %.3lfs", after.tv_sec - before.tv_sec +
                                 (after.tv_nsec - before.tv_nsec) / 1e9);

    solver_destroy(solver);
    config_destroy(config);
    return 0;
}
