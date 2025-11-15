#!/bin/bash

# Функция для вывода сообщения
display_message() {
    local msg=$1
    echo "[INFO] $msg"
}

# Функция для выполнения вычислений
perform_calculation() {
    local a=$1
    local b=$2
    
    if [ $a -gt $b ]; then
        echo "Разница: $((a - b))"
    else
        echo "Сумма: $((a + b))"
    fi
}

# Функция для проверки четности
is_even() {
    local num=$1
    if [ $((num % 2)) -eq 0 ]; then
        return 0
    else
        return 1
    fi
}