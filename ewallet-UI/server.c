#include "server.h"
#include <stdio.h>

void handle_client(void *client_socket_ptr) {
	SOCKET client_fd = *(SOCKET*)client_socket_ptr;
	free(client_socket_ptr);
	if(client_fd == INVALID_SOCKET) {
		printf("creation of the client socket failed: 0x%X\n", WSAGetLastError());
		_endthread();
		return;
	}
	FILE *transmissions = fopen("transmissions.txt", "w");
	if (transmissions == NULL) {
		printf("Error failed to open the transmission log file\n");
		closesocket(client_fd);
		_endthread();
		return;
	}
	//send and receive the file from the client 
	//the buffer always has to be empty, so that we receive bytes 
	char buffer[BUFFER_SIZE];    
	int bytes_received;
	while((bytes_received = recv(client_fd, buffer, sizeof(buffer), 0)) > 0) {
		fwrite(buffer, 1, bytes_received, transmissions);
		//write immediaty to file
		fflush(transmissions);
		printf("Received %d bytes._", bytes_received);
	}
	if(bytes_received == SOCKET_ERROR) {
		printf("The server is failing to receive data from the client socket: 0x%X\n", WSAGetLastError());
		closesocket(client_fd);
		fclose(transmissions);
		_endthread();
		return;
	}
	//clean up
	fclose(transmissions);
	closesocket(client_fd);
	printf("Client disconnected.\n");
	_endthread();
}

int customer_receive() {
	const int serverPort = 27015;
	//declare and intialize winsock2
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {    //the intilaized winsock is not null
		printf("WSAStartup failed: 0x%X\n", WSAGetLastError());
		return -1;
	}
	//create a socket
	SOCKET server_socket;
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket == INVALID_SOCKET) {
		printf("server socket creation failed: 0x%X\n", WSAGetLastError());
		WSACleanup();
		return -1;
	}
	//specify the socket address
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(serverPort);   //htons converts port into the binary form
	server_address.sin_addr.s_addr = INADDR_ANY;
	//bind the server socket with the server address
	if (bind(server_socket, (const struct sockaddr*)&server_address, sizeof(server_address)) == SOCKET_ERROR) {
		printf("Binding the socket with the server address failed: 0x%X\n", WSAGetLastError());
		closesocket(server_socket);
		WSACleanup();
		return -1;
	}
	//listen to the connections
	if (listen(server_socket, SOMAXCONN) == SOCKET_ERROR) {      //THE MAXIMUM CONNECTION THE SERVER SYSTEM WILL HANDLE somaxconn
		printf("The server is failing to listen for the pending connections: 0x%X\n", WSAGetLastError());
		closesocket(server_socket);
		WSACleanup();
		return -1;
	}
	printf("The server is listening on port: %d\n", serverPort);
	//allow the client to accept the connections 
	SOCKET client_socket;
	struct sockaddr_in client_address;
	int clientAddrLen = sizeof(client_address);
	while(1) {
		client_socket = accept(server_socket, (struct sockaddr*)&client_address, &clientAddrLen);
		if (client_socket == INVALID_SOCKET) {
			printf("The server is failing to accept the connection from the client: 0x%X/n", WSAGetLastError());
			closesocket(server_socket);
			WSACleanup();
			continue;
		}
		printf("New Client Connected._");

		SOCKET* client_socket_used_in_thread = malloc(sizeof(SOCKET));
		if (client_socket_used_in_thread == NULL) {
			printf("The client socket failed to be allocated on a heap memory");
			closesocket(client_socket);
			continue;
		}
		*client_socket_used_in_thread = client_socket;   //dereferencing pointer to assign to client_socket 
		//start a new thread on the server's processor for a new client
		//printf("Server socket: %d\n", server_socket);
		//printf("Client socket: %d\n", client_socket);
		_beginthread(handle_client, 0, client_socket_used_in_thread);
	}
	//clean up
	closesocket(server_socket);
	closesocket(client_socket);
	WSACleanup();
	return 0;
} 
