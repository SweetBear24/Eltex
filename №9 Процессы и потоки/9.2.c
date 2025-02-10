#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 3  // Количество потоков

// Структура для передачи данных в поток
typedef struct {
    int start;
    int end;
    long long result;  // Переменная для хранения результата
} ThreadData;

// Функция, выполняемая потоком
void* calculate_sum_of_squares(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    long long sum = 0;

    for (int i = data->start; i <= data->end; i++) {
        sum += (long long)i * i;
    }

    data->result = sum;
    pthread_exit(NULL);
}

int main() {
    int N = 1000;  // Верхняя граница чисел
    pthread_t threads[NUM_THREADS];
    ThreadData threadData[NUM_THREADS];

    // Разделение диапазона на поддиапазоны
    int step = N / NUM_THREADS;
    for (int i = 0; i < NUM_THREADS; i++) {
        threadData[i].start = i * step + 1;
        threadData[i].end = (i == NUM_THREADS - 1) ? N : (i + 1) * step;
        threadData[i].result = 0;

        // Создание потока
        if (pthread_create(&threads[i], NULL, calculate_sum_of_squares, &threadData[i]) != 0) {
            perror("Ошибка при создании потока");
            exit(1);
        }
    }

    // Ожидание завершения потоков и суммирование результатов
    long long totalSum = 0;
    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("Ошибка при ожидании завершения потока");
            exit(1);
        }
        totalSum += threadData[i].result;
    }

    // Вывод итогового результата
    printf("Сумма квадратов чисел от 1 до %d: %lld\n", N, totalSum);

    return 0;
}
