#include "Implementations.h"
#include "Validations.h"
#include "client.h"
#include "FileManager.h"

bool agentAfterNavigationDashboard(int choice) {
	switch(choice) {
		case 1:
			printf("Enter phone number: ");
			char phone_number[BUFFER_SIZE];
			scanf("%s", phone_number);
			bool exists;
			exists = validate(phone_number);
			if(exists) {
				exists = searchFirstField("user_data.txt", phone_number);   //checking if the number is existing in the files
				if(exists) {
					
					int amount;
					printf("Enter the amount you want to send: ");
					scanf("%d", &amount);
					
					FILE *send = fopen("transactions.txt", "w");  //write in normal mode
					if(send == NULL) {
						printf("Error opening the file\n");
						return -1;
					} else {
						/*[FUTURE UPDATE]
						THE MONEY THE AGENT IS SENDING TO THE CLIENT MUST ALSO DEDUCT AS THE MONEY IS BEING SENT
						ALSO INCLUDE A SECURITY MECHANISM BEFORE THE AGENT SENDS MONEY*/
						fprintf(send, "FILE|transactions|%s,%dMK\n", phone_number, amount);
						fclose(send);
						helperSendFileLines("transactions.txt");
					}
					
				} else {
					printf("Unable to find the phone number in the files\n");
				}
			} else {
				printf("Error: the phone number is invalid\n");
			}
			break;
		case 2: //helps the agent get the user data, to know them and communicate effectively  	
			justConnect(); 
			break;
		case 3: //the agent sends the user datea file to the clients 
			helperSendFileLines("user_data.txt");  
			break;
		default: 
			printf("Error: Invalid choice option\n");
			return -1;
			break;
	}	
}

