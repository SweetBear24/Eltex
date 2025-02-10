#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[]) {
    // Проверяем количество аргументов
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <filename> <string>\n", argv[0]);
        return 1;
    }

    // Извлекаем аргументы
    const char *filename = argv[1];
    const char *data = argv[2];

    // Открываем файл для записи, добавляем данные в конец файла
    int fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    // Пишем данные в файл
    ssize_t bytesWritten = write(fd, data, strlen(data));
    if (bytesWritten == -1) {
        perror("write");
        close(fd);
        return 1;
    }

    // Закрываем файл
    if (close(fd) == -1) {
        perror("close");
        return 1;
    }

    // Открываем файл для чтения
    fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    // Буфер для считывания данных
    char buffer[128];
    ssize_t bytesRead;

    // Чтение данных из файла
    while ((bytesRead = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytesRead] = '\0'; // Завершаем строку нулевым символом
        printf("%s", buffer);    // Выводим считанные данные
        printf("\n");
    }
    printf("\n");

    if (bytesRead == -1) {
        perror("read");
    }

    // Закрываем файл
    if (close(fd) == -1) {
        perror("close");
        return 1;
    }

    return 0;
}
