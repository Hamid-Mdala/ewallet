#include "client.h"
#include <stdio.h>
#define BUFFER_SIZE 1024
int agent_send(char *sent_string, int *sent_integer, char *flag) {
	const char *serverIP = "0.0.0.0";   //use this 0.0.0.0/127.0.0.1 ip address so that computers could connect to your server on the networkb 
	const int serverPort = 27015;
	//declare and intialize the winsock2
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		printf("WSAStartup failed: 0x%X\n", WSAGetLastError());
		return -1;
	}
	//creat a socket 
	SOCKET client_socket;
	client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket == INVALID_SOCKET) {
		printf("client socket creation failed: 0x%X\n", WSAGetLastError());
		WSACleanup();
		return -1;
	}
	//specify the client address
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(serverPort);   //convert the port number to binary
	server_address.sin_addr.s_addr = inet_addr(serverIP);  //convert the server ip address to binary
	
	//connet he client socket with client address to the server socket 
	if (connect(client_socket, (const struct sockaddr*)&server_address, sizeof(server_address)) == SOCKET_ERROR) {
		printf("The client socket is failing to connect to the server's socket: 0x%X\n", WSAGetLastError());
		closesocket(client_socket);
		WSACleanup();
		return -1;
	}
	printf("Connected to the server successfully\n");
	//writing details into a file
	FILE *transmissions = fopen("transmissions.txt", "w");
	if(transmissions == NULL){  //opening the whole script of the file
		printf("The agent client failed to send the transaction file.\n");
		closesocket(client_socket);
		WSACleanup();
		return -1;
	}
	printf("The money is being sent to the customer\n");
	fprintf(transmissions, "%s,%s,%dMK", flag ,sent_string, *sent_integer);
	fclose(transmissions); 

	//reopen the files in binary so that you could be send to the server 
	if(fopen_s(&transmissions, "transmissions.txt", "rb")) {   //read in binary 
		printf("Failed to open the transmissions file for reading in binary mode: 0x%X\n", WSAGetLastError());
		closesocket(client_socket);
		WSACleanup();
		return -1;
	} 
	//dont get the message just have a buffer able to store details
	char buffer[BUFFER_SIZE];
	size_t bytes_read;
	while((bytes_read = fread(buffer, 1, sizeof(buffer), transmissions)) > 0) {
	    if(send(client_socket, buffer, bytes_read, 0) == SOCKET_ERROR) {
	        printf("Error sending file content: 0x%X\n", WSAGetLastError());
	        fclose(transmissions);
	        closesocket(client_socket);
	        WSACleanup();
	        return -1;
	    }
	    printf("Sent %zu bytes\n", bytes_read);
	}
	printf("The agent has successfully sent transmissions text file to the customer\n");
	//clean up
	closesocket(client_socket);
	fclose(transmissions);  //close the file after 
	WSACleanup();
	return 0;
}