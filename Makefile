NAME=sudo-grep

main:
	gcc -o $(NAME) ./src/*.c

run_tests:
	./tests/test.sh
