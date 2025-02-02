#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define MSG_KEY 1234
#define MESSAGE_SIZE 100

struct msgbuf {
    long mtype;
    char mtext[MESSAGE_SIZE];
};

void sender(int msgid) {
    struct msgbuf message;
    struct timespec start, end;

    // Замер времени начала отправки
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < 1000; i++) {
        message.mtype = 1;
        snprintf(message.mtext, sizeof(message.mtext), "Message number %d", i);
        if (msgsnd(msgid, &message, sizeof(message.mtext), 0) == -1) {
            perror("msgsnd failed");
            exit(1);
        }
    }

    // Замер времени окончания отправки
    clock_gettime(CLOCK_MONOTONIC, &end);
    long time_taken = (end.tv_sec - start.tv_sec) * 1000000000L + (end.tv_nsec - start.tv_nsec);

    printf("System V Message: Time taken to send 1000 messages: %ld nanoseconds\n", time_taken);
}

void receiver(int msgid) {
    struct msgbuf message;
    struct timespec start, end;

    // Замер времени начала получения
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < 1000; i++) {
        if (msgrcv(msgid, &message, sizeof(message.mtext), 1, 0) == -1) {
            perror("msgrcv failed");
            exit(1);
        }
    }

    // Замер времени окончания получения
    clock_gettime(CLOCK_MONOTONIC, &end);
    long time_taken = (end.tv_sec - start.tv_sec) * 1000000000L + (end.tv_nsec - start.tv_nsec);

    printf("System V Message: Time taken to receive 1000 messages: %ld nanoseconds\n", time_taken);

    // Удаляем очередь сообщений
    if (msgctl(msgid, IPC_RMID, NULL) == -1) {
        perror("msgctl failed");
        exit(1);
    }
}

int main() {
    int msgid;

    // Создаем очередь сообщений
    msgid = msgget(MSG_KEY, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget failed");
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
        receiver(msgid);
    } else {
        // Родительский процесс (отправитель)
        sender(msgid);
    }

    return 0;
}
