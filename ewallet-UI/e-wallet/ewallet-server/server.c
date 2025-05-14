#include <winsock2.h>
#include <process.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#define BUFFER_SIZE 1024
#pragma comment(lib, "ws2_32.lib")
//This will help us maintain a list of connected client(thier sockets)in my server
#define MAX_CLIENTS 15
SOCKET client_sockets[MAX_CLIENTS] = {INVALID_SOCKET};  //we made 10 client sockets 
size_t client_count = 0;
//critical section for thread safety on the machine
CRITICAL_SECTION cs;  
//allow the thread and the executing code to work in sychronize to recieve data..
//This prevents corruption of the recieved data
void initializeCriticalSection() {
	InitializeCriticalSection(&cs); //first we have to intialize the thread when we want to use in sync  
}
void cleanCriticalSection() {
	DeleteCriticalSection(&cs);  //here we have delete the thread in our operator when we want to remove it from our processor
}
//broadcasting messages to all the clients
void broadcast_message(const char *filename, SOCKET socket) { //the file name and the socket of the server is passed in the arguments
//we only have one file name the server is broadcasting 
	//critical section for thread safety
	EnterCriticalSection(&cs);
	//open the sent transmissons file in bytes so that you send that file into bytes
	FILE *transmissions = fopen(filename, "rb"); //read in binary mode 
	if(transmissions == NULL) {
		printf("Error: Unable to open the transmissions file\n");
		LeaveCriticalSection(&cs);
		return;
	}
	
	char buffer[BUFFER_SIZE];
	size_t bytes_read;
	
	//fread is use when recieving bytes
	while((bytes_read = fread(buffer, 1, sizeof(buffer), transmissions)) > 0) {   //error free when the transmission is greater than 0
		for(size_t i = 0; i < client_count; ++i) {
			if(client_sockets[i] != INVALID_SOCKET && client_sockets[i] != socket) {  
				if(send(client_sockets[i], buffer, bytes_read, 0) == SOCKET_ERROR) { //imagine the file is unable to be sent to the other device
					printf("Error sending file content to client : 0x%X\n", i, WSAGetLastError());
					closesocket(client_sockets[i]);     //closes the client socket
					client_sockets[i] = INVALID_SOCKET; //removes the client socket in the array
					return;  //connection breaked here 
				}
				printf("Broadcasted %zu bytes to other client.\n", bytes_read, i);
			} 
		}	
	}
	//clean up
	fclose(transmissions);
	LeaveCriticalSection(&cs);
	return;
}
 
void broadcast_discussion(const char* note) {
	//lock in the critical section..so that the executing code and the thread work in synchronize to receive data 
	EnterCriticalSection(&cs);
	
	for(size_t i = 0; i < client_count; ++i) {  //accessing the amount of client added to the server to send them the message 
		send(client_sockets[i], note, strlen(note), 0);
	}
	
	LeaveCriticalSection(&cs);
}
//client management functions
void add_client(SOCKET sock) {
	EnterCriticalSection(&cs);   //lock in critical section inorder to make the thread and excuting code to the sychronize
    
    if(client_count < MAX_CLIENTS) {
    	client_sockets[++client_count] = sock;   //we created a socket when the client_count is less than the max_size
    	printf(":::ADDED CLIENT (SOCKET NUMBER: %llu):::\n", (unsigned long long)sock);
	} else {
        printf("Max clients reached, rejecting connection\n");
        closesocket(sock);
	}
    LeaveCriticalSection(&cs);   //unlock critical section so that we have the excuting code up only running
}

void remove_client(SOCKET sock) {
    EnterCriticalSection(&cs);  //lock in critical section inorder to make the thread and excuting code to the sychronize
    
    for(size_t i = 0; i < client_count; ++i) {
        if(client_sockets[i] == sock) { //check if its a socket, because it was made like that in client add
            closesocket(client_sockets[i]); //close the socket.inorder to remove it in a good condition 
            //shifting the remaining elements
			for(size_t j = i; j < client_count - 1; j++) {  //the client_count -1 of the user who is removing from the connection
            	client_sockets[j] = client_sockets[j+1];    //shifting elements to the next element  
			}
			client_count--; //subtracting the client_count by one..inorder to remove the client socket from the client variable     
            client_sockets[client_count] = INVALID_SOCKET;  //make the client who is removed an invalid socket 
            break;
        }
    }
    
    LeaveCriticalSection(&cs);   //unlock critical section so that we have the excuting code up only running
}

const char* my_inet_ntop(struct in_addr addr) {  //the struct in_addr addr is an address for the ip address
	static char ip_str[16]; //mac char 15 and null ptr
	unsigned char *bytes = (unsigned char *)&addr.s_addr;
	sprintf(ip_str, "%d.%d.%d.%d", bytes[0], bytes[1], bytes[2], bytes[3]);
	return ip_str;
}

