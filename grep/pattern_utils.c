#include "pattern_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct pattern_array *init(int capacity) {
    struct pattern_array *pattern_array = (struct pattern_array *)calloc(1, sizeof(struct pattern_array));
    if (pattern_array) {
        pattern_array->patterns = (char **)calloc(capacity, sizeof(char *));
        pattern_array->size = 0;
        pattern_array->capacity = capacity;
    }
    return pattern_array;
}

static char *copy_string(char const *string) {
    if (string) {
        size_t lenght = strlen(string);
        char *copy = (char *)calloc(lenght + 1, sizeof(char));
        if (copy) {
            memcpy(copy, string, lenght + 1);
        }
        return copy;
    }
    return NULL;
}

void add_pattern(struct pattern_array *pattern_array, char const *pattern) {
    if (pattern) {
        if (pattern_array->size < pattern_array->capacity) {
            pattern_array->patterns[pattern_array->size] = copy_string(pattern);
            pattern_array->size++;
        }
    }
}

void add_pattern_from_file(struct pattern_array *pattern_array, char const *filename) {
    FILE *file = fopen(filename, "r");
    if (file) {
        char line[LINE_MAX_LENGTH];
        while (fgets(line, LINE_MAX_LENGTH, file)) {
            int line_length = strlen(line);
            if (line_length > 0) {
                if (line[line_length - 1] == '\n') {
                    line[line_length - 1] = '\0';
                }
                add_pattern(pattern_array, line);
            }
        }
        fclose(file);
    }
}

void free_patterns(struct pattern_array *pattern_array) {
    for (int i = 0; i < pattern_array->size; i++) {
        free(pattern_array->patterns[i]);
    }
    free(pattern_array->patterns);
    free(pattern_array);
}
