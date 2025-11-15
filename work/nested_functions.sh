#!/bin/bash

# Внутренняя функция для вычисления
calculate() {
    local operation=$1
    local num1=$2
    local num2=$3
    local result
    
    case $operation in
        "add")
            result=$((num1 + num2))
            ;;
        "multiply")
            result=$((num1 * num2))
            ;;
        *)
            result=0
            ;;
    esac
    
    echo $result
}

# Внешняя функция, вызывающая внутреннюю
process_data() {
    echo "Вычисляем сумму 5 и 3:"
    local sum=$(calculate "add" 5 3)
    echo "Результат: $sum"
    
    echo "Вычисляем произведение 4 и 7:"
    local product=$(calculate "multiply" 4 7)
    echo "Результат: $product"
}

# Вызов основной функции
process_data