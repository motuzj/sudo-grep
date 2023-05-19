#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

// prints only lines that contain "word"
int print_line_with_word(char *word, char *line) {
    char *p_word; // storing pointer to "word"

    if ((p_word = strstr(line, word)) != NULL) {
        if (isatty(STDOUT_FILENO)) { // if stdout is terminal
            int position = p_word - line;

            for (int i = 0; i < strlen(line); i++) {
                if (i >= position && i <= position + strlen(word) - 1) {
                    printf("\033[32m%c\033[0m", line[i]);
                } else {
                    printf("%c", line[i]);
                }
            }
        } else {
            printf("%s", line);
        }
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
    errno = 0;

    while (getline(&str, &n, stdin) != -1) {
        if (errno != 0) {
            fprintf(stderr, "Error: %s\n", strerror(errno));
            break;
        }
        print_line_with_word(word, str);
        errno = 0; // clear ENOTTY error caused by isatty()
    }

    free(str);
    return 0;
}
