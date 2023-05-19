#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

// prints only lines that contains "word"
int print_line_with_word(char *word, char *line) {
    if (strstr(line, word) != NULL) {
        printf("%s", line);
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
        print_line_with_word(word, str);
    }

    free(str);

    if (errno != 0) {
        fprintf(stderr, "Error: %s\n", strerror(errno));
        return 1;
    }
    return 0;
}
