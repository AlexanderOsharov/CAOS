#!/bin/bash

# Функция обработки чисел
process_numbers() {
    local limit=$1
    local count=0
    local sum=0
    
    while [ $count -lt $limit ]; do
        if [ $((count % 2)) -eq 0 ]; then
            echo "Четное число: $count"
            sum=$((sum + count))
        else
            echo "Нечетное число: $count"
        fi
        ((count++))
    done
    
    echo "Сумма четных чисел: $sum"
    return $sum
}

# Функция анализа файлов
analyze_files() {
    local dir_path=$1
    local file_count=0
    
    if [ -d "$dir_path" ]; then
        for file in "$dir_path"/*; do
            if [ -f "$file" ]; then
                ((file_count++))
                echo "Файл $file_count: $(basename "$file")"
            fi
        done
    else
        echo "Директория $dir_path не существует"
    fi
    
    echo "Всего файлов: $file_count"
}

# Основная функция
main() {
    echo "=== Обработка чисел ==="
    process_numbers 10
    
    echo -e "\n=== Анализ файлов ==="
    analyze_files "."
}

# Вызов основной функции
main