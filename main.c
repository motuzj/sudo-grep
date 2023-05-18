#include <stdio.h>
#include <stdlib.h>

int main(void) {
	char *str = NULL;
	size_t n = 0;
	while (getline(&str, &n, stdin) != -1) {
		printf("%s", str);
	}
	free(str);
	return 0;
}
