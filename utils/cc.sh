#! /bin/bash

if [[ $# -lt 2 ]]; then
    echo "Usage: cc.sh {AOS | SOA} TEST.c"
    exit
fi

wd=$(pwd)

test_file=$2
file_name=$(echo $test_file | grep -o "[^/]*\.c" | cut -d '.' -f 1)

echo "Compiling:" $file_name "-> a.out"

# Compile 
gcc -Wall -Wpedantic -g -o a.out $test_file ../board/$1/*.c ../data/railroad_data.c ../railroad_expansions.c

