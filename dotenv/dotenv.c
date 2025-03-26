//
// Created by daou_tlscjfghks on 25. 3. 26.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dotenv.h"

void load_env(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Could not open .env file");
        return;
    }
    printf("123123\n");

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char* key = strtok(line, "=");
        char* value = strtok(NULL, "\n");

        if (key && value) {
            // Windows에서는 _putenv_s() 사용
            if (_putenv_s(key, value) != 0) {
                fprintf(stderr, "Failed to set environment variable: %s\n", key);
            }
        }
    }
    fclose(file);
}