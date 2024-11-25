#!/bin/bash

make all

echo "Запуск тестов..."

# Тестируем каждый флаг
for flag in "-b" "-e" "-n" "-s" "-t"; do
    echo "Тестируем флаг: $flag"
    for test_file in ./test_cat/*.txt; do
        # Генерация выходных файлов
        ./s21_cat $flag $test_file > test_output.txt
        cat $flag $test_file > expected_output.txt

        # Сравниваем результаты
        if cmp -s test_output.txt expected_output.txt; then
            echo "Тест $test_file с $flag: Успех"
        else
            echo "Тест $test_file с $flag: Провал"
        fi

        # Удаление временных файлов
        rm -f test_output.txt expected_output.txt
    done
done

echo "Тесты завершены."