//prepoceser directives/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
//user made directives/libraries
#include "client.h"
#include "FileManager.h"
#include "Validations.h"
#include "Implementations.h"
#define BUFFER_SIZE 1024
#pragma comment(lib, "ws2_32.lib")  // Auto-linking (optional but helpful)

bool make_search_query() {
	bool exists;
	FILE *fptr = fopen("Query.txt", "r"); //mode r reads and writes content to the file without changing the existing content
    if (fptr == NULL) {
    	system("CLS");
        printf("Error: Unable to open the Query file/n");
        exit(1);
    } else {
		char line[BUFFER_SIZE];
	    while (fgets(line, sizeof(line), fptr)) {
			if(strlen(line) > 20) {
				return true;
			}
		} return false;
	}
}

void registration() {
	
	FILE* openFile = fopen("user_data.txt", "a");
	if(openFile == NULL) {
		printf("Error opening the file\n");
		exit(1);
	}
	
	bool exists;
	//registration_form
	char phone_number[BUFFER_SIZE];	
	printf("Enter phone_number: ");    
	scanf("%s", phone_number); 
	exists = validate(phone_number);
	if (exists) {
		exists = searchFirstField("user_data.txt", phone_number);
	    if(exists) {
			printf("Phone Number already exists\n");
		} else {
			char password[BUFFER_SIZE];	
			printf("Enter password: ");
		    scanf("%s", password);
		    exists = validate(password);
		    if (exists) {
		        char first_name[BUFFER_SIZE];	
				printf("Enter First Name: ");
		        scanf("%s", first_name);
		        exists = validate(first_name);
		        if (exists) {
		        	char last_name[BUFFER_SIZE];	
		            printf("Enter Last Name: ");
		            scanf("%s", last_name);
		            exists = validate(last_name);       
		        	if(exists) {
		        		system("CLS");
		        		register_user(phone_number, password, first_name, last_name);
		        		UtilitySendFileLines("user_data.txt"); //sending the lines inside the user data to the server 
					} 
				} else {
		        	printf("Invalid Last Name length\n");
				}
		    } else {
		    	printf("Inavalid Password length\n");
			}
		} 
	} else {
		printf("Invalid Phone Number length\n");
	}
}

static bool beforeNavigationDashboard() {
	bool exists;
	FILE *fptr1 = fopen("Query.txt", "a");
	if(fptr1 == NULL) {
		printf("Error: Unable to open the Query file/n");
		exit(1);
	} else {
		exists = make_search_query();
		if(exists) {
			//enter the customer code to go to the buy customer 
			char ussd[BUFFER_SIZE];
			printf("Enter USSD code: "); //its unstructured supplementary service data
			scanf("%s", ussd);
			system("CLS");
			if(strcmp(ussd,"*101#") == 0) {
				/* [FUTURE UPDATES] 
				next time once the user enters the ussd just_connect(), connect them to the server
				and next time allow the user to have the option to buy bundles or buy airtime
				implement notifications next time so that the user is notified on their transactions, 
				when the connected agents on the same server started publishig content
				and also feature added later: 2.PAY BILLS like electricity, water, house rent, school fees & MIFI internet bills*/
				printf("1. BUY TOKENS\n");   							  //used for buying bundles	
		        printf("2. SEND CASH TO ANOTHER NUMBER\n");  			  //send cash to another client
		        printf("3. VIEW ACCOUNT DETAILS\n");   					  //able to see details from their query file 
		        printf("4. RECEIVE MONEY\n");							  //used for receiving money
				/* [FUTURE UPDATE]
				VENTURE INTO AGENT PUBLISHING EVENTS OR QUOATES IN LIFE
				competitions(sports,educational), lectures & spiritual events published by agents 
				1.SPORTS AND ENTERTAINMENT 2. INTERESTING TOPICS 3. LECTURES ON RELIGION 4. LIFE HACKS 
				(so that the agent is able to choose on what part to negoitate and also add choice of content the clients what to look or even comment at)
				*/
		    	/* [FUTURE UPDATE]
				CONTRIBUTE TO WORLDLIFE ORGANIZATION/NGOs
				Assume we have a link with MALAWI WORLD VISION, HABITANT, & WATERAID MALAWI
				*/ 
				printf("5. EXIT\n");
				printf("Enter your choice option: ");
    			int choice;
				scanf("%d", &choice);
    			system("CLS");
				return customerAfterNavigationDashboard(choice);
			} else if(strcmp(ussd,"2025") == 0) {
				/* [FUTURE UPDATE]
				The agent & customer user must have an implementation that helps him navigate the transaction history*/
				printf("1. TRANSACTION\n");					//send money to the customers
				/* [FUTURE UPDATE]
				content creating used for making deals
				Making deals with the clients && any deals agents have to offer once the deals are being convincing 
				how convincing(when they are purchased) we deduct some thing from the agents 
				*/	
				printf("2. RECIEVE REGISTRATION FILE\n");		//used for registration of handling when the clients register
				printf("3. SEND REGISTRATION FILE\n");				//sending the client the user data file
				printf("4. EXIT\n");                      
				printf("Enter your choice option: ");
				int choice;
    			scanf("%d", &choice);
    			system("CLS");
    			return agentAfterNavigationDashboard(choice);
			} else {
				printf("Invalid USSD\n");
			}
	    } else {
	    	/* [FUTURE UPDATE]
			the system must use some sort of implemetation to automatically know if the user operation using the
			system is either an agent user or cleint user*/
			char option[10];
	    	printf("are you an agent or not (yes) or (not): ");
	    	scanf("%s", option);
	    	system("CLS");
	    	if(strcmp(option, "yes") == 0) {
	    		/*the agent has to enter the entry 
				details inside the user data file*/
				registration();
			} else if(strcmp(option, "not") == 0) {  
				/*client has to connect first to the
				server inorder to recieve the user data 
				file and write details in the file*/
				justConnect();
				registration();
			} else {
				system("CLS");
				printf("Wrong option\n");
			}
	    } 
	} return true;
}
int main(int argc, char** argv[]) {
    printf("\n:::::::WELCOME TO TRIPLE H SERVICES:::::::.\n");
    beforeNavigationDashboard();
    return 0; 
}
//so how will i use other pc to get the money
//have the query file for storing your details only and the user data file is sent to the server as transmission 
//then we get to read the first field of the transimmison to save across the other device as user data 