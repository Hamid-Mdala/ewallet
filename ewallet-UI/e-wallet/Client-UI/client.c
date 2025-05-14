#include <stdio.h>
#include "client.h"
#include <windows.h>

/* [IMPORTANT NOTES]
then remove that newline from the file inoder to establish file handling content 
this function is very important it is used to control newline when the customer recieves the user data and the newline is inserted 
then after when the agent receive using just connect will allow removing the newline from the file */

//recieve data continuosly when the user calls the just connnect function
unsigned __stdcall ReceiveThread(void* pArguments) {
	ThreadArgs *args = (ThreadArgs*)pArguments;
	char buffer[BUFFER_SIZE];
	
	while(args->keepRunning) {
		int bytes_received = recv(args->client_socket, buffer, BUFFER_SIZE - 1, 0);
		if(bytes_received > 0) {
			
			buffer[bytes_received] = '\0';  //adding a newline character of the amount of space used 
		
			//handling the notifications sent from the server
			if(strncmp(buffer, "NOTIF|", 6) == 0) {
				printf("\nNOTIFICATION: %s\n", buffer + 6);
			} else {
				//handling when user data file is sending the user data file 
				if(strncmp(buffer, "0", 1) == 0) {
					FILE *userData = fopen("user_data.txt", "a");
					if(userData == NULL) {
						printf("Error opening the user data file\n");
						args->keepRunning = 0;
						break;
					} else {
						fprintf(userData, "%s\n", buffer);
						fclose(userData);  
					}		
				}
			
				//handling when a message is sent message sent 
				else if(strncmp(buffer, "MSG|", 4) == 0) {
					printf("\nMESSAGE: %s\n", buffer + 4);  
				}
			
				//handling when transaction file is sent 
				else if(strncmp(buffer, "FILE|transactions|", 18) == 0) {
					FILE *transactions = fopen("transactions.txt", "w");
					if(transactions == NULL) {
						printf("Erorr opening the user data file\n");
						break;
					} else {
						const char *return_number = returnNumber();  ///this is the will get the number in the Query file
						bool exists;
						//265994600600 this is the return number 
						//FILE|transactions|265994600600,80MK this is the buffer 
						//using strstr needle and haystack will work  
						
						if(strstr(buffer, return_number)) {
							//the number could be 265994500600 return number but since its not found in the haystack
							fprintf(transactions, "%s\n", buffer + 18);
							fclose(transactions);      //UPDATE(please implement a map in c using objects and struct)
							printf("\nYou have successfully received money from the client\n");
						} else {
							printf("Opps unable to authenticate the account\n");
							args->keepRunning = 0;
							break;
						}
					} 
				}
					
				//handling the message that has an unkwown purpose 
				else {
					printf("\nUnknown message type: %s\n", buffer);
				}
			}
		}
		//when the connection is cut off
		else if(bytes_received == 0) {
			//when bytes_reieved is equal to zero then, its when you closed the connection 
			printf("\nSERVER DISCONNECTED\n");  //when program ends
			args->keepRunning = 0; //this breaks the while loop
			break;
			
		//when errors occur while recieving details
		} else {
			int error = WSAGetLastError();
			if(error == WSAECONNRESET) {
				printf("\nSERVER CONNECTION RESET\n"); //when the terminal is forcifly removed
			} else {
				printf("\nRECIEVE ERROR: 0x%X", WSAGetLastError());
			}
			args->keepRunning = 0;
			break;
		}	
	} 
	
	closesocket(args->client_socket);  //clean up
	free(args);						   //free memory 
	WSACleanup();
	return 0;
}

