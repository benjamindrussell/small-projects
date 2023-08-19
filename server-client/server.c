#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdio.h>
#include<string.h>

short socketCreate(void) {
	short hSocket = 0;
	printf("Create the socket\n");
	hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	return hSocket;
}

int bindCreatedSocket(int hSocket) {
	int iRetval = -1;
	int clientPort = 12345;

	struct sockaddr_in remote = {0};

	// Internet Address family (IPv4)
	remote.sin_family = AF_INET;

	// Any incoming interface
	remote.sin_addr.s_addr = inet_addr("127.0.0.1"); 
	remote.sin_port = htons(clientPort); // local port
	
	iRetval = bind(hSocket, (struct sockaddr *) &remote, sizeof(remote));
	
	return iRetval;
}

int main(void){
	int fdSocket = 0;  
	int fdConnection= 0;// also a file descriptor 
	int clientLen = 0;

	int clientNum = 0;
	int serverNum = 0;

	int clientPosition = 1;

	struct sockaddr_in client;

	//create socket
	fdSocket = socketCreate();
	if(fdSocket == -1){
		printf("Could not create socket");
		return 1;
	}

	printf("Socket created\n");

	//bind
	if(bindCreatedSocket(fdSocket) < 0) {
		perror("bind failed. ");
		return 1;
	}

	printf("bind done\n");

	//Listen
	listen(fdSocket, 3);

	while(1){
		printf("Waiting for incoming connections...\n");
		clientLen = sizeof(struct sockaddr_in);

		// Accept
		fdConnection = accept(fdSocket, (struct sockaddr *) &client, (socklen_t *) &clientLen);

		if(fdConnection < 0) {
			perror("accept failed");
			return 1;
		}

		printf("Connection accepted\n");

		if(clientPosition == 1){
			clientNum = serverNum = 0;

			if( recv(fdConnection, (int *) &clientNum, sizeof(int), 0) < 0){
				printf("recv failed");
				return 1;
			}

			printf("Client number: %d\n", clientNum);
			close(fdConnection);
			clientPosition++;
		} else if(clientPosition > 1){
			serverNum = clientNum - 1;

			if(send(fdConnection, (int *) &serverNum, sizeof(int), 0) < 0) {
				printf("Send failed");
				return 1;
			}

			close(fdConnection);
			clientPosition = 1;
		}
	}

	return 0;
}