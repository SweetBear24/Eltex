#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX 80

int server_fd, client_fd;

void sigio_handler(int signo) {
    char buffer[MAX];
    ssize_t bytes_read;
    
    while ((bytes_read = read(client_fd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes_read] = '\0';
        printf("Received: %s", buffer);
        write(client_fd, buffer, bytes_read); // Отправляем обратно
    }

    // Повторно включаем FASYNC, чтобы сигналы продолжали приходить
    fcntl(client_fd, F_SETFL, O_NONBLOCK | FASYNC);
}

int main() {
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    
    // Создаем сокет
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    // Устанавливаем параметры сервера
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);
    
    // Привязываем сокет
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
    
    // Слушаем входящие соединения
    if (listen(server_fd, 5) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
    printf("Server listening on port %d\n", PORT);
    
    // Принимаем соединение
    client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
    if (client_fd < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }
    printf("Client connected\n");
    
    // Устанавливаем неблокирующий режим и FASYNC
    fcntl(client_fd, F_SETFL, O_NONBLOCK | FASYNC);
    fcntl(client_fd, F_SETOWN, getpid()); // Устанавливаем владельца
    
    // Назначаем обработчик сигнала SIGIO
    signal(SIGIO, sigio_handler);
    
    // Бесконечный цикл ожидания событий (сервер не завершает работу)
    while (1) {
        pause(); // Ждем сигнала
    }
    
    close(client_fd);
    close(server_fd);
    return 0;
}
