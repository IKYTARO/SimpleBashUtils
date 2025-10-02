#ifndef CATUTILS_H
#define CATUTILS_H

#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>

#define LINE_MAX_LENGTH 1024

struct CatOptions {
    bool show_nonprinting;
    bool show_tabs;
    bool number;
    bool number_nonblank;
    bool show_ends;
    bool squeeze_blank;
};

int parse_options(int argc, char *argv[], struct CatOptions *options);
void execute_options(struct CatOptions const *options, FILE *file);

#endif