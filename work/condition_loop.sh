#!/bin/bash

# Функция проверки числа
check_number() {
    local num=$1
    if [ $num -gt 10 ]; then
        echo "Число $num больше 10"
    elif [ $num -lt 10 ]; then
        echo "Число $num меньше 10"
    else
        echo "Число $num равно 10"
    fi
}

# Цикл while для подсчета
counter=1
while [ $counter -le 5 ]; do
    check_number $counter
    ((counter++))
done