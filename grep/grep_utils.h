#ifndef GREP_UTILS_H
#define GREP_UTILS_H

#include <getopt.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>

#include "pattern_utils.h"

struct grep_options {
    bool regexp;
    bool match_icase;
    bool out_invert;
    bool count_matches;
    bool list_files;
    bool out_line;
    bool no_messages;
    bool no_filename;
    bool only_matching;
    bool output_help;
    bool output_version;
    bool output_option_error;
};

int parse_options(int argc, char *argv[], struct grep_options *options, struct pattern_array *pattern_array);

void execute_options(struct grep_options *options, struct pattern_array *pattern_array, FILE *file,
                     char const *filename, bool is_single_file);

#endif
