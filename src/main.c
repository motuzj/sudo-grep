#define _GNU_SOURCE
#include <ctype.h>
#include <dirent.h>
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
bool recursive = false;

bool is_directory = false;

// prints only lines that contain "word"
int print_line_with_word(const char *word, const char *line, int is_tty, const char *filename) {
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

            if (is_directory && found_cnt == 1 && filename != NULL) { // print file name
                if (is_tty) {
                    printf("\033[31m%s\033[35m:\033[0m", filename);
                } else {
                    printf("%s:", filename);
                }
            }

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

int grep_file(const char *word, FILE *file, int is_tty, const char *filename) {
    char *line = NULL;
    size_t n = 0;
    errno = 0;

    while (getline(&line, &n, file) != -1) {
        if (errno != 0) {
            fprintf(stderr, "Error: %s\n", strerror(errno));
            break;
        }
        print_line_with_word(word, line, is_tty, filename);
    }
    free(line);
    return 0;
}

int grep_directory(const char *word, const char *directory, int is_tty) {
    DIR *dir = opendir(directory);
    if (dir == NULL) {
        fprintf(stderr, "Error: %s: Directory can't be opened\n", directory);
        return 1;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        is_directory = true;
        if (entry->d_type == DT_REG) { // regular file
            char path[PATH_MAX];
            snprintf(path, sizeof(path), "%s/%s", directory, entry->d_name);
            FILE *file = fopen(path, "r");
            if (file == NULL) {
                fprintf(stderr, "Error: %s: File can't be opened\n", path);
                continue;
            }

            grep_file(word, file, is_tty, path);
            fclose(file);
        } else if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) { // folder
            char path[PATH_MAX];
            snprintf(path, sizeof(path), "%s/%s", directory, entry->d_name);
            grep_directory(word, path, is_tty);
        }
    }

    closedir(dir);
    return 0;
}

int main(int argc, char *argv[]) {
    int is_tty = isatty(STDOUT_FILENO);
    bool print_filenames = false;

    // clang-format off
    struct option long_options[] = {
        {"invert-match", no_argument, 0, 'v'}, 
        {"ignore-case", no_argument, 0, 'i'},
        {"recursive", no_argument, 0, 'R'}
    };
    // clang-format on
    int opt;
    while ((opt = getopt_long(argc, argv, "ivR", long_options, NULL)) != -1) {
        switch (opt) {
            case 'i':
                ignore_case = true;
                break;
            case 'v':
                invert_match = true;
                break;
            case 'R':
                recursive = true;
                break;
        }
    }

    const char *word;
    if (argv[optind] != NULL) { // if it has argument
        word = argv[optind];
    } else {
        printf("Error: Too few arguments\n");
        return 1;
    }

    // read from files or directories provided in arguments
    if (argv[optind + 1] != NULL) {
        for (int i = optind + 1; argv[i] != NULL; i++) {
            if (recursive) {
                grep_directory(word, argv[i], is_tty);
            } else {
                FILE *file = fopen(argv[i], "r");
                if (file == NULL) {
                    fprintf(stderr, "Error: %s: File can't be opened\n", argv[i]);
                    continue;
                }

                grep_file(word, file, is_tty, NULL);
                fclose(file);
            }
        }
    } else {
        grep_file(word, stdin, is_tty, NULL);
    }
    return 0;
}
