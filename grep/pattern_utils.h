#ifndef PATTERNS_H
#define PATTERNS_H

#define DEFAULT_CAPACITY 150
#define LINE_MAX_LENGTH 1024

struct pattern_array {
    char **patterns;
    int size;
    int capacity;
};

struct pattern_array *init(int capacity);
void add_pattern(struct pattern_array *pattern_array, char const *pattern);
void add_pattern_from_file(struct pattern_array *pattern_array, char const *filename);
void free_patterns(struct pattern_array *pattern_array);

#endif