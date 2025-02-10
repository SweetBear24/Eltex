#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>

#define FIFO_PATH "/tmp/myfifo" // Path to the named pipe

void writer_process() {
   int fd;
   char message[] = "Hello, named pipe!";
   struct timespec start, end;

   // Открываем именованный канал на запись
   fd = open(FIFO_PATH, O_WRONLY);
   if (fd == -1) {
      perror("open");
      exit(EXIT_FAILURE);
   }

   // Засекаем время начала отправки сообщений
   clock_gettime(CLOCK_MONOTONIC, &start);

   for (int i = 0; i < 1000; i++) {
      write(fd, message, strlen(message) + 1);
   }

   // Засекаем время окончания отправки сообщений
   clock_gettime(CLOCK_MONOTONIC, &end);

   // Вычисляем время работы
   double elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
   printf("Writer time: %.6f seconds\n", elapsed_time);

   // Закрываем именованный канал
   close(fd);
}

void reader_process() {
   int fd;
   char buffer[100];
   struct timespec start, end;

   // Открываем именованный канал на чтение
   fd = open(FIFO_PATH, O_RDONLY);
   if (fd == -1) {
      perror("open");
      exit(EXIT_FAILURE);
   }

   // Засекаем время начала чтения сообщений
   clock_gettime(CLOCK_MONOTONIC, &start);

   for (int i = 0; i < 1000; i++) {
      read(fd, buffer, sizeof(buffer));
   }

   // Засекаем время окончания чтения сообщений
   clock_gettime(CLOCK_MONOTONIC, &end);

   // Вычисляем время работы
   double elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
   printf("Reader time: %.6f seconds\n", elapsed_time);

   // Закрываем именованный канал
   close(fd);
}

int main() {
   pid_t pid;

   // Создаем именованный канал
   mkfifo(FIFO_PATH, 0666);

   // Создаем дочерний процесс
   pid = fork();

   if (pid == -1) {
      perror("fork");
      exit(EXIT_FAILURE);
   } else if (pid == 0) {
      // Дочерний процесс (отправитель)
      writer_process();
   } else {
      // Родительский процесс (получатель)
      reader_process();

      // Ждем завершения дочернего процесса
      wait(NULL);

      // Удаляем именованный канал
      unlink(FIFO_PATH);
   }
   return 0;
}
