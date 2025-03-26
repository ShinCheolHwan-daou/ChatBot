//
// Created by daou_tlscjfghks on 25. 3. 26.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dotenv.h"

void dotenv_load(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Could not open .env file");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char *key = strtok(line, "=");
        char *value = strtok(NULL, "\n");

        if (key && value) {
            if (_putenv_s(key, value) != 0) {
                fprintf(stderr, "Failed to set environment variable: %s\n", key);
            }
        }
    }
    fclose(file);
}
