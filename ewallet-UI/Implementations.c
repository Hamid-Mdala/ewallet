#include "Implementations.h"
#include "Validations.h"
#include "client.h"
#include "FileManager.h"
bool exists;

bool handleBundlePurchase(int choice); 
bool handleAirtimePurchase(int choice);
bool agentAfterNavigationDashboard(int choice) {
	switch(choice) {
		case 1:
			//SERVICE
			printf("Enter your phone number: ");
			char phone_number[BUFFER_SIZE];
			scanf("%s", phone_number);
			exists = validatePhone(phone_number);
			if(exists) {
				exists = search_for_field("user_data.txt", phone_number);//checking if the number is existing in the files
				if(exists) {
					int amount;
					printf("Enter the amount you want to send: ");
					scanf("%d", &amount);
					int *amount_ptr = &amount;
					char *phone_number_ptr = &phone_number[BUFFER_SIZE];
					agent_send(phone_number_ptr, amount_ptr, "transactions");	
				} else {
					printf("Unable to find the phone number in order to send money\n");
				}
			} else {
				printf("Error: the phone number is invalid\n");
			}
			break;
		case 2:
			//PUBLISH EVENTS THAT ARE HAPPENING
			//competitions(sports, educational), lectures, & spiritual events
			//if the agent publishes more the will have more money
			//have to send this files just like the transaction File
			printf("YOU CAN PUBLISH YOUR THOUGHTS OR IDEAS LIKE SPORTS NEWS, EDUCAIONAL TOPICS, SPIRITUAL EVENTS & LECTURES\n");
			printf("IF YOU PUBLISHES CONTENT YOU ARE GIVEN A BONUS FEE AND EVEN TOP UP MONEY\n");
			printf("1. SPORTS AND ENTERTAINMENT\n");
			printf("2. INTERESTING TOPICS\n");
			printf("3. LECTURES ON RELIGION");
			printf("4. LIFE HACKS\n");
			printf("5. EXIT\n");
			printf("ENTER YOUR CHOICE OF INTERESTION: ");
			scanf("%d", &choice);
			char thought[BUFFER_SIZE];
			if(choice == 1) {
				printf(":::::::SHARE YOUR THOUGHT:::::\n");
				scanf("%s", thought);
				int *choice_ptr = &choice;
				char *thought_ptr = &thought[BUFFER_SIZE];
				agent_send(thought_ptr, choice_ptr, "sports");
				//then send them us a client to the server
			} else if(choice == 2) {
				printf(":::::::SHARE YOUR THOUGHT::::::::\n");
				scanf("%s", thought);
				int *choice_ptr = &choice;
				char *thought_ptr = &thought[BUFFER_SIZE];
				agent_send(thought_ptr, choice_ptr, "interesting_topic");
				//then send them us a client to the server
			} else if(choice == 3) {
				printf(":::::::::SHARE YOUR THOUGHT::::::::::\n");
				scanf("%s", thought);
				int *choice_ptr = &choice;
				char *thought_ptr = &thought[BUFFER_SIZE];
				agent_send(thought_ptr, choice_ptr, "lecture_on_religion");
				//then send them us a client to the server
			} else if(choice == 4) {
				printf("::::::::::SHARE YOUR THOUGHT::::::::::\n");
				scanf("%s", thought);
				int *choice_ptr = &choice;
				char *thought_ptr = &thought[BUFFER_SIZE];
				agent_send(thought_ptr, choice_ptr, "life_hack");
				//then send them us a client to the server
			} else if(choice == 5) {
				exit(1);
			} else {
				printf("Error: Invalid option to share thought\n");
				return -1;
			}
			break;
		default: 
			printf("Error: Invalid choice option\n");
			return -1;
			break;
	}	
}

