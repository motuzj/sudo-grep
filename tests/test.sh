#!/usr/bin/env bash
TESTS_DIR="$(dirname -- "${BASH_SOURCE[0]}")"
SUDO_GREP=$(realpath $TESTS_DIR/../sudo-grep) 
ls=$(ls -1 $TESTS_DIR/*.in 2> /dev/null | wc -l)

if [ ! -f $SUDO_GREP ]; then
    echo "Error: sudo-grep not found at \"$SUDO_GREP\""
    exit 1
fi

if [ $ls == 0 ]; then
    echo "Error: no input files were found!"
    exit 1
fi

for ((i=1; i<=$ls; i++))
do
    echo -en "\nRunning test $i. $(head -1 $TESTS_DIR/$i.in)...\t"
    cat $TESTS_DIR/text.txt | $SUDO_GREP $(sed -n '3p' $TESTS_DIR/$i.in) "$(sed -n '2p' $TESTS_DIR/$i.in)" | diff -bq $TESTS_DIR/$i.out - && echo -e "\e[32m✓ OK\e[0m" || echo -e "\e[31m✕ ERROR\e[0m"
done
