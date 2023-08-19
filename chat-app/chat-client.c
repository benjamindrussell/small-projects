#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdio.h>
#include<string.h>

int main(void){
	int fdSocket = 0;
	struct sockaddr_in client = {0};
	struct sockaddr_in remote = {0};

	char messageToSend[140];
	char messageToRecieve[200];

	// Create socket
	fdSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if(fdSocket < 0){
		perror("Socket failed");
		return 1;
	}

	printf("Socket created\n");

	remote.sin_addr.s_addr = inet_addr("127.0.0.1");
	remote.sin_family = AF_INET;
	remote.sin_port = htons(12345);

	// Connect
	if(connect(fdSocket, (struct sockaddr *) &remote, sizeof(remote)) < 0){
		perror("Connection failed");
		return 1;
	}

	printf("Connection successful\n");

	printf("Enter your message: ");
	scanf("%[^\n]", messageToSend);

	// Write
	if(send(fdSocket, (char *) messageToSend, sizeof(messageToSend), 0) < 0){
		perror("Send failed");
		return 1;
	}

	printf("Message sent\n");

	// Read
	if(recv(fdSocket, (char *) messageToRecieve, sizeof(messageToRecieve), 0) < 0){
		perror("Recieve failed");
		return 1;
	}

	printf("Response: %s\n", messageToRecieve);

	close(fdSocket);
	return(0);
}
