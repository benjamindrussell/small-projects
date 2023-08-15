#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>

// Create socket for server communication
short socketCreate(void){
	short hSocket;
	printf("Create the socket\n");
	hSocket = socket(AF_INET, SOCK_STREAM, 0);
	return hSocket;
}

// Try to connect with server
int socketConnect(int hSocket){
	int iRetval = -1;
	int serverPort = 12345;

	struct sockaddr_in remote = {0};
	remote.sin_addr.s_addr = inet_addr("192.168.1.124");
	remote.sin_family = AF_INET;
	remote.sin_port = htons(serverPort);

	iRetval = connect(hSocket, (struct sockaddr *) &remote, sizeof(struct sockaddr_in));

	return iRetval;
}

// Send the data to the server and set the timeout of 20 seconds
int socketSend(int hSocket, int *rqst, int lenRqst) {
	int shortRetVal = -1;
	struct timeval tv;
	tv.tv_sec = 20;
	tv.tv_usec = 0;

	if(setsockopt(hSocket, SOL_SOCKET, SO_SNDTIMEO, (char *) &tv, sizeof(tv)) < 0){
		printf("Time out\n");
		return -1;
	}

	shortRetVal = send(hSocket, rqst, lenRqst, 0);
	return shortRetVal;
}

int main(void){
	int fdSocket = 0;
	int sendToServer = 0;

	//Create socket
	fdSocket = socketCreate();

	if(fdSocket == -1){
		printf("Could not create socket\n");
		return 1;
	}

	printf("Socket is created\n");

	//Connect to remote server
	if(socketConnect(fdSocket) < 0){
		perror("connect failed.\n");
		return 1;
	}

	printf("Successfully connected with server\n");
	printf("Enter the Message: ");
	scanf("%d", &sendToServer);

	// Send data to the server
	if((socketSend(fdSocket, &sendToServer, sizeof(int))) < 0){
		perror("Send failed");
		return -1;
	}

	close(fdSocket);
	return 0;
}
