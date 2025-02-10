#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid;  // Переменная для хранения PID процесса

    // Создание дочернего процесса
    pid = fork();

    if (pid < 0) {
        // Ошибка при вызове fork()
        perror("Ошибка при создании дочернего процесса");
        exit(1);
    } else if (pid == 0) {
        // Код дочернего процесса
        printf("Дочерний процесс (PID: %d) запущен\n", getpid());

        // Подготовка аргументов для execvp()
        char *args[] = {"ls", "-l", NULL};

        // Замена образа текущего процесса на внешнюю программу
        if (execvp(args[0], args) == -1) {
            perror("Ошибка при выполнении execvp");
            exit(1);  // Выход из дочернего процесса в случае ошибки
        }
    } else {
        // Код родительского процесса
        int status;
        printf("Родительский процесс (PID: %d) ждёт завершения дочернего процесса\n", getpid());

        // Ожидание завершения дочернего процесса
        if (waitpid(pid, &status, 0) == -1) {
            perror("Ошибка при ожидании дочернего процесса");
            exit(1);
        }

        // Проверка кода завершения дочернего процесса
        if (WIFEXITED(status)) {
            printf("Дочерний процесс завершился с кодом: %d\n", WEXITSTATUS(status));
        } else {
            printf("Дочерний процесс завершился некорректно\n");
        }
    }

    return 0;
}
