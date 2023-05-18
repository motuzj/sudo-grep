#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(void) {
    char *str = NULL;
    size_t n = 0;
    errno = 0;

    while (getline(&str, &n, stdin) != -1) {
        printf("%s", str);
    }

    free(str);

    if (errno != 0) {
        fprintf(stderr, "%s\n", strerror(errno));
        return 1;
    }
    return 0;
}
