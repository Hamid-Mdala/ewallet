#pragma once

#include <stdbool.h>
#include "FileManager.h"
#include <winsock2.h>

#include <process.h> //used for _beginthreadex uses stack data structure than CreateThread causes memory leaks

#define BUFFER_SIZE 1024
//#pragma comment(lib, "ws2_32.lib")  // Auto-linking (optional but helpful)

typedef struct {
    SOCKET client_socket; // Socket for this thread
    volatile int keepRunning;	  // Control flag (1 = run, 0 = stop)
} ThreadArgs;

unsigned __stdcall ReceiveThread(void* pArguments);

//client sending a file to the server 
int sendMoney(const char *phone_number, const int *amount);
int sendFileLines(const char *line);
int justSend(const char *filename);

//client just connect to the network 
int justConnect();

//client sending a message to the server
int sendMessage(const char *discussions);

//handling the file content send to the file
void strip_newline(const char *filename);