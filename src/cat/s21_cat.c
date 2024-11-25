#include "s21_cat.h"

int main(int argc, char **argv) {
  opt options = {0};  // Инициализация структуры для хранения флагов

  if (argc > 1) {
    parse_flags(argc, argv, &options);
    FILE *f = NULL;

    // Обработка каждого файла, указанного в аргументах
    for (int i = 1; i < argc; ++i) {
      if (argv[i][0] != '-') {  // Пропускаем флаги
        f = fopen(argv[i], "r");
        if (f != NULL) {
          // Ставим указатель в начало файла и печатаем его
          fseek(f, 0, SEEK_SET);
          print_file(f, &options);
          fclose(f);
        } else {
          printf("s21_cat: %s: No such file\n", argv[i]);
        }
      }
    }
  } else {
    printf("usage: s21_cat [-bens] [file ...]\n");
  }

  return 0;
}

// Функция разбора флагов
void parse_flags(int argc, char **argv, opt *options) {
  int option_index;
  int opt_char;

  while ((opt_char = getopt_long(argc, argv, "+benstvTE", long_options,
                                 &option_index)) != -1) {
    switch (opt_char) {
      case 'b':
        options->b = 1;
        break;
      case 'e':
        options->e = 1;
        options->v = 1;
        break;
      case 'n':
        options->n = 1;
        break;
      case 's':
        options->s = 1;
        break;
      case 't':
        options->t = 1;
        options->v = 1;
        break;
      case 'v':
        options->v = 1;
        break;
      case 'T':
        options->t = 1;
        break;
      case 'E':
        options->e = 1;
        break;
      case '?':
        printf("Некорретный флаг!\n");
        exit(1);
    }
  }

  if (options->b) options->n = 0;
}

// Функция вывода содержимого файла с учётом флагов
void print_file(FILE *f, opt *options) {
  int number = 0, new_line = 1;  // Счётчик строк и флаг новой строки
  char pr_ch = 0, ch = 0;  // Предыдущий и текущий символ

  while ((ch = fgetc(f)) != EOF) {
    if (options->s) {
      if (pr_ch == '\n' && ch == '\n') {
        int skip_newlines = 0;
        // Пропускаем лишние пустые строки
        while ((ch = fgetc(f)) == '\n') {
          skip_newlines = 1;
        }
        if (skip_newlines) {
          if (options->n) printf("%6d\t", ++number);
          options->e ? printf("$\n") : printf("\n");
        }
        if (ch != EOF) ungetc(ch, f);  // Возвращаем символ, который не '\n'
        pr_ch = '\n';
        continue;
      }
    }

    // Обработка флага -n (нумерация всех строк)
    if (options->n && new_line) {
      printf("%6d\t", ++number);
      new_line = 0;
    }

    // Обработка флага -b (нумерация только непустых строк)
    if (options->b && new_line && ch != '\n') {
      printf("%6d\t", ++number);
      new_line = 0;
    }

    // Обработка флага -e (отображение символов конца строки)
    if (options->e && ch == '\n') {
      printf("$");
    }

    // Обработка флага -t (замена табуляций)
    if (options->t && ch == '\t') {
      printf("^I");
      continue;
    }

    // Обработка флага -v (печать управляющих символов)
    if (options->v) {
      if (ch <= 31 && ch != '\n' && ch != '\t') {
        printf("^%c", ch + 64);
        continue;
      } else if (ch == 127) {
        printf("^?");
        continue;
      }
    }

    // Вывод символа
    printf("%c", ch);

    // Проверка на новую строку
    if (ch == '\n') new_line = 1;

    pr_ch = ch;  // Обновляем предыдущий символ
  }
}