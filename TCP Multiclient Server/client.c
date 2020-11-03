#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 4449

int main(){

	int clientSocket, ret;
	struct sockaddr_in serverAddr;
	char buffer[100];
	char userId[10];
    char inout[6];

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    
	if(clientSocket < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Client Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    
	if(ret < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Connected to Server.\n");
    bzero(buffer, sizeof(buffer));
    bzero(userId, sizeof(userId));

	while(1){
        
        printf("\n");
        printf("Enter your ID: \t");
        scanf("%[^\n]%*c", userId);
        
        if(strcmp(userId, "exit") == 0){
            send(clientSocket, userId, strlen(userId), 0);
            strcpy(buffer, "exit");
            send(clientSocket, buffer, strlen(buffer), 0);
            close(clientSocket);
            printf("[-]Disconnected from server.\n");
            exit(1);
        }
        printf("\n");
        printf("For 'CHECK IN' Enter -- 'IN' or 'in' \n");
        printf("For 'CHECK OUT' Enter -- 'OUT' or 'out' \n");
        printf("\n");
		printf("[FROM]Client: \t");
		scanf("%[^\n]%*c", buffer);
		send(clientSocket, buffer, strlen(buffer), 0);
		send(clientSocket, userId, strlen(userId), 0);
        
		if(recv(clientSocket, buffer, 100, 0) < 0){
			printf("[-]Error in receiving data.\n");
		}else{
			printf("[FROM]Server: \t%s\n", buffer);
		}
        bzero(buffer, sizeof(buffer));
        bzero(userId, sizeof(userId));
	}
	return 0;
}
