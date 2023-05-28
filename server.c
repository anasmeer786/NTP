#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/time.h>

#define MAX_CLIENTS 10

pthread_mutex_t mutex;

int client_socket[MAX_CLIENTS];
int client_count = 0;

void *handle_client(void *arg) {
    int client_fd = *((int *)arg);

    while (1) {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        char *timestamp = (char *)malloc(sizeof(char) * 20);
        sprintf(timestamp, "%ld.%06ld", tv.tv_sec, (long int) tv.tv_usec);


        write(client_fd, timestamp, strlen(timestamp));
        sleep(1);
    }

    return NULL;
}

int main(int argc, char const *argv[]) {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    pthread_t tid[MAX_CLIENTS];

    if (pthread_mutex_init(&mutex, NULL) != 0) {
        printf("Mutex init failed!\n");
        exit(1);
    }

    // create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        printf("Socket creation failed!\n");
        exit(1);
    }

    // set server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8000);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // bind socket to address
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        printf("Socket bind failed!\n");
        exit(1);
    }

    // listen for clients
    if (listen(server_fd, MAX_CLIENTS) == -1) {
        printf("Listen failed!\n");
        exit(1);
    }

    printf("Server started! Listening on port 8000\n");

    while (1) {
        socklen_t client_len = sizeof(client_addr);

        // accept client connection
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);

        pthread_mutex_lock(&mutex);

        if (client_count < MAX_CLIENTS) {
            client_socket[client_count++] = client_fd;

            pthread_create(&tid[client_count - 1], NULL, handle_client, (void *)&client_socket[client_count - 1]);

            printf("Client connected! (%s:%d)\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        } else {
            printf("Client refused! (Max clients reached)\n");
            close(client_fd);
        }

        pthread_mutex_unlock(&mutex);
    }

    return 0;
}

