#include "FileManager.h"
#include "time.h"
#include <stdio.h>
#include <string.h>
#include "client.h"
#include <stdlib.h>  //using it for strtoll function
int handleMoney();
int handleAirtime();

//this handles the 
bool helperSendFileLines(const char *filename) {
	FILE *readFile = fopen(filename, "r");
	if(readFile == NULL) {
		printf("Error: Unable to open teh user details file\n");
		exit(1); 
	}
	char line[BUFFER_SIZE]; //extracted lines
	char sent_line[BUFFER_SIZE];
	while(fgets(line, sizeof(line), readFile)) {
		line[strcspn(line, "\r\n")] = '\0';  /*removing the new line character from the buffer*/
		strcpy(sent_line, line); //copy and send the line that has no new line character 
	}
	//the agent must recieve the file content set & sending details that don't have newline character 
	sendFileLines(sent_line);
	return true;
}

bool register_user
 (const char *phone_number, const char *password, const char *first_name, const char *last_name) {
    FILE* Fptr = fopen("user_data.txt", "a");
    if (Fptr == NULL) {
        printf("Unable to add details in the user data file\n");
        return false;
    }
    else {
        fprintf
        (Fptr, "%s,%s,%s,%s\n", phone_number, password, first_name, last_name);
        //once the agent make his file he doesn't need to make other user data file, only he can receive the user data files 
        //with its details and could only view the desired needs
		fclose(Fptr);
		printf("User data file saved\n");
    }
    FILE* Fptr2 = fopen("Query.txt", "a");
    if (Fptr2 == NULL) {
        printf("Error: Unable to open the Query file\n");
        return false;
    }
    else {
        fprintf(Fptr2, "%s,%s,%s,%s\n", phone_number, password, first_name, last_name);
        fclose(Fptr2);
        printf("Your query file saved\n");
    }
	return true;
}

//seearches for the phone number which is on the first field
bool searchFirstField(const char *filename, const char *phone_number) {
	FILE* Fptr = fopen(filename, "r");
	if (Fptr == NULL) {
		printf("Error: Unable to open the file %s", filename);
		exit(1);
	} else {
		char line[BUFFER_SIZE];
		while (fgets(line, sizeof(line), Fptr)) {
			const char *extracted_phone_number = strtok(line, ",");
			if(strcmp(extracted_phone_number, phone_number) == 0) {
				return true;
			}
		}
		 
		fclose(Fptr);
		return false;
	} 
	
	return true;
}

char *returnNumber() {
	char *number;
	FILE* Fptr = fopen("Query.txt", "r");
	if (Fptr == NULL) {
		printf("Error: Unable to open the file");
		exit(1);
	} else {
		char line[BUFFER_SIZE];
		while (fgets(line, sizeof(line), Fptr)) {
			//e.g 0994600600,password,HASSAN,MDALA
			number = strtok(line, ",");
			return number;  //the file just has a single line
		}
	}
	
	fclose(Fptr);
	return false;
}

