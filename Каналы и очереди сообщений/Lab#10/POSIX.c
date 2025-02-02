#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define MQ_NAME "/my_queue"
#define MESSAGE_SIZE 100

void sender(mqd_t mq) {
    char message[MESSAGE_SIZE];
    struct timespec start, end;

    // Замер времени начала отправки
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < 1000; i++) {
        snprintf(message, sizeof(message), "Message number %d", i);
        if (mq_send(mq, message, strlen(message) + 1, 0) == -1) {
            perror("mq_send failed");
            exit(1);
        }
    }

    // Замер времени окончания отправки
    clock_gettime(CLOCK_MONOTONIC, &end);
    long time_taken = (end.tv_sec - start.tv_sec) * 1000000000L + (end.tv_nsec - start.tv_nsec);

    printf("POSIX Message Queue: Time taken to send 1000 messages: %ld nanoseconds\n", time_taken);
}

void receiver(mqd_t mq) {
    char message[MESSAGE_SIZE];
    struct timespec start, end;

    // Замер времени начала получения
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < 1000; i++) {
        ssize_t bytes_received = mq_receive(mq, message, sizeof(message), NULL);
        if (bytes_received == -1) {
            perror("mq_receive failed");
            exit(1);
        }
    }

    // Замер времени окончания получения
    clock_gettime(CLOCK_MONOTONIC, &end);
    long time_taken = (end.tv_sec - start.tv_sec) * 1000000000L + (end.tv_nsec - start.tv_nsec);

    printf("POSIX Message Queue: Time taken to receive 1000 messages: %ld nanoseconds\n", time_taken);
}

int main() {
    mqd_t mq;
    struct mq_attr attr;

    // Устанавливаем атрибуты очереди сообщений
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MESSAGE_SIZE;
    attr.mq_curmsgs = 0;

    // Создаем очередь сообщений
    mq = mq_open(MQ_NAME, O_CREAT | O_RDWR, 0666, &attr);
    if (mq == (mqd_t)-1) {
        perror("mq_open failed");
        exit(1);
    }

    pid_t pid = fork();

    if (pid == -1) {
        // Ошибка при создании процесса
        perror("fork failed");
        exit(1);
    }

    if (pid == 0) {
        // Дочерний процесс (получатель)
        receiver(mq);
    } else {
        // Родительский процесс (отправитель)
        sender(mq);
    }

    // Закрываем очередь сообщений
    mq_close(mq);

    // Удаляем очередь сообщений
    mq_unlink(MQ_NAME);

    return 0;
}
