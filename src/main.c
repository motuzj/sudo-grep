#define _GNU_SOURCE
#include <ctype.h>
#include <errno.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// options
bool ignore_case = false;
bool invert_match = false;

// prints only lines that contain "word"
int print_line_with_word(const char *word, const char *line, int is_tty) {
    int found_cnt = 0;
    const char *search_start = line; // storing pointer to "word"
    while (search_start && *search_start) {
        const char *found_word;
        if (ignore_case) {
            found_word = strcasestr(search_start, word);
        } else {
            found_word = strstr(search_start, word);
        }

        if (found_word && !invert_match) {
            found_cnt++;
            for (; search_start < found_word; search_start++) {
                printf("%c", *search_start);
            }
            if (is_tty) {
                printf("\033[32m"); // set green color
            }

            printf("%.*s", (int)strlen(word), search_start); // https://stackoverflow.com/a/256223

            if (is_tty) {
                printf("\033[0m"); // reset color
            }
            search_start += strlen(word);
        } else if (!found_word && invert_match) {
            printf("%s", line);
            break;
        } else {
            break;
        }
    }
    if (found_cnt > 0) {
        // print remaining characters
        printf("%s", search_start);
    }
    return 0;
}

int main(int argc, char *argv[]) {
    // clang-format off
    struct option long_options[] = {
        {"invert-match", no_argument, 0, 'v'}, 
        {"ignore-case", no_argument, 0, 'i'}
    };
    // clang-format on
    int opt;
    while ((opt = getopt_long(argc, argv, "iv", long_options, NULL)) != -1) {
        switch (opt) {
            case 'i':
                ignore_case = true;
                break;
            case 'v':
                invert_match = true;
        }
    }

    const char *word;
    if (argv[optind] != NULL) { // if it has argument
        word = argv[optind];
    } else {
        printf("Error: Too few arguments\n");
        return 1;
    }

    char *str = NULL;
    size_t n = 0;
    int is_tty = isatty(STDOUT_FILENO);
    errno = 0;

    while (getline(&str, &n, stdin) != -1) {
        if (errno != 0) {
            fprintf(stderr, "Error: %s\n", strerror(errno));
            break;
        }
        print_line_with_word(word, str, is_tty);
    }

    free(str);
    return 0;
}