//int getAmountFromFiles(char *type_of_field) {  
//    char line[BUFFER_SIZE];
//	if(strcmp(type_of_field, "money") == 0) {    
//		handleMoney();
//	} else if(strcmp(type_of_field, "airtime") == 0) {
//		handleAirtime();
//	} return 0;
//}
bool handlingNotification(int *current_balance, char *flag) {
//    const map_element_t result;
//    char* username = result.value->details_field1;
//    FILE* Fptr = fopen("notification_history.txt", "a");
//    if (Fptr == NULL) {
//        printf("Error: Unable to open the notification file\n");
//        return false;
//    }
//    else {
//        FILE* fptr = fopen("notification_of_purchased_bundles.txt", "w");
//        FILE* otherFptr = fopen("notification_for_deduction_of_money", "w");
//        if (*choicePtr1 == 1 && *choicePtr2 == 1 && *choicePtr3 == 1 && *choicePtr4 == 1) {
//            printf("Dear Customer, Your request is being processed. You will receive a confirmation message shortly");
//            fprintf(fptr, "You have successfully purchased 1GB of daily internet bundles");
//            fprintf(otherFptr, "Dear Customer, Your current balance is: %d\n"
//                "\t\tThe amount of money removed from your account is: %d", *current_balance, *deducted_amount);
//        }
//        else if (*choicePtr1 == 1 && *choicePtr2 == 1 && *choicePtr3 == 1 && *choicePtr4 == 1) {
//            printf("Dear Customer, Your request is beign processed. You will receive a cornrirmation message shortly");
//            fprintf(fptr, "You have successfully purchased 2GB of daily internet bundles");
//            fprintf(otherFptr, "Dear Customer, Your current balance is: %d\n"
//                "\t\tThe amount of money removed from your account is: %d", *current_balance, *deducted_amount);
//        }
//        else if (*choicePtr1 == 1 && *choicePtr2 == 1 && *choicePtr3 == 1 && *choicePtr4 == 1) {
//            printf("Dear Customer, Your request is beign processed. You will receive a cornrirmation message shortly");
//            fprintf(fptr, "You have successfully purchased 3GB of daily internet bundles");
//            fprintf(otherFptr, "Dear Customer, Your current balance is: %d\n"
//                "\t\tThe amount of money removed from your account is: %d", *current_balance, *deducted_amount);
//        }
//        else if (*choicePtr1 == 1 && *choicePtr2 == 1 && *choicePtr3 == 1 && *choicePtr4 == 2) {
//            printf("Dear Customer, Your request is beign processed. You will receive a cornrirmation message shortly");
//            fprintf(fptr, "You have successfully purchased 1GB of weekly internet bundles");
//            fprintf(otherFptr, "Dear Customer, Your current balance is: %d\n"
//                "\t\tThe amount of money removed from your account is: %d", *current_balance, *deducted_amount);
//        }
//        else if (*choicePtr1 == 1 && *choicePtr2 == 1 && *choicePtr3 == 1 && *choicePtr4 == 2) {
//            printf("Dear Customer, Your request is beign processed. You will receive a cornrirmation message shortly");
//            fprintf(fptr, "You have successfully purchased 1GB of monthly internet bundles");
//            fprintf(otherFptr, "Dear Customer, Your current balance is: %d\n"
//                "\t\tThe amount of money removed from your account is: %d", *current_balance, *deducted_amount);
//        }
//        else if (*choicePtr1 == 2) {}
//        char *getTime = findTime();
//        fprintf(Fptr, "You successfully purchased 1GB of daily internet bundles on %d\nYour current balance was: %d\n"
//            "\t\tThe amount removed that day from your account was: %d", *getTime, *current_balance, *deducted_amount);
//    }  return true;
}	
char line[BUFFER_SIZE];
//int handleMoney() {
//	const char *extracting_phone_number;
//	const char *sent_amount;
//	const char *money_balance;
//    FILE *transactionFile = fopen("transactions.txt", "r");
//	if(transactionFile == NULL) {
//	    printf("Error: Unable to open the transaction file");
//	    return -1;
//	}
//	memset(line, 0, sizeof(line));	//empty the line(buffer) inside the variable before we use it
//	while(fgets(line, sizeof(line), transactionFile)) {
//		existsing_phone_number = strtok(line, ",");
//		money_ = strtok(NULL, "\n");
//	}
//	fclose(transactionFile);
//	FILE* amountFile = fopen("amount_sheet.txt", "r+");  //r+ allows to read and write in append mode
//	if(amountFile == NULL) {
//		printf("Error: Unable to open the transaction file");
//		return -1;
//	}	
//	memset(line, 0, sizeof(line));	//empty the line(buffer) inside the variable before we use it
//	while(fgets(line, sizeof(line), amountFile)) {
//		saved_phone_number = strtok(line, ","); //extracted and store the variable pointer to the value
//		money_balance = strtok(NULL, "\n"); 	//extracted the money_balance and stored it to a pointer 
//	} //when the amount sheet is null then do as follows
//	if(line == NULL) {   //when the lines in the file is empty means that the user has no money in his amount sheet
//		fprintf(amountFile,"%s,%s", saved_phone_number, sent_amount);  //amount sheet will write the phone number and amount
//		fclose(amountFile);
//		printf("The money sheet has been made successfully."); 
//		return 0;
//	}
//	int amount = atoi(sent_amount);  
//	int balance = atoi(money_balance);
//	balance += amount;
//	fprintf(amountFile,"%s,%dMK", saved_phone_number, balance);
//	fclose(amountFile);
//	printf("Your money sheet has been updated successfully.");
//	return 0;
//}
//int handleAirtime() {
//	char *saved_phone_number;
//	char *sent_airtime;
//	char *airtime_balance;
//	char line[BUFFER_SIZE];
//	//handle airtime part
//	FILE *airtimeTransactions = fopen("airtime_Transactions.txt", "r");
//	if(airtimeTransactions == NULL) {
//		printf("Error: Unable to open the airtime transactions text file");
//		return -1;
//	}
//	memset(line, 0, sizeof(line));
//	while(fgets(line, sizeof(line), airtimeTransactions)) {
//		saved_phone_number = strtok(line, ",");
//		sent_airtime = strtok(NULL, "\n"); 
//	}
//	fclose(airtimeTransactions);
//	FILE *airtimeFile = fopen("airtime_sheet.txt", "r+");
//	if(airtimeFile == NULL) {
//		printf("Error: Unable to open the airtime sheet text file\n");
//		return -1;
//	}
//	memset(line, 0, sizeof(line));
//	while(fgets(line, sizeof(line), airtimeFile)) {
//		saved_phone_number = strtok(line, ",");
//		airtime_balance = strtok(NULL, "\n");
//	}
//	if(line == NULL) {
//		fprintf(airtimeFile, "%s,%s", saved_phone_number, sent_airtime);
//		fclose(airtimeFile);
//		printf("The airtime sheet has been made successfully\n");
//		return 0;
//	}
//	int amount = atoi(sent_airtime);
//	int balance = atoi(airtime_balance);
//	balance += amount;
//	fprintf(airtimeFile, "%s,%d", saved_phone_number, balance);
//	fclose(airtimeFile);
//	printf("Your airtime sheet has been updated successfully\n");
//	return 0;	
//}
bool rename_transmissions(const char *key, const char *value) {
	const char *description;
	const char *phone_number;
	char line[BUFFER_SIZE];
	FILE *transmissions = fopen("transmissions.txt", "r");
	if(transmissions == NULL) {
		printf("Error: Unable to open the transmissions\n");
		return -1;
	}
	while(fgets(line,sizeof(line), transmissions)) {
		description = strtok(line, ",");
		phone_number = strtok(NULL, ",");
		if(strcmp(key, description) == 0) {
			if(strcmp(value, phone_number) == 0) {
				if(strcmp(key, "transactions") == 0) {
					const char *OldFile = "transmissions.txt";
					const char *NewFile = "transactions.txt";
					if(rename(OldFile, NewFile) == 0) {
						printf("The transmissions file has been renamed to transactions file successfully\n");
					} else {
						perror("Error: Uanble to rename the transmissions file to transactions\n");
					}
				} else if(strcmp(key, "user_details")) {
					//const char *
					const char *OldFile = "transmissions.txt";
					const char *NewFile = "user_details.txt";
					if(rename(OldFile,NewFile) == 0) {
						printf("The transmissions file has been rename to user details file successfully\n");
					} else {
						perror("Error: Unable to rename the transmissions file to user details\n");
					}
				} else if(strcmp(key, "sports_events") == 0) {
					
				} else if(strcmp(key, "interesting_topic") == 0) {
					
				} else if(strcmp(key, "lectures_on_religion") == 0) {
					
				} else if(strcmp(key, "life_hack") == 0) {
					
				} else {
					
				}
			} else {
				printf("This transaction sent is not for your number, please wait");
			}
		} else {
			printf("The description variable is not a valid value passed");
		}
	}
}
