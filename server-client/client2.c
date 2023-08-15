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
	remote.sin_addr.s_addr = inet_addr("127.0.0.1");
	remote.sin_family = AF_INET;
	remote.sin_port = htons(serverPort);

	iRetval = connect(hSocket, (struct sockaddr *) &remote, sizeof(struct sockaddr_in));

	return iRetval;
}

//recieve the date from the server
int socketRecieve(int hSocket, int *rsp, int rcvSize){
	int shortRetVal = -1;
	struct timeval tv;
	tv.tv_sec = 20;
	tv.tv_usec = 0;

	if(setsockopt(hSocket, SOL_SOCKET, SO_RCVTIMEO, (char *) &tv, sizeof(tv)) < 0){
		printf("Time out\n");
		return -1;
	}

	shortRetVal = recv(hSocket, rsp, rcvSize, 0);
	printf("Response %d\n", *rsp);

	return shortRetVal;
}


int main(void){
	int fdSocket = 0;
	int getFromServer = 0;

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

	// Recieved the data from the server
	if((socketRecieve(fdSocket, (int *) &getFromServer, sizeof(int))) < 0){
		perror("Error recieving message");
		return 1;
	}

	printf("Server Response : %d\n\n", getFromServer);

	close(fdSocket);
	return 0;
}
