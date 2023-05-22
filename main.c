#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

// prints only lines that contain "word"
int print_line_with_word(char *word, const char *line, int is_tty) {
    int found_cnt = 0;
    const char *search_start = line; // storing pointer to "word"
    while (search_start && *search_start) {
        const char *found_word = strstr(search_start, word);
        if (found_word) {
            found_cnt++;
            for (; search_start < found_word; search_start++) {
                printf("%c", *search_start);
            }
            if (is_tty) {
                printf("\033[32m"); // set green color
            }
            printf("%s", word);
            if (is_tty) {
                printf("\033[0m"); // reset color
            }
            search_start += strlen(word);
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
    char *word;
    if (argc >= 2) { // if it has argument
        word = argv[1];
    } else { // will print complete stdin, if no argument is given
        word = "";
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
