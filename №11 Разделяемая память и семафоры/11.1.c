#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/types.h>

#define SHM_KEY 0x1234  // Ключ разделяемой памяти
#define SEM_KEY 0x5678  // Ключ для семафоров
#define BUFFER_SIZE 5   // Размер кольцевого буфера
#define MESSAGE_SIZE 64 // Размер одного сообщения
#define TOTAL_MESSAGES 6 // Количество сообщений, отправляемых производителем

// Семафоры
#define SEM_MUTEX 0   // Блокировка доступа к буферу
#define SEM_FULL 1    // Количество сообщений в буфере
#define SEM_EMPTY 2   // Количество свободных мест

// Структура кольцевого буфера в разделяемой памяти
typedef struct {
    char messages[BUFFER_SIZE][MESSAGE_SIZE]; // Буфер сообщений
    int head;  // Указатель на чтение
    int tail;  // Указатель на запись
} SharedBuffer;

// Функции работы с семафорами
void semaphore_wait(int sem_id, int sem_num) {
    struct sembuf op = {sem_num, -1, 0};
    semop(sem_id, &op, 1);
}

void semaphore_signal(int sem_id, int sem_num) {
    struct sembuf op = {sem_num, 1, 0};
    semop(sem_id, &op, 1);
}

// Функция процесса-поставщика
void producer(int shm_id, int sem_id) {
    SharedBuffer *buffer = (SharedBuffer *)shmat(shm_id, NULL, 0);
    if (buffer == (SharedBuffer *)-1) {
        perror("Ошибка подключения к памяти");
        exit(1);
    }

    for (int i = 1; i <= TOTAL_MESSAGES; i++) {
        semaphore_wait(sem_id, SEM_EMPTY);
        semaphore_wait(sem_id, SEM_MUTEX);

        snprintf(buffer->messages[buffer->tail], MESSAGE_SIZE, "Сообщение %d", i);
        buffer->tail = (buffer->tail + 1) % BUFFER_SIZE;

        printf("[Producer] Записано: Сообщение %d\n", i);

        semaphore_signal(sem_id, SEM_MUTEX);
        semaphore_signal(sem_id, SEM_FULL);

        sleep(1);
    }

    shmdt(buffer);
}

// Функция процесса-потребителя
void consumer(int shm_id, int sem_id, int consumer_id) {
    SharedBuffer *buffer = (SharedBuffer *)shmat(shm_id, NULL, 0);
    if (buffer == (SharedBuffer *)-1) {
        perror("Ошибка подключения к памяти");
        exit(1);
    }

    for (int i = 0; i < TOTAL_MESSAGES / 2; i++) { 
        semaphore_wait(sem_id, SEM_FULL);
        semaphore_wait(sem_id, SEM_MUTEX);

        printf("[Consumer %d] Прочитано: %s\n", consumer_id, buffer->messages[buffer->head]);
        buffer->head = (buffer->head + 1) % BUFFER_SIZE;

        semaphore_signal(sem_id, SEM_MUTEX);
        semaphore_signal(sem_id, SEM_EMPTY);

        sleep(1);
    }

    shmdt(buffer);
}

// Очистка разделяемой памяти и семафоров
void cleanup(int shm_id, int sem_id) {
    shmctl(shm_id, IPC_RMID, NULL);
    semctl(sem_id, 0, IPC_RMID);
    printf("Ресурсы освобождены.\n");
}

// Главная функция
int main() {
    // Создаем разделяемую память
    int shm_id = shmget(SHM_KEY, sizeof(SharedBuffer), IPC_CREAT | 0666);
    if (shm_id < 0) {
        perror("Ошибка создания разделяемой памяти");
        exit(1);
    }

    // Создаем семафоры
    int sem_id = semget(SEM_KEY, 3, IPC_CREAT | 0666);
    if (sem_id < 0) {
        perror("Ошибка создания семафоров");
        exit(1);
    }

    // Инициализация семафоров
    semctl(sem_id, SEM_MUTEX, SETVAL, 1);
    semctl(sem_id, SEM_FULL, SETVAL, 0);
    semctl(sem_id, SEM_EMPTY, SETVAL, BUFFER_SIZE);

    // Инициализация разделяемой памяти
    SharedBuffer *buffer = (SharedBuffer *)shmat(shm_id, NULL, 0);
    buffer->head = 0;
    buffer->tail = 0;
    shmdt(buffer);

    // Запуск процессов
    pid_t pid1 = fork();
    if (pid1 == 0) {
        producer(shm_id, sem_id);
        exit(0);
    }

    pid_t pid2 = fork();
    if (pid2 == 0) {
        consumer(shm_id, sem_id, 1);
        exit(0);
    }

    pid_t pid3 = fork();
    if (pid3 == 0) {
        consumer(shm_id, sem_id, 2);
        exit(0);
    }

    // Ожидание завершения процессов
    wait(NULL);
    wait(NULL);
    wait(NULL);

    // Очистка ресурсов
    cleanup(shm_id, sem_id);

    return 0;
}
