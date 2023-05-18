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
    if (errno != 0) {
        printf("%s\n", strerror(errno));
        free(str);
        return 1;
    }

    free(str);
    return 0;
}
