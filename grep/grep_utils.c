
#include "grep_utils.h"

#include <stdlib.h>
#include <string.h>

static const char *short_options = "e:ivclnf:shoV";

static const struct option long_options[] = {{"regexp", required_argument, NULL, 'e'},
                                             {"ignore-case", no_argument, NULL, 'i'},
                                             {"invert-match", no_argument, NULL, 'v'},
                                             {"count", no_argument, NULL, 'c'},
                                             {"files-with-matches", no_argument, NULL, 'l'},
                                             {"line-number", no_argument, NULL, 'n'},
                                             {"file", required_argument, NULL, 'f'},
                                             {"no-filename", no_argument, NULL, 'h'},
                                             {"only-matching", no_argument, NULL, 'o'},
                                             {"no-messages", no_argument, NULL, 's'},
                                             {"version", no_argument, NULL, 'V'},
                                             {"help", no_argument, NULL, 0},
                                             {NULL, 0, NULL, 0}};

static void print_help_message() {
    printf(
        "Grep was first included in version 4 of Unix. Noting that it is \"generally cited as the\n\
prototypical software tool\", McIlroy credited grep with \"irrevocably entrenching\" Thompson's tool\n\
philosophy in Unix.\n\n\
Usage: ./my_grep [OPTION]... PATTERNS [FILE]...\n\
Search for PATTERNS in each FILE.\n\
Example: ./my_grep -i 'hello world' menu.h main.c\n\n\
Pattern selection and interpretation:\n\
    -e, --regexp=PATTERNS     use PATTERNS for matching\n\
    -f, --file=FILE           take PATTERNS from FILE\n\
    -i, --ignore-case         ignore case distinctions in patterns and data\n\n\
Miscellaneous:\n\
    -v, --invert-match        select non-matching lines\n\
    -V, --version             display version information and exit\n\
        --help                display this help text and exit\n\n\
Output control:\n\
    -s, --no-messages         suppress error messages\n\
    -n, --line-number         print line number with output lines\n\
    -h, --no-filename         suppress the file name prefix on output\n\
    -o, --only-matching       show only nonempty parts of lines that match\n\
    -l, --files-with-matches  print only names of FILEs with selected lines\n\
    -c, --count               print only a count of selected lines per FILE\n");
}

static void print_version() {
    printf(
        "s21_grep 1.0\n\
The application s21_grep was developed in accordance with the utility\n\
grep of version (GNU grep) 3.7\n\n\
Written by Nikolay Martynenko.\n");
}

static void print_error_message(char const *app_name) {
    printf("Try '%s --help' for more information.\n", app_name);
}

static void compile_regex_patterns(regex_t *regex_array, struct pattern_array *pattern_array,
                                   int compile_flag) {
    for (int i = 0; i < pattern_array->size; i++) {
        if (regcomp(&regex_array[i], pattern_array->patterns[i], compile_flag) != 0) {
            perror("Can not compiling regex.");
        }
    }
}

static void free_regex_patterns(regex_t *regex_arr, int patterns_count) {
    for (int i = 0; i < patterns_count; i++) {
        regfree(&regex_arr[i]);
    }
}

static void building_hierarchy(struct grep_options *options, int *flag) {
    if (options->list_files) {
        options->count_matches = false;
        options->out_line = false;
        options->only_matching = false;
    }

    if (options->count_matches) {
        options->out_line = false;
        options->only_matching = false;
    }

    if (options->out_invert && options->only_matching) {
        *flag = 0;
    }

    if (options->no_messages) {
        options->output_option_error = false;
    }

    if (options->output_version) {
        options->output_help = false;
        options->output_option_error = false;
    }

    if (options->output_help) {
        options->output_option_error = false;
    }
}

int parse_options(int argc, char *argv[], struct grep_options *options, struct pattern_array *pattern_array) {
    int option, optinedx, continue_processing = 1;
    while ((option = getopt_long(argc, argv, short_options, long_options, &optinedx)) != -1) {
        switch (option) {
            case 'e':
                options->regexp = true;
                add_pattern(pattern_array, optarg);
                break;
            case 'i':
                options->match_icase = true;
                break;
            case 'v':
                options->out_invert = true;
                break;
            case 'c':
                options->count_matches = true;
                break;
            case 'l':
                options->list_files = true;
                break;
            case 'n':
                options->out_line = true;
                break;
            case 'f':
                options->regexp = true;
                add_pattern_from_file(pattern_array, optarg);
                break;
            case 's':
                options->no_messages = true;
                break;
            case 'h':
                options->no_filename = true;
                break;
            case 'o':
                options->only_matching = true;
                break;
            case 'V':
                options->output_version = true;
                continue_processing = 0;
                break;
            case 0:
                options->output_help = true;
                continue_processing = 0;
                break;
            case '?':
                options->output_option_error = true;
                continue_processing = 0;
                break;
        }
    }

    building_hierarchy(options, &continue_processing);

    if (!options->regexp && continue_processing) {
        add_pattern(pattern_array, argv[optind]);
        optind++;
    }

    if (options->output_version) print_version();
    if (options->output_help) print_help_message();
    if (options->output_option_error) print_error_message(argv[0]);

    return continue_processing;
}

