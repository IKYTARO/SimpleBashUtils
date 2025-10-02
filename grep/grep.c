#include "grep_utils.h"

int main(int argc, char *argv[]) {
    struct grep_options options = {0};
    struct pattern_array *patterns = init(DEFAULT_CAPACITY);
    if (parse_options(argc, argv, &options, patterns)) {
        int file_index = optind;
        bool single_file = (optind == argc - 1);
        FILE *current_file = NULL;
        while (file_index < argc) {
            if ((current_file = fopen(argv[file_index], "r")) != NULL) {
                execute_options(&options, patterns, current_file, argv[file_index], single_file);
                fclose(current_file);
            } else if (!options.no_messages) {
                printf("%s: %s: No such file or directory\n", argv[0], argv[file_index]);
            }
            file_index++;
        }
    }
    free_patterns(patterns);

    return 0;
}