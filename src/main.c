/**
 * @file    main.c
 * @brief   The program's entry point.
 * @author  Gabriel Dos Santos
 **/ 

#include "config.h"

#include <stdio.h>

int main(int argc, char* argv[argc + 1])
{
    if (argc < 2) {
        return printf("Usage: %s <CONFIG_FILE>\n", argv[0]), 1;
    }

    config_t* config = config_load(argv[1]);
    config_print(*config);
    config_destroy(config);

    return 0;
}
