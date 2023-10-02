#! /bin/bash

file_name_chosen=false
compiler_chosen=false
flags_chosen=false
aos_chosen=false
soa_chosen=false

while getopts ':n:c:fASh' OPTION; do
    case "$OPTION" in
        n)
            file_name=$OPTARG
            file_name_chosen=true
            ;;
        c)
            compiler=$OPTARG
            compiler_chosen=true
            ;;
        f)
            flags=$OPTARG
            flags_chosen=true
            ;;
        AOS)
            echo "AOS"
            aos_chosen=true
            ;;
        SOA)
            echo "SOA"
            soa_chosen=true
            ;;
        h)
            echo "Usage: cc [-f file_name] [-c compiler] [-f compiler_flags] [-A] [-S] "
            echo ""
            echo "-n        The file name of the test file to be compiled"
            echo ""
            echo "-c        Which compiler to use, by default gcc is used."
            echo ""
            echo "-f        The flags which will be sent to the compiler, the default"
            echo "          flags are '-Wall -Wpedantic -fsanitize=address'"
            echo ""
            echo "-A        Denotes if the board should use an Array of Structs design"
            echo "          pattern, true by default"
            echo ""
            echo "-S        Denotes if the board should use an Struct of Arrays design"
            echo "          pattern, overrides the A flag."
            exit
            ;;
    esac
done

if [ "$file_name_chosen" = false ] ; then
    echo "No file chosen. See usage with -h"
    exit
fi
if [ "$flags_chosen" = false ] ; then
    flags="-Wall -Wpedantic -fsanitize=address"
fi
if [ "$compiler_chosen" = false ] ; then
    compiler=gcc
fi
if [ "$soa_chosen" = true ] ; then
    board_dir=SOA
else
    board_dir=AOS
fi
if [ "$aos_chosen" = true ] && [ "$aos_chosen" = true ] ; then
    echo "Cannot specify AOS and SOA at the same time."
    exit
fi


print_name=$(echo $file_name | grep -o "[^/]*\.c" | cut -d '.' -f 1)
echo "Compiling:" $print_name "-> a.out"

$compiler $flags -o a.out $file_name ../board/$board_dir/*.c ../data/railroad_data.c ../railroad_expansions.c

