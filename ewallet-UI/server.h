#pragma once 

#include <winsock2.h>
#include <process.h> //For the _begintheard() & _endthread() used in thearding 

#define BUFFER_SIZE 1024
//#pragma comment(lib, "ws2_32.lib")  // Auto-linking (optional but helpful

void handle_client(void *client_socket_ptr);
int customer_receive();
