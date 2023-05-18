#include <stdio.h>
#include <unistd.h>

int main(void) {
	char str[10000];
	read(STDIN_FILENO, str, 10000);
	printf("%s", str);
	return 0;
}
