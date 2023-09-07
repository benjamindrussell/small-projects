#include<arpa/inet.h>
#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>

#define INET_ADDR "127.0.0.1"
#define MAX_CLIENTS 10
#define DEFAULT_PORT 4444

int listen_tcp_socket(int portnum);
void* handle_client(void* args);

int client_count = 0;
int client_sockets[MAX_CLIENTS];

int main(int argc, char** argv){
	int portnum = DEFAULT_PORT;

	//command line argument specifies port number
	if(argc > 1){
		portnum = atoi(argv[1]);
	}

	int sock_fd = listen_tcp_socket(portnum);

	while(1){
		struct sockaddr_in client_addr;
		socklen_t client_addr_len = sizeof(client_addr);

		client_sockets[client_count] = accept(sock_fd, (struct sockaddr*)&client_addr, &client_addr_len);

		if(client_sockets[client_count] < 0){
			perror("Accept failed");
			exit(0);
		}

		pthread_t client_thread;

		pthread_create(&client_thread, NULL, handle_client, &client_sockets[client_count]);
		client_count++;

		// pthread_detach(client_thread);
	}


	return 0;
}

int listen_tcp_socket(int portnum){

	//create socket
	int sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sock_fd < 0){
		perror("Socket Creation Failed");
		exit(0);
	}

	//bind to port
	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;	
	server_addr.sin_port = htons(portnum);

	if(bind(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
		perror("Binding Failed");
		exit(0);
	}	

	if (listen(sock_fd, MAX_CLIENTS) < 0) {
		perror("Listen Failed");
	}

	return sock_fd;
}

void* handle_client(void* args){
	int client_socket = *((int *)args);
	char buffer[1024];
	char welcome_message[] = "--------Welcome to the chat room--------";
	if(send(client_socket, (char *) welcome_message, sizeof(welcome_message), 0) < 0){
		perror("Send failed");
		pthread_exit(NULL);
	}
	
	while(1){
		memset(buffer, 0, sizeof(buffer));
		if(recv(client_socket, buffer, sizeof(buffer), 0) < 0){
			perror("Recieve Failed");
			break;
		}

		for(int i = 0; i < client_count; i++){
			if(client_sockets[i] != client_socket){
				if(send(client_sockets[i], buffer, sizeof(buffer), 0) < 0){
					perror("Send failed");
					pthread_exit(NULL);
				}
			}
		}
	}

	for(int i = 0; i < client_count; i++){
		if(client_sockets[i] == client_socket){
			for(int j = i; j < client_count - 1; j++){
				client_sockets[j] = client_sockets[j + 1];
			}
			client_sockets[client_count] = 0;
			client_count--;
			break;
		}
	}

	pthread_exit(NULL);
}

