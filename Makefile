NAME=sudo-grep

main:
	gcc -o $(NAME) ./src/*.c

test:
	./tests/test.sh
