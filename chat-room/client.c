#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define BUFFER_SIZE 1024

int client_socket;
char username[100];

void *receive_messages(void *arg) {
    char buffer[BUFFER_SIZE];

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (bytes_received <= 0) {
            printf("Server disconnected.\n");
            break;
        }

        printf("\n> %s", buffer);
    }

    close(client_socket);
    exit(0);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <server_ip> <port>\n", argv[0]);
        exit(1);
    }

    char *server_ip = argv[1];
    int server_port = atoi(argv[2]);

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("socket");
        exit(1);
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(server_ip);
    server_addr.sin_port = htons(server_port);

    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        exit(1);
    }

	char welcome[128];
	if(recv(client_socket, welcome, sizeof(welcome), 0) < 0){
		perror("recieve");
		exit(0);
	}
	printf("%s\n", welcome);

    printf("Enter your username: ");
    fgets(username, sizeof(username), stdin);
    username[strlen(username) - 1] = '\0'; // Remove the newline character

    pthread_t thread;
    pthread_create(&thread, NULL, receive_messages, NULL);

    char message[BUFFER_SIZE];
    while (1) {
		printf("> ");
        fgets(message, sizeof(message), stdin);
        if (strlen(message) <= 1) {
            continue; // Skip empty messages
        }

        // Prepare the message with the username
        char formatted_message[BUFFER_SIZE];
        snprintf(formatted_message, sizeof(formatted_message), "%s: %s", username, message);

        if (send(client_socket, formatted_message, strlen(formatted_message), 0) == -1) {
            perror("send");
            break;
        }
    }

    close(client_socket);
    return 0;
}
