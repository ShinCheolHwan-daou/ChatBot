//
// Created by daou_tlscjfghks on 25. 3. 26.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dotenv.h"

void dotenv_load(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Could not open .env file");
        return;
    }

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

    // todo:: delete
    const char *DB_HOST = getenv("DB_HOST");
    const char *DB_USERNAME = getenv("DB_USERNAME");
    const char *DB_PASSWORD = getenv("DB_PASSWORD");
    const char *PERPLEXITY_API_KEY = getenv("PERPLEXITY_API_KEY");
    printf("DB_HOST = %s\n", DB_HOST);
    printf("DB_USERNAME = %s\n", DB_USERNAME);
    printf("DB_PASSWORD = %s\n", DB_PASSWORD);
    printf("PERPLEXITY_API_KEY = %s\n", PERPLEXITY_API_KEY);
}