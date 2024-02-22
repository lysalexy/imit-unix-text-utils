#! /bin/bash

options=(-b -e -v -n -s -T -E -t --number-nonblank --number --squeeze-blank)
testfiles=(test1.txt test2.txt)

for ((integer = 0; integer <= 2023; integer++)); do
    flags=""
    for ((index = 0; index <= 10; index++)); do
        if [ "$((${integer} & ((1 << ${index}))))" != "0" ]; then
            flags+=${options[index]}
            flags+=" "
        fi
    done
    echo ${flags}
    for ((file = 1; file <= 3; file++)); do
        files=""
        for ((fileindex = 0; fileindex <= 2; fileindex++)); do
            if [ "$((${file} & ((1 << ${fileindex}))))" != "0" ]; then
                files+=${testfiles[fileindex]}
                files+=" "
            fi
        done
        echo ${flags} ${files}
       # valgrind --tool=memcheck --leak-check=yes --error-exitcode=1 ./../s21_cat ${flags} ${files} 1>/dev/null 2>/dev/null || echo -e "\033[0;31mVALGRIND FAILED\033[0m ${flags} ${files}"
        #### если вывод следующей команды пустой, то passed, иначе-вывод отличий
        (cat ${flags} ${files}) | diff -u - <(./../s21_cat ${flags} ${files})
    done
done
