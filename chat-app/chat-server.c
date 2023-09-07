#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdio.h>
#include<string.h>


int main(void){
	int isMessage = 0;
	int fdSocket = 0 ;
	int fdConnect = 0;
	socklen_t addrLen;

	struct sockaddr_in server = {0};
	struct sockaddr_in client = {0};

	char clientMessage[140] = {0};
	// char clientMessage2[140] = {0};

	char serverMessage[200] = {0};

	// char clientAddress1[50] = {0};
	// char clientAddress2[50] = {0};

	// Create socket
	fdSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if(fdSocket < 0){
		perror("Could not create socket");
		return 1;
	}

	printf("Socket created\n");

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");	
	server.sin_port = htons(12345);

	// Bind
	if (bind(fdSocket, (struct sockaddr*) &server, sizeof(server)) < 0 ){
		perror("Bind failed");
		return 1;
	}

	printf("Socket bound\n");

	// Listen
	if(listen(fdSocket, 2) < 0){
		perror("Listen failed");
		return 1;
	}

	while(1){
		// Write
		
		// int tempAddress[50] = {0}; 

		printf("Waiting for incoming connections...\n");

		addrLen = sizeof(struct sockaddr_in);

		// Accept
		fdConnect = accept(fdSocket, (struct sockaddr *) &client, (socklen_t *) &addrLen);
		/* inet_ntop(AF_INET, &client, tempAddress, 50);

		if(strncmp(tempAddress, clientAddress1, 50) == 0){
			strncpy(clientAddress1, tempAddress, 50);
		} else if(strncmp(tempAddress, clientAddress2, 50) == 0){
			strncpy(clientAddress2, tempAddress, 50);
		} */

		if(fdConnect < 0){
			perror("Accept failed");
			return 1;
		}

		printf("Connection accepted\n");

		if(send(fdConnect, (int *) &isMessage, sizeof(int), 0) < 0){
			perror("Send failed");
			return 1;	
		}
		printf("Signal sent\n");

		if(isMessage){
			if(send(fdConnect, (char *) clientMessage, sizeof(clientMessage), 0) < 0){
				perror("Send failed");
				return 1;

			}
			printf("Message sent\n");
		}

		memset(clientMessage, '\0', sizeof(clientMessage));
		memset(serverMessage, '\0', sizeof(serverMessage));

		// Read
		if(recv(fdConnect, (char *) clientMessage, sizeof(clientMessage), 0) < 0){
			perror("Recv failed");
			return 1;
		}

		printf("Message recieved: %s\n\n", clientMessage);
		sprintf(serverMessage, "Client message : %s", clientMessage);


		// Close
		close(fdConnect);
		isMessage++;
	}
	
}
