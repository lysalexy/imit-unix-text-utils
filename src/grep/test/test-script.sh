#! /bin/bash

options=("-e D" "-e s" "-f regex.txt" -s -i -v -c -l -n -h --invert-match --count --files-with-matches --line-number)
testfiles=(test1.txt test2.txt)

testsPassed=0

for ((integer = 0; integer <= 32767; integer++)); do
    flags="p "
    for ((index = 0; index <= 14; index++)); do
        if [ "$((${integer} & ((1 << ${index}))))" != "0" ]; then
            flags+=${options[index]}
            flags+=" "
        fi
    done
    ###echo ${flags}
    for ((file = 1; file <= 3; file++)); do
        files=""
        for ((fileindex = 0; fileindex <= 1; fileindex++)); do
            if [ "$((${file} & ((1 << ${fileindex}))))" != "0" ]; then
                files+=${testfiles[fileindex]}
                files+=" "
            fi
        done
        ###echo ${flags} ${files}
        valgrind --tool=memcheck --leak-check=yes --error-exitcode=1 ./../s21_grep ${flags} ${files} 1>/dev/null 2>/dev/null || echo -e "\033[0;31mVALGRIND FAILED\033[0m ${flags} ${files}"
        #### если вывод следующей команды пустой, то passed, иначе-вывод отличий
        ###(grep ${flags} ${files}) | diff -u - <(./../s21_grep ${flags} ${files}) &&  echo -e "\033[32mPASSED\033[0m ${flags} ${files}" ||  echo -e "\033[0;31mDIFFER FROM SYSTEM CALL\033[0m ${flags} ${files}"
        (grep ${flags} ${files}) 2>/dev/null | diff -u - <(./../imit_grep ${flags} ${files} 2>/dev/null) && echo -e "\033[32mPASSED\033[0m ${flags} ${files}" || echo -e "\033[0;31mDIFFER FROM SYSTEM CALL\033[0m ${flags} ${files}" || echo -e "\033[0;31mDIFFER FROM SYSTEM CALL\033[0m ${flags} ${files}"
        testsPassed+=1
    done
done
