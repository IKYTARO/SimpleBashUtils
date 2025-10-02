#include "cat_utils.h"

int main(int argc, char *argv[]) {
    if (argc > 1) {
        struct CatOptions options = {0};
        if (parse_options(argc, argv, &options)) {
            int file_index = optind;
            FILE *current_file = NULL;
            while (file_index < argc) {
                if ((current_file = fopen(argv[file_index], "r")) != NULL) {
                    execute_options(&options, current_file);
                    fclose(current_file);
                } else {
                    printf("%s: %s: No such file or directory\n", argv[0], argv[file_index]);
                }
                file_index++;
            }
        }
    }

    return 0;
}