void handle_client(void *client_socket_ptr) {
	SOCKET client_fd = *(SOCKET*)client_socket_ptr;
	free(client_socket_ptr);
	if(client_fd == INVALID_SOCKET) {
		printf("creation of the client socket failed: 0x%X\n", WSAGetLastError());
		_endthread();
		return;
	}
	const char *filename = "transmissions.txt";
	FILE *transmissions = fopen(filename, "w");
	if (transmissions == NULL) {
		printf("Error failed to open the transmission log file\n");
		remove_client(client_fd);
		_endthread();
		return;
	}

	//gets the ip address of the devices 
	struct sockaddr_in client_address;
	int addr_len = sizeof(client_address);
	getpeername(client_fd, (struct sockaddr*)&client_address, &addr_len);   //this getpeername helps in geeting the socket instead of duplicating other sockets
	const char *client_ip = my_inet_ntop(client_address.sin_addr);  //give the client address
	
	//send and recieve to exchange data, the array has to be empty, so that we receive the bytes that are sent
	char buffer[BUFFER_SIZE];    
	size_t bytes_received;
	int connection_active = 1;
	int connection_deactive = 0;
	
	while(connection_active) {
		bytes_received = recv(client_fd, buffer, sizeof(buffer), 0);
		if((int)bytes_received > 0) {
			fwrite(buffer, 1, bytes_received, transmissions);
			fflush(transmissions);
			printf(":::RECEIVED %d BYTES FROM IPV4<%s> (SOCKET: %llu).\n", (int*)bytes_received, client_ip, (unsigned long long)client_fd, client_count);
			broadcast_message(filename, client_fd);
		}
		else if((int)bytes_received == 0) {
			printf("CLIENT IPV4<%s> DISCONNECTED GRACEFULLY\n", client_ip);
			connection_active = connection_deactive;
		} else {
			int error = WSAGetLastError();
			if(error == WSAECONNRESET) {
				printf("CLIENT IPV4<%s> FORCIBLE DISCONNECTED (CONNECTION RESET)\n", client_ip);
			} else {
				printf("RECEIVE ERROR FROM CLIENT IPV4<%s> (SOCKET: %llu)", client_ip, (unsigned long long)client_fd);
			}
			connection_active = connection_deactive;
		}
	}
	
	//when client is disconnected 
	char notification[BUFFER_SIZE];
	sprintf(notification, "NOTIF|[SERVER] CLIENT DISCONNECTED IPV4<%s> (REMAINING CLIENTS: %d)\n", client_ip, client_count-1);
	broadcast_discussion(notification);
		
	printf("ENDED CONNECTION WITH IPV4<%s> (SOCKET : %llu)\n", client_ip, (unsigned long long)client_fd);
	
	//clean up
	fclose(transmissions);
	remove_client(client_fd);
	_endthread();
}

int main() {
	const int serverPort = 27015;
	
	initializeCriticalSection();
	//declare and intialize winsock2
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {    //the intilaized winsock is not null
		printf("WSAStartup failed: 0x%X\n", WSAGetLastError());
		cleanCriticalSection();
		return -1;
	}
	//create a socket
	SOCKET server_socket;
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket == INVALID_SOCKET) {
		printf("socket creation failed: 0x%X\n", WSAGetLastError());
		WSACleanup();
        cleanCriticalSection();
        return -1;
	}
	
	//allow socket reuse (avoid "specifying the address of a server socket when already in use")
	int opt = 1;
	setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));
	
	//specify the address of the socket
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;  //using the ipv4 or ipv6
	server_address.sin_port = htons(serverPort);   //htons converts port into the binary form
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);   //able to listen on all network interfaces(WIFI, LAN & INTERNET)
	
	//bind the server socket with the server address
	if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == SOCKET_ERROR) {
	    printf("bind failed: 0x%X\n", WSAGetLastError());
		closesocket(server_socket);
        WSACleanup();
        cleanCriticalSection();
		return -1;
	}
	//listen to the connections
	if (listen(server_socket, SOMAXCONN) == SOCKET_ERROR) {      //THE MAXIMUM CONNECTION THE SERVER SYSTEM WILL HANDLE somaxconn
		printf("listen failed: 0x%X\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        cleanCriticalSection();
	}
	
	//yes the server is running 
	printf("Server started and listening on port: %d\n", serverPort);
	printf("Waiting for connections....\n");
	
	//allow the client to accept the connections in an infinite loop  
	while(1) {	
		struct sockaddr_in client_address;  //specified structure of client address..
		//we can't specify the client address..because its not on own machine 
		int clientAddrLen = sizeof(client_address); //the size of the client structure
		SOCKET client_socket = accept(server_socket, (struct sockaddr*)&client_address, &clientAddrLen);
		
		if (client_socket == INVALID_SOCKET) {
			printf("accept failed: 0x%X\n", WSAGetLastError());
            continue;   //skip it..so that other user could connect to the other client sockets could connect to the server socket  
		}
		const char *client_ip = my_inet_ntop(client_address.sin_addr);
		printf("ACCEPTING NEW CONNECTION FROM IPV4<%s> (SOCKET: %llu)\n", client_ip, (unsigned long long)client_socket);  //using my_inet_ntop want to allow multiple threads running on the server, instead of using one to thread on my server computer
		
		char notification[BUFFER_SIZE];
		sprintf(notification, "NOTIF|[SERVER] NEW CLIENT CONNECTION IPV4<%s> (TOTAL CLIENTS: %d)", client_ip, client_count);
		broadcast_discussion(notification);
		
		SOCKET *client_socket_ptr = malloc(sizeof(SOCKET));
        if(client_socket_ptr == NULL) {
            printf("Memory allocation failed\n");
            closesocket(client_socket);
            continue;  //skip it..so that other user could connect to the other client sockets could connect to the server socket  
        }
		
		*client_socket_ptr = client_socket;  //dereference it so that the client_socket_ptr = client_socket
		add_client(client_socket);
		//start a new thread on the server's processor for a new client
		_beginthread(handle_client, 0, client_socket_ptr);
	}
	
	//clean up
	closesocket(server_socket);
	WSACleanup();
    cleanCriticalSection();
	return 0;
} 