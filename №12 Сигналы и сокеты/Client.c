#include <arpa/inet.h> 
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <errno.h>

#define MAX 80
#define PORT 8080
#define SA struct sockaddr

void func(int sockfd) {
    char buff[MAX];
    int n;
    ssize_t bytes_written, bytes_read;

    for (;;) {
        // Очистка буфера
        bzero(buff, sizeof(buff));
        printf("Enter the string: ");
        
        n = 0;
        while ((buff[n++] = getchar()) != '\n' && n < MAX - 1);
        buff[n] = '\0'; // Добавляем терминатор строки

        // Отправляем только реальную длину строки
        bytes_written = write(sockfd, buff, strlen(buff));
        if (bytes_written <= 0) {
            perror("Write to server failed");
            break;
        }

        // Очистка буфера перед чтением
        bzero(buff, sizeof(buff));

        // Читаем ответ сервера
        bytes_read = read(sockfd, buff, sizeof(buff) - 1);
        if (bytes_read <= 0) {
            if (bytes_read == 0)
                printf("Server disconnected\n");
            else
                perror("Read from server failed");
            break;
        }

        buff[bytes_read] = '\0';
        printf("From Server: %s", buff);

        // Если получено "exit", выходим
        if (strncmp(buff, "exit", 4) == 0) {
            printf("Client Exit...\n");
            break;
        }
    }
}

int main() {
    int sockfd;
    struct sockaddr_in servaddr;

    // Создание сокета
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    printf("Socket successfully created.\n");

    // Заполняем структуру сервера
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    // Подключаемся к серверу
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        perror("Connection with the server failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("Connected to the server.\n");

    // Вызов функции обмена сообщениями
    func(sockfd);

    // Закрываем соединение
    close(sockfd);
    return 0;
}
