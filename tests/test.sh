#!/usr/bin/env bash
SUDO_GREP=$(realpath ./sudo-grep)
ls=$(ls -1 tests/*.in 2> /dev/null | wc -l)

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
    echo -en "\nRunning test $i. $(head -1 tests/$i.in)...\t"
    eval args=("$(sed -n '2p' tests/$i.in)")

    cat tests/texts/text1.txt | 
    $SUDO_GREP "${args[@]}" | 
    diff -bq tests/$i.out - && 
    echo -e "\e[32m✓ OK\e[0m" || 
    echo -e "\e[31m✕ ERROR\e[0m"
done
