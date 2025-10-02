#!/bin/bash
echo "------------------------------------------ Start testing ------------------------------------------"
echo " "

echo "Response to unrecognized option:"
./my_grep -t test/test_1.txt
echo " "

echo "Response to file opening error:"
./my_grep -e pattern nofile.txt

pattern=int

for options in -e -i -c -l -n -v -s -o -h -cn -cnl -nl -nvc -lnvc -lnsvh -hsnc -hn -hc -lh -iv -in -onc -oncl -ov
do
    test_number=1
    echo " "
    echo "Options $options:"
    for test_file in  tests/*.txt
    do
        ./my_grep $options $pattern $test_file > s21_grep_single.out
        grep $options $pattern $test_file > grep_single.out

        ./my_grep $options $pattern $test_file tests/test_2.txt > s21_grep_double.out
        grep $options $pattern $test_file tests/test_2.txt > grep_double.out

        if cmp -s s21_grep_single.out grep_single.out && cmp -s s21_grep_double.out grep_double.out; then
            echo "Test $test_number: SUCCESS"
        else
            echo "Test $test_number: FAIL"
        fi
        
        rm s21_grep_single.out grep_single.out s21_grep_double.out grep_double.out
        test_number=$(( $test_number + 1 ))
    done
done
echo " "
echo "------------------------------------------ Finish testing ------------------------------------------"