//stops the thread
void stop_receive_thread(ThreadArgs* args) {
    args->keepRunning = 0;          // Signal thread to stop
    shutdown(args->client_socket, SD_BOTH);  // shut down the thread
}
//this function just connects the other client and recieve bytes
int justConnect() {  
	const char *serverIP = "192.168.1.178";
	const int Port = 27015;
	//declare and intialize winsock2
	WSADATA wsaData;
	
	if(WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
		printf("WSAStartup failed: 0x%X\n", WSAGetLastError());
		return -1;
	}
	//create a socket and then intialize it with the type of connection you want to establish
	SOCKET client_socket;
	client_socket = socket(AF_INET, SOCK_STREAM, 0);
	
	if(client_socket == INVALID_SOCKET) {
		printf("client socket creation failed: 0x%X\n", WSAGetLastError());
		WSACleanup();
		return -1;
	}
	
	//specify our socket with address structure
	struct sockaddr_in client_address;
	client_address.sin_family = AF_INET;
	client_address.sin_port = htons(Port);
	client_address.sin_addr.s_addr = inet_addr(serverIP);
	//connect to the server until you get response files from the server 
	
	if(connect(client_socket, (struct sockaddr*)&client_address, sizeof(client_address)) == SOCKET_ERROR) {
		printf("Connection failed: 0x%X\n", WSAGetLastError());
		closesocket(client_socket);
		WSACleanup();
		return -1;	
	}
    
	printf("Your device connected to the server successfully\n");
	
	ThreadArgs* args = (ThreadArgs*)malloc(sizeof(ThreadArgs));
	args->client_socket = client_socket;  // Or
	args->keepRunning = 1;

	//start recieve thread 
	HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, ReceiveThread, args, 0, NULL);  //takes them to receive thread
	if (!hThread) {
        free(args);
        closesocket(client_socket);
        return -1;
    }
 
	
	FILE *transmissions = fopen("transmissions.txt", "a"); //write in append mode 
	if(transmissions == NULL) {
		printf("Error opening log file");
		args->keepRunning = 0;
		WaitForSingleObject(hThread, INFINITE);
		CloseHandle(hThread);
		closesocket(client_socket);
		WSACleanup();
		return -1;
	}	
	
	char stopThread[BUFFER_SIZE];
	// User enters Enter to stop
	printf("\nENTER 'ENTER' TO STOP THE THREAD...\n");
	scanf("%s", stopThread);
	while(args->keepRunning) {
		if(strcmp(stopThread, "ENTER") == 0) {
			// Signal thread to exit
			args->keepRunning = 0;    //this will break the loop           
			shutdown(client_socket, SD_BOTH);    //this will shutdown the synchronize mood
			
			// Wait for thread to finish
			WaitForSingleObject(hThread, INFINITE);    //waiting for the thread to end safetly 
			CloseHandle(hThread);    //close the thread 
			
			printf("Thread stopped. Continuing...\n");
			fclose(transmissions);
			break;
		}
	}
	//clean up
	closesocket(client_socket);
    WSACleanup();
	return 0;
}
int sendFileLines(const char *line) {
	const char *serverIP = "192.168.1.178";
	const int Port = 27015;
	WSADATA wsaData;
	
	if(WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
		printf("WSAStartup failed: 0x%X\n", WSAGetLastError());
		return -1;
	}
	
	SOCKET client_socket;
	client_socket = socket(AF_INET, SOCK_STREAM, 0);
	
	if(client_socket == INVALID_SOCKET) {
		printf("Client socket creation failed: 0x%X\n", WSAGetLastError());
		WSACleanup();
		return -1;
	}
	
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(Port);
	server_address.sin_addr.s_addr = inet_addr(serverIP);
	
	if(connect(client_socket, (const struct sockaddr*)&server_address, sizeof(server_address)) == SOCKET_ERROR) {
		printf("Connection failed: 0x%X\n", WSAGetLastError());
		closesocket(client_socket);
		WSACleanup();
		return -1;
	} 

	printf("Your device connected to the server successfully\n");
	
	//make transmissions file in the same directory as the source code before we send
	FILE *transmissions = fopen("transmissions.txt", "w");
	if(transmissions == NULL) {
		printf("Error: Unable to open the transmissions file\n");
		closesocket(client_socket);
		WSACleanup();
		return -1;
	}
	
	fprintf(transmissions, "%s", line);   //send line content without the newline character from the helper
	fclose(transmissions);
	
	if(fopen_s(&transmissions, "transmissions.txt", "rb")) {   
		printf("Failed to open file for reading\n", WSAGetLastError());
		closesocket(client_socket);
		WSACleanup();
		return -1;
	}
	
	char buffer[BUFFER_SIZE];
	size_t send_bytes;
	
	while((send_bytes = fread(buffer, 1, sizeof(buffer), transmissions)) > 0) {
		if(send(client_socket, buffer, send_bytes, 0) == SOCKET_ERROR) {
			printf("Error sending file content: 0x%X\n", WSAGetLastError());
	        fclose(transmissions);
	        closesocket(client_socket);
	        WSACleanup();
	        return -1;
		}
		
		Sleep(1000); //pre 1 seconds delay
		printf("SENT %d BYTES\n", (int)send_bytes);
	}	
	//clean up
	/*delete the user data file, so that the user does have access to that file anymore*/
	closesocket(client_socket);
	fclose(transmissions);
	WSACleanup();
	return 0;
}

//when the user registers its saved in the user data file shared to the server in append mode so that the server is able to view and search for the number
//once that works,
//the client user first has to deposit money into the  