static void output_linenumber(int linenumber) { printf("%d:", linenumber); }

static void output_filename(char const *filename, bool with_colon) {
    printf("%s", filename);
    if (with_colon) {
        printf(":");
    }
}

static void output_count(int count, char const *filename, bool with_filename) {
    if (with_filename) {
        output_filename(filename, true);
    }

    printf("%d\n", count);
}

static void output_matched_string(char const *string, char const *filename, bool is_single_file,
                                  int line_number, struct grep_options const *options) {
    bool with_filename = !is_single_file && !options->no_filename;
    if (with_filename) output_filename(filename, true);
    if (options->out_line) output_linenumber(line_number);

    printf("%s", string);

    if (string[strlen(string) - 1] != '\n') {
        printf("\n");
    }
}

static void output_result(struct grep_options const *options, int match_line_count, char const *filename,
                          bool is_single_file) {
    if (options->list_files && match_line_count > 0) {
        output_filename(filename, false);
        printf("\n");
    }
    if (options->count_matches) {
        bool with_filename = !is_single_file && !options->no_filename;
        output_count(match_line_count, filename, with_filename);
    }
}

static bool process_line_for_matches(char const *line, regex_t const *regex_arr, int patterns_count,
                                     char const *filename, bool is_single_file, int line_number,
                                     struct grep_options const *options) {
    bool found_any_match = false;
    bool output_line = !options->list_files && !options->count_matches;

    for (int i = 0; i < patterns_count; i++) {
        regmatch_t match;
        if (regexec(&regex_arr[i], line, 1, &match, REG_NOTEOL) == 0) found_any_match = true;
    }

    found_any_match = (options->out_invert) ? !found_any_match : found_any_match;

    if (found_any_match && output_line) {
        output_matched_string(line, filename, is_single_file, line_number, options);
    }

    return found_any_match;
}

static bool handle_only_matches(char const *line, int line_number, regex_t const *regex_arr,
                                int patterns_count, struct grep_options *options, char const *filename,
                                bool is_single_file) {
    bool found_any_match = false;
    regmatch_t match;

    for (int i = 0; i < patterns_count; i++) {
        char const *cursor = line;
        while (regexec(&regex_arr[i], cursor, 1, &match, 0) == 0) {
            if (match.rm_so != match.rm_eo) {
                found_any_match = true;

                int match_len = match.rm_eo - match.rm_so;
                char *match_string = (char *)calloc(match_len + 1, sizeof(char));
                if (match_string) {
                    strncpy(match_string, cursor + match.rm_so, match_len);
                    match_string[match_len] = '\0';
                    output_matched_string(match_string, filename, is_single_file, line_number, options);
                    free(match_string);
                }
            }
            cursor += match.rm_eo;
        }
    }
    return found_any_match;
}

void execute_options(struct grep_options *options, struct pattern_array *pattern_array, FILE *file,
                     char const *filename, bool is_single_file) {
    char line[LINE_MAX_LENGTH];
    int line_number = 0, match_line_count = 0;

    regex_t regex_array[pattern_array->size];
    int compile_flag = options->match_icase ? REG_ICASE | REG_EXTENDED : REG_EXTENDED;
    compile_regex_patterns(regex_array, pattern_array, compile_flag);

    while (fgets(line, LINE_MAX_LENGTH, file)) {
        line_number++;
        bool has_match = false;
        if (options->only_matching) {
            has_match = handle_only_matches(line, line_number, regex_array, pattern_array->size, options,
                                            filename, is_single_file);
        } else {
            has_match = process_line_for_matches(line, regex_array, pattern_array->size, filename,
                                                 is_single_file, line_number, options);
        }
        if (has_match) {
            match_line_count++;
        }
    }

    output_result(options, match_line_count, filename, is_single_file);

    free_regex_patterns(regex_array, pattern_array->size);
}