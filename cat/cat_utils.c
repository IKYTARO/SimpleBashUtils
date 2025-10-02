#include "cat_utils.h"

#include <string.h>

static char const *short_options = "beEnstTvV";

static struct option const long_options[] = {{"number-nonblank", no_argument, NULL, 'b'},
                                             {"show-ends", no_argument, NULL, 'E'},
                                             {"number", no_argument, NULL, 'n'},
                                             {"squeeze-blank", no_argument, NULL, 's'},
                                             {"show-tabs", no_argument, NULL, 'T'},
                                             {"show-nonprinting", no_argument, NULL, 'v'},
                                             {"version", no_argument, NULL, 'V'},
                                             {"help", no_argument, NULL, 0},
                                             {NULL, 0, NULL, 0}};

static void print_help_message() {
    printf(
        "Cat is one of the most frequently used commands on Unix-like operating systems. It\n\
has three related functions with regard to text files: displaying them, combining\n\
copies of them and creating new ones.\n\n\
Usage: cat [OPTION] [FILE]...\n\n\
Options:\n\
    -b, --number-nonblank   numbers only non-empty lines\n\
    -e                      equivalent to -vE\n\
    -E, --show-ends         display end-of-line characters as $\n\
    -n, --number            number all output lines\n\
    -s, --squeeze-blank     squeeze multiple adjacent blank lines\n\
    -t                      equivalent to -vT\n\
    -T, --show-tabs         display tabs as ^I\n\
    -v, --show-nonprinting  use ^ and M- notation, except for LFD and TAB\n\
    -V, --version           output version information and exit\n\
        --help              display this help and exit\n");
}

static void print_version(char const *app_name) {
    printf(
        "The application %s was developed in accordance with the utility\n\
cat of version (GNU coreutils) 8.32\n\n\
Written by Nikolay Martynenko.\n",
        app_name);
}

static void print_error_message(char const *app_name) {
    printf("Try '%s --help' for more information.\n", app_name);
}

int parse_options(int argc, char *argv[], struct CatOptions *options) {
    int option, option_index, parsing_status = 1;
    while ((option = getopt_long(argc, argv, short_options, long_options, &option_index)) != -1) {
        switch (option) {
            case 'b':
                options->number = true;
                options->number_nonblank = true;
                break;
            case 'e':
                options->show_ends = true;
                options->show_nonprinting = true;
                break;
            case 'E':
                options->show_ends = true;
                break;
            case 'n':
                options->number = true;
                break;
            case 's':
                options->squeeze_blank = true;
                break;
            case 't':
                options->show_tabs = true;
                options->show_nonprinting = true;
                break;
            case 'T':
                options->show_tabs = true;
                break;
            case 'v':
                options->show_nonprinting = true;
                break;
            case 'V':
                print_version(argv[0]);
                parsing_status = 0;
                break;
            case 0:
                print_help_message();
                parsing_status = 0;
                break;
            case '?':
                print_error_message(argv[0]);
                parsing_status = 0;
                break;
        }
    }
    return parsing_status;
}

static void handle_symbol_options(struct CatOptions const *options, unsigned char symbol) {
    if (options->show_nonprinting) {
        if (symbol >= 32) {
            if (symbol < 127)
                putchar(symbol);
            else if (symbol == 127)
                printf("^?");
            else {
                printf("M-");
                if (symbol >= 128 + 32) {
                    if (symbol < 128 + 127)
                        putchar(symbol - 128);
                    else
                        printf("^?");
                } else
                    printf("^%c", symbol - 128 + 64);
            }
        } else if (symbol == '\t') {
            if (options->show_tabs)
                printf("^I");
            else
                putchar('\t');
        } else if (symbol == '\n') {
            if (options->show_ends)
                printf("$\n");
            else
                putchar('\n');
        } else {
            printf("^%c", symbol + 64);
        }
    } else {
        if (symbol == '\t' && options->show_tabs) {
            printf("^I");
        } else if (symbol == '\n' && options->show_ends) {
            printf("$\n");
        } else
            putchar(symbol);
    }
}

void execute_options(struct CatOptions const *options, FILE *file) {
    char current_line[LINE_MAX_LENGTH];
    int line_number = 1;
    bool prev_line_blank = 0;
    while (fgets(current_line, LINE_MAX_LENGTH, file)) {
        int line_length = strlen(current_line);
        bool line_blank = (line_length == 1 && current_line[0] == '\n');

        if (options->squeeze_blank) {
            if (line_blank && prev_line_blank) continue;
            prev_line_blank = line_blank;
        }

        if (options->number_nonblank && !line_blank) {
            printf("%6d\t", line_number);
            line_number++;
        }

        if (options->number && !options->number_nonblank) {
            printf("%6d\t", line_number);
            line_number++;
        }

        for (int i = 0; i < line_length; i++) {
            handle_symbol_options(options, (unsigned char)current_line[i]);
        }
    }
}
