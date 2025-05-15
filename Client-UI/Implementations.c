#include "Implementations.h"
#include "Validations.h"
#include "client.h"
#include "FileManager.h"

bool agentAfterNavigationDashboard(int choice) {
	switch(choice) {
		case 1:
			printf("HERE YOU ARE ABLE TO SEND MONEY TO THE CLIENT WHO WANT TO DEPOSIT MONEY INTO THEIR ACCOUNTS\n");
			printf("=============================================================================================\n");
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
					printf("The phone number does not exists in our credentials\n");
				}
			} else {
				printf("The phone number is invalid\n");
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
			printf("HERE AFTER FULL INTERGRATION YOU WILL PURCHASE TOKENS USED FOR INTERNET ACESS, VOICE CALLS & SMS MESSAGING\n");
			printf("===========================================================================================================\n");
			printf("1. Purchase 200 tokens with MK200\n");
			printf("2. Purchase 300 tokens with MK300\n");
			printf("Enter your choice: ");
			scanf("%d", &choice);
			system("CLS");
			if(choice == 1) {
		   	
				int amount;
				printf("Enter the amount: ");
				scanf("%d", &amount);
			
				FILE *check = fopen("transactions.txt", "r");
			    if(check == NULL) {
			    	system("CLS");
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
						system("CLS");
						printf("You have no money in your trasaction file\n");
						return -1;
					}
					
					int money = atoi(saving);
					int deductions = money - amount;  //deducted the money extracted from the files with the money the client enters
					
					if(money >= 200) {  //lets say to buy the 200 tokens your saving have to be more than 200 only
						/*[FUTURE UPDATE]
						ABLE TO MAKE MONEY WHEN THE CLIENT USES OUR SERVICES LIKE THIS
						SHOULD BE ABLE TO USE THE PASSWORD BEFORE THE USER SENDS MONEY TO OTHER CLIENTS OR THE AGENTS
						*/
						FILE *send = fopen("transactions.txt", "w");
						if(send == NULL) {
							system("CLS");
							printf("Error unable open file\n");
							return -1;
						} else {
							system("CLS");
							if(deductions <= 0) {  //lets say when we deduct we have only less than or equal to 0
								printf("Your saving are very low to buy tokens\n");
								return -1;
							}
							system("CLS");	 
							printf("You have successfully bought the 200 tokens at %dMK\n", amount);
							printf("Your balance is %d\n", deductions);
							fprintf(send, "FILE|transactions|%s,%dMK", p_number, deductions);  //updating the file with new amount
							fclose(send);
						}
					
					} else {   //lets say we have 50MK as the money its less than the requirement to buy the 200 tokens at 200MK
						printf("Insufficient funds.sorry you can not purchase 200 tokens for 200MK becuase your saving are %dMK\n", money);
					}
				}
				
				printf("===========================================================================================================\n");
				//clean up
				fclose(check);	
		   } else if(choice == 2) {
		   	
				int amount;
				printf("Enter the amount: ");
				scanf("%d", &amount);
			
				FILE *check = fopen("transactions.txt", "r");
		    	if(check == NULL) {
		    		system("CLS");
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
						system("CLS");
						printf("You have no money in your trasaction file\n");
						return -1;
					}
				
					int money = atoi(saving);
					int deductions = money - amount;  //deducted the money extracted from the files with the money the client enters
				
					if(money >= 300) {  //lets say to buy the 300 tokens your saving have to be more than OR equal to 300 only
						/*[FUTURE UPDATE]
						ABLE TO MAKE MONEY WHEN THE CLIENT USES OUR SERVICES LIKE THIS
						SHOULD BE ABLE TO USE THE PASSWORD BEFORE THE USER SENDS MONEY TO OTHER CLIENTS OR THE AGENTS
						*/
						FILE *send = fopen("transactions.txt", "w");
						if(send == NULL) {
							system("CLS");
							printf("Error unable open file\n");
							return -1;
						} else {
							if(deductions <= 0) {  //lets say when we deduct we have only less than or equal to 0
								system("CLS");
								printf("Your saving are very low to buy tokens\n");
								exit(1);  //make it to exit..quit the terminal
							}
							system("CLS");
							printf("You have successfully bought the 200 tokens at %dMK\n", amount);
							printf("Your balance is %d\n", deductions);
							fprintf(send, "FILE|transactions|%s,%dMK", p_number, deductions);  //updating the file with new amount
							fclose(send);
						}
					
					} else {  //lets say we have 200MK as the money its less than the requirement to buy the 300 tokens at 300MK
						printf("Insufficient funds.sorry you can not purchase 300 tokens for 300MK becuase your saving are %dMK\n", money);
					}
				}
				
				printf("===========================================================================================================\n");
				//clean up
				fclose(check);
		   } else {
				printf("Invalid input\n");
				return -1;
		   }
		   break;  
	   	case 2: //SEND CASH TO ANOTHER NUMBER
			printf("HERE YOU WILL BE ABLE TO SEND CASH TO OTHER USERS'\n");
			printf("=====================================================");
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
		    			system("CLS");
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
						if(p_number == NULL && saving == NULL) {  //implementation helping me to get the saving in the client whos sending the money
							system("CLS");
							printf("You have no money in your saving in our transaction file\n");
							return -1;
						}
						int money = atoi(saving);
						int deductions = money - amount;  //deducted the money extracted from the files with the money the client enters
						
						if(money >= amount) {  //lets say we have 90000 as the money and 500 as the amount
							/*[FUTURE UPDATE]
							ABLE TO MAKE MONEY WHEN THE CLIENT USES OUR SERVICES LIKE THIS
							SHOULD BE ABLE TO USE THE PASSWORD BEFORE THE USER SENDS MONEY TO OTHER CLIENTS OR THE AGENTS
							*/
							FILE *send = fopen("transactions.txt", "w");
							if(send == NULL) {
								system("CLS");
								printf("Error unable open file\n");
								return -1;
							} else {
								if(deductions <= 0) {  //lets say when we deduct we have only less than or equal to 0
									system("CLS");
									printf("Your saving are very low to buy tokens\n");
									return -1;
								}
								system("CLS");	 
								printf("You have successfully sent %d to the number: %s\n", amount, phone_number);
								printf("Your balance is %d\n", deductions);
								fprintf(send, "FILE|transactions|%s,%dMK", phone_number, deductions);  //updating the file with new amount
								fclose(send);
							}
						
						} else {  //lets say we have 500 as the money and 90000 as the amount
							printf("Insufficient funds....\n");
						}
					}
				} else {
					printf("The phone number does not exists in our credentials\n");
				}
			} else {
				printf("The phone number is invalid\n");
			}
			
			printf("====================================================="); 
			break;
	   	case 3:  /*[FUTURE UPDATE]
		   the user is able to view a variety of things like; user data file, transactions history and money balance*/
			printf("HERE YOU ARE ABLE TO VIEW YOUR PROFILE DETAILS:\n");
			printf("================================================\n");
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
				printf("================================================\n");	
			} 
			break;
	   	case 4: //RECEIVE MONEY FROM THE AGENT OR OTHER CLIENTS
			printf("HERE YOU WILL CONNECT TO THE SERVER INORDER TO RECIEVE MONEY FROM OTHER USERS'\n");
			printf("===============================================================================\n");	
			justConnect();
			printf("===============================================================================\n");	
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
