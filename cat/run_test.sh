#!/bin/bash
echo "------------------------------------------ Start testing ------------------------------------------"
echo " "

echo "Response to unrecognized option:"
./s21_cat -q tests/test_1.txt
echo " "

echo "Response to file opening error:"
./s21_cat -b nofile.txt

for options in -b -e -n -s -t -v -ben -nst -vtb -ens -benstv
do
    test_number=1
    echo " "
    echo "Options $options:"
    for test_file in  tests/*.txt
    do
        ./s21_cat $options $test_file > s21_cat.out
        cat $options $test_file > cat.out

        if cmp -s s21_cat.out cat.out; then
            echo "Test $test_number: SUCCESS"
        else
            echo "Test $test_number: FAIL"
        fi
        
        rm s21_cat.out cat.out
        test_number=$(( $test_number + 1 ))
    done
done
echo " "
echo "------------------------------------------ Finish testing ------------------------------------------"