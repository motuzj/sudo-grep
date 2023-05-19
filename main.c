#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

// prints only lines that contain "word"
int print_line_with_word(char *word, char *line) {
    char *p_word = line; // storing pointer to "word"
    int printed = 0;

    while ((p_word = strstr(p_word, word)) != NULL) {
        if (!isatty(STDOUT_FILENO) || !strcmp(word, "")) { // if stdout is terminal or "word" is empty
            printf("%s", line);
            return 0;
        }
        int position = p_word - line;

        // print characters before "word"
        for (int i = printed; i < position; i++) {
            printf("%c", line[i]);
        }

        printf("\033[32m"); // set green color
        for (int i = position; i < position + strlen(word); i++) {
            printf("%c", line[i]); // print word
        }
        printf("\033[0m"); // reset color

        printed = position + strlen(word);
        p_word += strlen(word);
    }
    if (printed) {
        // print remaining characters
        for (int i = printed; i < strlen(line); i++) {
            printf("%c", line[i]);
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