bool customerAfterNavigationDashboard(int choice) {
   switch (choice) {
	   	case 1: //BUY BUNDLES
	    	printf("You have selected to buy bundles.\n");
	    	printf("1. Buy bundles\n");
	    	printf("2. Buy airtime bundles\n");
	    	printf("Enter your choice option: ");
	    	scanf("%d", &choice);
	    	system("CLS");
	    	if (choice == 1) {
	        	return handleBundlePurchase(choice);
	    	}
	    	else if (choice == 2) {
	        	return handleAirtimePurchase(choice);
	    	}
	    	else {
	        	printf("Error: Invalid choice option.\n");
	        	return false;
	    	}
	   	case 2: 
			//SEND CASH TO ANOTHER NUMBER
			// Add your code here
	    	break;
	   	case 3: 
			//VIEW ACCOUNT DETAILS
	    	// Add your code here
			break;
	   	case 4: 
			//VIEW UPCOMING EVENTS
			// Add your code here
	    	break;
	    case 5:
	    	//NOTIFICATIONS
	    	//SHOW THE NOTIFICATIONS FOR THE BUNDLES BOUGHT
	    	break;
	   	case 6: 
			//CONTRIBUTE TO CHARITABLE ORGANIZATION/WORLDLIFE ORGANIZATION
			//Assume we have a link with MALAWI WORLD VISION, HABITANT, & WATERAID MALAWI
			break;
		case 7: 
			//EXIT
			printf("Exiting the program.\n");
			break;
	   	default:
			printf("Invalid choice option.\n");
	    	break;
   }
   return true;
}
bool handleBundlePurchase(int choice) {
   printf("You have selected to buy data bundles.\n");
   printf("1. For your number\n");
   printf("2. For another number\n");
   printf("Enter your choice option: ");
   if (scanf("%d", &choice) != 1) {
       printf("Error: Invalid input.\n");
       return false;
   }
   system("CLS");
   if (choice == 1) {
       // Handle purchase for your number
       //in the future if' its needed to beign implemented i could include "year", "month" and "daily" bundles
       printf("1. Purchase 3GB of daily bundles for MK 1500");
       printf("2. Purchase 2GB of daily bundles for MK 1000");
       printf("3. Purchase 1GB of daily bundles for MK 500");
       printf("Enter your choice option: ");
       if(scanf("%d", &choice) != 1) {
			printf("Error: Invalid input.\n");
	   } else {//c is not good in keeping a reference value, so what if we make a pointer variable to avoid copying data
	   		if(choice == 1);
	   }
   }
   else if (choice == 2) {
       char phone_number[BUFFER_SIZE];
       printf("Enter the phone number to make purchase bundles for someone: ");
       if (scanf("%s", phone_number) != 1) {
            printf("Error: Invalid input.\n");
            return false;
       }
       	system("CLS");
    	exists = search_for_field("user_data.txt", phone_number);
    	if (exists) {
           // Handle purchase for another number
           printf("1. ");
       }
       else {
           printf("Error: Phone number not found in user data.\n");
       }
   }
   else {
       printf("Error: Invalid choice option.\n");
       return false;
   } return true;
}

bool handleAirtimePurchase(int choice) {
   printf("1. For your number\n");
   printf("2. For another number\n");
   printf("Enter your choice option: ");
   int copychoice;
   if (scanf("%d", &copychoice) != 1) {
       printf("Error: Invalid input.\n");
       return false;
   }
   system("CLS");
   if (copychoice == 1) {
       int amount;
       printf("Enter the amount of airtime you want to buy: ");
       if (scanf("%d", &amount) != 1) {
           printf("Error: Invalid input.\n");
           return false;
       }
       	system("CLS");
        char password[100];
        printf("Enter your password: ");
        if (scanf("%s", password) != 1) {
        	printf("Error: Invalid input.\n");
        	return false;
        }
        system("CLS");
        //return handlePayment(copychoice, password, amount);
   	 	return true;
   }
   else if (copychoice == 2) {
       // Handle airtime purchase for another number
   }
   else {
       printf("Error: Invalid choice option.\n");
   } return true;
}