bool customerAfterNavigationDashboard(int choice) {
   switch (choice) {
	   	case 1: //BUY TOKENS
			printf("1. Purchase 200 tokens with MK200\n");
			printf("2. Purchase 300 tokens with MK300\n");
			printf("Enter your choice: ");
			scanf("%d", &choice);
			if(choice == 1) {
		   	
				int amount;
				printf("Enter the amount: ");
				scanf("%d", &amount);
			
				FILE *check = fopen("transactions.txt", "r");
			    if(check == NULL) {
			    	printf("Error unable open file, because you do not have your saving yet\n");
			    	return -1;
				} else {
					char line[BUFFER_SIZE];
					const char *saving;
					const char *p_number;
					while(fgets(line, sizeof(line), check)) {
						p_number = strtok(line, ",");
						saving = strtok(NULL, "\n");
					}
					
					if(p_number == NULL && saving == NULL) {
						printf("You have no money in your trasaction file\n");
						return -1;
					}
					
					int money = atoi(saving);
					int deductions = money - amount;  //deducted the money extracted from the files with the money the client enters
					
					if(deductions <= 0) {
						printf("Your saving are very low to buy tokens\n");
						return -1;
					} else {
						/*[FUTURE UPDATE]
						ABLE TO MAKE MONEY WHEN THE CLIENT USES OUR SERVICES LIKE THIS
						SHOULD BE ABLE TO USE THE PASSWORD BEFORE THE USER SENDS MONEY TO OTHER CLIENTS OR THE AGENTS
						*/
						FILE *send = fopen("transactions.txt", "w");
						if(send == NULL) {
							printf("Error unable open file\n");
							return -1;
						} else {
							printf("You have successfully bought the 200 tokens\n");
							printf("Your balance is %d\n", deductions);
							fprintf(send, "FILE|transactions|%s,%dMK", p_number, deductions);  //updating the file with new amount
							fclose(send);
						}
					
					}
				}
				
				//clean up
				fclose(check);	
		   } else if(choice == 2) {
		   	
				int amount;
				printf("Enter the amount: ");
				scanf("%d", &amount);
			
				FILE *check = fopen("transactions.txt", "r");
		    	if(check == NULL) {
		    		printf("Error unable open file, because you do not have your saving yet\n");
		    		return -1;
				} else {
					char line[BUFFER_SIZE];
					const char *saving;
					const char *p_number;
				
					while(fgets(line, sizeof(line), check)) {
						p_number = strtok(line, ",");
						saving = strtok(NULL, "\n");
					}
				
					if(p_number == NULL && saving == NULL) {
						printf("You have no money in your trasaction file\n");
						return -1;
					}
				
					int money = atoi(saving);
					int deductions = money - amount;  //deducted the money extracted from the files with the money the client enters
				
					if(deductions <= 0) {
						printf("Your saving are very low to buy tokens\n");
						return -1;
					} else {
						/*[FUTURE UPDATE]
						ABLE TO MAKE MONEY WHEN THE CLIENT USES OUR SERVICES LIKE THIS
						SHOULD BE ABLE TO USE THE PASSWORD BEFORE THE USER SENDS MONEY TO OTHER CLIENTS OR THE AGENTS
						*/
						FILE *send = fopen("transactions.txt", "w");
						if(send == NULL) {
							printf("Error unable open file\n");
							return -1;
						} else {
							printf("You have successfully bought the 200 tokens\n");
							printf("Your balance is %d\n", deductions);
							fprintf(send, "FILE|transactions|%s,%dMK", p_number, deductions);  //updating the file with new amount
							fclose(send);
						}
					}	
				}
				
				//clean up
				fclose(check);
		   } else {
				printf("Invalid input\n");
				return -1;
		   }
		   break;  
	   	case 2: //SEND CASH TO ANOTHER NUMBER
			printf("Enter the phone number: ");	
			char phone_number[BUFFER_SIZE];
			scanf("%s", phone_number);
			
	    	bool exists = validate(phone_number);
			if(exists) {
				exists = searchFirstField("user_data.txt", phone_number);
		    	if(exists) {
		    		int amount;
		    		printf("Enter the amount you want to send: ");
		    		scanf("%d", &amount);
		    		
		    		FILE *send = fopen("transactions.txt", "r");
		    		if(send == NULL) {
		    			printf("Error unable open file, because you do not have your saving yet\n");
		    			return -1;
					} else {
						char line[BUFFER_SIZE];
						const char *saving;
						const char *p_number;
						while(fgets(line, sizeof(line), send)) {
							p_number = strtok(line, ",");
							saving = strtok(NULL, "\n");
						}
						if(p_number == NULL && saving == NULL) {
							printf("You have no money in your saving in our transaction file\n");
							return -1;
						}
						int money = atoi(saving);
						int deductions = money - amount;  //deducted the money extracted from the files with the money the client enters
						if(deductions <= 0) {
							printf("Your saving are very low to share to other people\n");
							return -1;
						} else {
							/*[FUTURE UPDATE]
							ABLE TO MAKE MONEY WHEN THE CLIENT USES OUR SERVICES LIKE THIS
							SHOULD BE ABLE TO USE THE PASSWORD BEFORE THE USER SENDS MONEY TO OTHER CLIENTS OR THE AGENTS
							*/
							FILE *approveSend = fopen("trasanction.txt", "w");
							if(approveSend == NULL) {
								printf("Error unable to open file\n");
								return -1;
							}
							
							printf("Your balance is %d\n", deductions);
							fprintf(send, "FILE|transactions|%s,%dMK", p_number, deductions);  //updating the file with new amount
							fclose(approveSend);
							helperSendFileLines("transactions.txt");
							
							printf("The transactions you made to the other client is being sent successfull\n");
						}
					}
				}
			} 
			
			break;
	   	case 3:  /*[FUTURE UPDATE]
		   the user is able to view a variety of things like; user data file, transactions history and money balance*/
			printf("HERE YOU ARE ABLE TO VIEW YOUR PROFILE DETAILS:\n");
			printf("=========================================\n");
			FILE *view = fopen("user_data.txt", "r");
			if(view == NULL) {
				printf("Error unable to open file\n");
				return -1;	
			} else {		
				char line[BUFFER_SIZE];
				while(fgets(line, sizeof(line), view)) {
					const char *field1 = strtok(line, ",");
					const char *field2 = strtok(NULL, ",");
					const char *field3 = strtok(NULL, ",");
					const char *field4 = strtok(NULL, "\n");
					printf("PHONE NUMBER: %s\n", field1);
					printf("FIRST NAME: %s\n", field3);
					printf("LAST NAME: %s\n", field4);
				}
				printf("=========================================\n");	
			} 
	
			break;
	   	case 4: //RECEIVE MONEY FROM THE AGENT OR OTHER CLIENTS
			justConnect();
			
	    	break;
		case 5: //EXIT
			printf("Exiting the program.\n");
			
			break;
	   	default:
			printf("Invalid choice option.\n");
			
	    	break;
   }
   return true;
}
