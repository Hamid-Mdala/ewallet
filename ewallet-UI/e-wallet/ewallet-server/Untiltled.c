//#include <winsock2.h>
//#include <process.h>
//#include <stdlib.h>
//#include <stdio.h>
//#include <windows.h>
//#include <ws2tcpip.h>
//#define BUFFER_SIZE 1024
//#pragma comment(lib, "ws2_32.lib")
//#define MAX_CLIENTS 15
//
//SOCKET client_sockets[MAX_CLIENTS] = {INVALID_SOCKET};
//size_t client_count = 1;
//CRITICAL_SECTION cs;
//
//void initialize_critical_section() {
//    InitializeCriticalSection(&cs);
//}
//
//void cleanup_critical_section() {
//    DeleteCriticalSection(&cs);
//}
//
//void broadcast_message(const char *filename, SOCKET sender_socket) {
//    EnterCriticalSection(&cs);
//    
//    FILE *transmissions = fopen(filename, "rb");
//    if(transmissions == NULL) {
//        printf("Error: Unable to open the transmissions file\n");
//        LeaveCriticalSection(&cs);
//        return;
//    }
//    
//    char buffer[BUFFER_SIZE];
//    size_t bytes_read;
//    
//    while((bytes_read = fread(buffer, 1, sizeof(buffer), transmissions)) > 0) {
//        for(size_t i = 0; i < client_count; i++) {
//            if(client_sockets[i] != INVALID_SOCKET && client_sockets[i] != sender_socket) {
//                if(send(client_sockets[i], buffer, bytes_read, 0) == SOCKET_ERROR) {
//                    printf("Client %d disconnected during broadcast(send error: 0x%X)\n", i, WSAGetLastError());
//                    closesocket(client_sockets[i]);
//                    client_sockets[i] = INVALID_SOCKET;
//                }
//            } 
//        }    
//    }
//    
//    fclose(transmissions);
//    LeaveCriticalSection(&cs);
//}
//
//void broadcast_discussions(const char *discussions) {
//    EnterCriticalSection(&cs);
//    
//    for(int i = 0; i < client_count; i++) {
//    	send(client_sockets[i], discussions, strlen(discussions), 0);
//    }
//    
//    LeaveCriticalSection(&cs);
//}
//
//void add_client(SOCKET sock) {
//    EnterCriticalSection(&cs);
//    
//    if(client_count < MAX_CLIENTS) {
//        client_sockets[client_count++] = sock;
//        printf("ADDED CLIENT %d (SOCKET: %llu)\n", client_count-1, (unsigned long long)sock);
//    } else {
//        printf("MAX CLIENTS REACHED, REJECTING CONNECTIONS (SOCKET: %llu)\n", (unsigned long long)sock);
//        closesocket(sock);
//    }
//    
//    LeaveCriticalSection(&cs);
//}
//
//void remove_client(SOCKET sock) {
//    EnterCriticalSection(&cs);
//    
//    for(size_t i = 0; i < client_count; i++) {
//        if(client_sockets[i] == sock) {
//            printf("REMOVING CLIENT %d (SOCKET: %llu)\n", i, (unsigned long long)sock);
//            closesocket(client_sockets[i]);
//            // Shift remaining elements
//            for(size_t j = i; j < client_count - 1; j++) {
//                client_sockets[j] = client_sockets[j+1];
//            }
//            client_count--;
//            client_sockets[client_count] = INVALID_SOCKET;
//            break;
//        }
//    }
//    
//    LeaveCriticalSection(&cs);
//}
//
//const char* my_inet_ntop(struct in_addr addr) {
//    static char ip_str[16];
//    unsigned char *bytes = (unsigned char *)&addr.s_addr;
//    sprintf(ip_str, "%d.%d.%d.%d", bytes[0], bytes[1], bytes[2], bytes[3]);
//    return ip_str;
//}
//
//void handle_client(void *client_socket_ptr) {
//    SOCKET client_fd = *(SOCKET*)client_socket_ptr;
//    free(client_socket_ptr);
//    
//    if(client_fd == INVALID_SOCKET) {
//        printf("Invalid client socket\n");
//        _endthread();
//        return;
//    }
//    
//    struct sockaddr_in client_address;
//    int addr_len = sizeof(client_address);
//    getpeername(client_fd, (struct sockaddr*)&client_address, &addr_len);
//    const char *client_ip = my_inet_ntop(client_address.sin_addr);
//    
//    //printf("Handling new client from %s (socket: %llu)\n", client_ip, (unsigned long long)client_fd);
//    
//    const char *filename = "transmissions.txt";
//    FILE *transmissions = fopen(filename, "w");
//    if (transmissions == NULL) {
//        printf("Error failed to open the transmission log file\n");
//        remove_client(client_fd);
//        _endthread();
//        return;
//    }
//    
//    char buffer[BUFFER_SIZE];
//    int bytes_received;
//    int connection_active = 1;
//    
//    while(connection_active) {
//        bytes_received = recv(client_fd, buffer, sizeof(buffer), 0);
//        
//        if(bytes_received > 0) {
//            fwrite(buffer, 1, bytes_received, transmissions);
//            fflush(transmissions);
//            printf("RECIEVED %d BYTES FROM IPV4<%s>\n", bytes_received, client_ip);
//            broadcast_message(filename, client_fd);
//        }
//        else if(bytes_received == 0) {
//            printf("Client IPV4<%s> GRACEFULLY DISCONNECTED\n", client_ip);
//            connection_active = 0;
//        }
//        else {
//            int error = WSAGetLastError();
//            if(error == WSAECONNRESET) {
//                printf("Client IPV4<%s> FORCIBLY DISCONNECTED (CONNECTION RESET)\n", client_ip);
//            } else {
//                printf("RECIEVE ERROR FROM %s: 0x%X\n", client_ip, error);
//            }
//            connection_active = 0; //this will break the conditions
//        }
//    }
//    //server notifing the client that they connected to the server
//	char notification[BUFFER_SIZE];
//	sprintf(notification, "NOTIF|[SERVER] Client DISCONNECTED: <%s> (REMAINING CLIENTS: %d)", client_ip, client_count-1);
//	broadcast_discussions(notification);
//	
//	//closing from the servers side 
//    printf("CLOSING CONNECTION WITH <%s> (SOCKET: %llu)\n", client_ip, (unsigned long long)client_fd);
//    fclose(transmissions);
//    remove_client(client_fd);
//    _endthread();
//}
//
//int main() {
//    const int serverPort = 27015;
//    initialize_critical_section();
//    
//    WSADATA wsaData;
//    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
//        printf("WSAStartup failed: 0x%X\n", WSAGetLastError());
//        cleanup_critical_section();
//        return -1;
//    }
//    
//    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);
//    if (server_socket == INVALID_SOCKET) {
//        printf("socket creation failed: 0x%X\n", WSAGetLastError());
//        WSACleanup();
//        cleanup_critical_section();
//        return -1;
//    }
//    
//    int opt = 1;
//    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));
//    
//    struct sockaddr_in server_address;
//    server_address.sin_family = AF_INET;
//    server_address.sin_port = htons(serverPort);
//    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
//    
//    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == SOCKET_ERROR) {
//        printf("bind failed: 0x%X\n", WSAGetLastError());
//        closesocket(server_socket);
//        WSACleanup();
//        cleanup_critical_section();
//        return -1;
//    }
//    
//    if (listen(server_socket, SOMAXCONN) == SOCKET_ERROR) {
//        printf("listen failed: 0x%X\n", WSAGetLastError());
//        closesocket(server_socket);
//        WSACleanup();
//        cleanup_critical_section();
//        return -1;
//    }
//    
//    printf("Server started and listening on port %d\n", serverPort);
//    printf("Waiting for connections...\n");
//    
//    while(1) {
//        struct sockaddr_in client_address;
//        int clientAddrLen = sizeof(client_address);
//        SOCKET client_socket = accept(server_socket, (struct sockaddr*)&client_address, &clientAddrLen);
//        
//        if (client_socket == INVALID_SOCKET) {
//            printf("accept failed: 0x%X\n", WSAGetLastError());
//            continue;
//        }
//        
//        const char *client_ip = my_inet_ntop(client_address.sin_addr);
//		printf("ACCEPTING NEW CONNECTION FROM IPV4<%s> SOCKET: %llu\n", client_ip, (unsigned long long)client_socket);
//        
//		char notification[BUFFER_SIZE];
//		sprintf(notification, "NOTIF|[SERVER] NEW CLIENT CONNECTION: %s (TOTAL CLIENTS: %d)", client_ip, client_count);
//		broadcast_discussions(notification);
//
//        SOCKET *client_socket_ptr = malloc(sizeof(SOCKET));
//        if(client_socket_ptr == NULL) {
//            printf("Memory allocation failed\n");
//            closesocket(client_socket);
//            continue;
//        }
//        
//        *client_socket_ptr = client_socket;
//        add_client(client_socket); //added client to the servers processor 
//        _beginthread(handle_client, 0, client_socket_ptr);
//    }
//    
//    closesocket(server_socket);
//    WSACleanup();
//    cleanup_critical_section();
//    return 0;
//}