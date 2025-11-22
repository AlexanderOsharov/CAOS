#define _POSIX_C_SOURCE 200809L
/*
 * syscopy.c
 * Копирование файлов с использованием только системных вызовов.
 * Поддержка:
 *   - копирование с большим буфером (по умолчанию: 64 КБ)
 *   - опционально: буфер 32 байта (флаг -s)
 *   - сохранение прав доступа (включая исполняемость)
 *
 * Компиляция:
 *   gcc -std=c11 -Wall -Wextra -O2 syscopy.c -o syscopy
 *
 * Использование:
 *   ./syscopy [-s] <input_file> <output_file>
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

#define DEFAULT_BUFFER_SIZE (64 * 1024)  // 64 КБ
#define SMALL_BUFFER_SIZE 32

void usage(const char *progname) {
    fprintf(stderr, "Использование: %s [-s] <исходный_файл> <целевой_файл>\n", progname);
    fprintf(stderr, "  -s: использовать маленький буфер (32 байта)\n");
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    int opt;
    int use_small_buffer = 0;

    // Обработка опционального флага -s
    while ((opt = getopt(argc, argv, "s")) != -1) {
        switch (opt) {
            case 's':
                use_small_buffer = 1;
                break;
            default:
                usage(argv[0]);
        }
    }

    if (optind + 2 != argc) {
        usage(argv[0]);
    }

    const char *input_filename = argv[optind];
    const char *output_filename = argv[optind + 1];

    // Открываем исходный файл только для чтения
    int in_fd = open(input_filename, O_RDONLY);
    if (in_fd == -1) {
        fprintf(stderr, "Ошибка открытия файла '%s': %s\n", input_filename, strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Получаем атрибуты исходного файла (для копирования прав)
    struct stat file_stat;
    if (fstat(in_fd, &file_stat) == -1) {
        perror("fstat");
        close(in_fd);
        exit(EXIT_FAILURE);
    }

    // Создаём/перезаписываем целевой файл с начальными правами 0666 (маска umask применится)
    int out_fd = open(output_filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (out_fd == -1) {
        fprintf(stderr, "Ошибка создания файла '%s': %s\n", output_filename, strerror(errno));
        close(in_fd);
        exit(EXIT_FAILURE);
    }

    // Выбираем размер буфера
    size_t buffer_size = use_small_buffer ? SMALL_BUFFER_SIZE : DEFAULT_BUFFER_SIZE;

    // Выделяем буфер
    char *buffer = malloc(buffer_size);
    if (!buffer) {
        perror("malloc");
        close(in_fd);
        close(out_fd);
        exit(EXIT_FAILURE);
    }

    ssize_t bytes_read;
    while ((bytes_read = read(in_fd, buffer, buffer_size)) > 0) {
        ssize_t written = 0;
        while (written < bytes_read) {
            ssize_t result = write(out_fd, buffer + written, bytes_read - written);
            if (result == -1) {
                perror("write");
                free(buffer);
                close(in_fd);
                close(out_fd);
                exit(EXIT_FAILURE);
            }
            written += result;
        }
    }

    if (bytes_read == -1) {
        perror("read");
        free(buffer);
        close(in_fd);
        close(out_fd);
        exit(EXIT_FAILURE);
    }

    free(buffer);
    close(in_fd);
    close(out_fd);  // Закрываем ДО chmod, чтобы избежать проблем на некоторых ФС

    // Теперь безопасно применяем права по имени файла
    if (chmod(output_filename, file_stat.st_mode) == -1) {
        perror("chmod");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}