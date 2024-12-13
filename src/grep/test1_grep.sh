#!/bin/bash

make all
if [ $? -ne 0 ]; then
  echo "Ошибка компиляции!"
  exit 1
fi

template="int"

for flag in e i v c l n h s o; do
    echo "Тестирование флага -$flag"
    test_number=0
    for test_file in ./tests_grep/test_*.txt; do
        # Запуск программы с флагом и проверка результата
        ./s21_grep -$flag $template $test_file > 1.txt
        grep -$flag $template $test_file > 2.txt

        # Сравниваем результаты работы нашей программы с оригинальной
        if ! cmp -s 1.txt 2.txt; then
            echo "Тест $test_number: Неудача"
        else
            echo "Тест $test_number: Успех"
        fi

        rm -f 1.txt 2.txt
        test_number=$((test_number + 1))
    done
done
