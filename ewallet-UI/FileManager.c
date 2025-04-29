#include "FileManager.h"
#include "time.h"
#include <string.h>

int handleMoney();
int handleAirtime();

bool register_user
 (char *username, char *password, char *id, char *first_name,
 char *last_name, char *gender, char *dob, char *phone_number) {
    FILE* Fptr = fopen("user_data.txt", "a");
    if (Fptr == NULL) {
        printf("Unable to add details in the user data file\n");
        return false;
    }
    else {
        fprintf
        (Fptr, "%s,%s,%s,%s,%s,%s,%s,%s\n", 
		username, password, id, first_name, last_name, gender, dob, phone_number);
        fclose(Fptr);
        printf("Successfully added details in the user data file\n");
    }
    FILE* Fptr2 = fopen("Query.txt", "a");
    if (Fptr2 == NULL) {
        printf("Error: Unable to open the Query file\n");
        return false;
    }
    else {
        fprintf(Fptr2, "service_provider,%s\n", phone_number);
        fclose(Fptr2);
        printf("Query search file has beign added to your device successfully.");
    } return true;
}
//very important when we want to view our nofications count
bool counting_notification() {
	int notification_count = 0;
    FILE* Fptr = fopen("notification.txt", "r");
	if (Fptr == NULL) {
        printf("Error: Unable to open the notification file\n");
        return false;
    } else {
        char line[200];    
        while (fgets(line, 200, Fptr)) {
            notification_count++;
        } fclose(Fptr);
    } return notification_count;
}
//checks if the map loaded details
void checkResult(const map_element_t result) {
    if (result.was_inserted) {
        //    printf("Insertion of details successful! Key: %d, Value: '%s'\n", result.key, result.value->password);
    }
    else {
        printf("Insertion failed! Key %d already exists with Value: '%s'\n", result.key, result.value->details_field4);
    }
}
//as the name suggest it loads the details to a map
bool loading_in_map(map_t* user_details) {
    FILE* Fptr = fopen("user_data.txt", "r");
    if (Fptr == NULL) {
        printf("Error: Unable to open the user data file inorder to load the data in the map\n");
        return false;
    } else {
        char line[100];
        while (fgets(line, 100, Fptr)) {
            char *username = strtok(line, ",");
            char *password = strtok(line, ",");
            char *id = strtok(NULL, ",");
            char *first_name = strtok(NULL, ",");
            char *last_name = strtok(NULL, ",");
            char *sex = strtok(NULL, ",");
            char *gender = strtok(NULL, ",");
            char *phone_number = strtok(NULL, "\n");
            struct user_details* userDetails = malloc(sizeof(struct user_details));
            assert(username != NULL);
            userDetails->details_field1 = username;
            userDetails->details_field2 = password;
            userDetails->details_field3 = id;
            userDetails->details_field4 = first_name;
            userDetails->details_field5 = last_name;
            userDetails->details_field6 = sex;
            userDetails->details_field7 = gender;
            userDetails->details_field8 = phone_number;


            map_element_t result = map_insert(user_details, atoi(phone_number), userDetails);
            checkResult(result);
            fclose(Fptr);
        } 
    } return true;
}
//it authenticaties the password the user inputs while purchasing something
bool authenticate_password(char *phone_number, char *password) {
    map_t* user_details = map_new();
    map_ctr(user_details);
    if (loading_in_map(user_details)) {
        printf("The function load in map is loaded successfully\n");
    }
    else {
        printf("Error: Unable to successfully load the function load in map\n");
        return false;
    }
    map_element_t result = map_find(user_details, atoi(phone_number)); //atoi converts the stirng to imtegers
    if (result.value != NULL && strcmp(result.value->details_field2, password) && strcmp(result.value->details_field8, phone_number) == 0) {
        printf("Successfully authenticated both the user's password and phone number\n");
        return true;
    } else {
        printf("Oops unable to authenticate the user, because the credentials don't match our records\n");
        return false;
    } return true;
}
//seearches for the first field in a file
bool search_for_field(char *filename, char *phone_number) {
	FILE* Fptr = fopen("user_data.txt", "r");
	if (Fptr == NULL) {
		printf("Error: Unable to open the file %s", filename);
		exit(1);
	} else {
		char line[200];
		while (fgets(line, sizeof(line), Fptr)) {
			char *extracted_username = strtok(line, ",");
			char *extracted_password = strtok(NULL, ",");
			char *extracted_id = strtok(NULL, ",");
			char *extracted_first_name = strtok(NULL, ",");
			char *extracted_last_name = strtok(NULL, ",");
			char *extracted_gender = strtok(NULL, ",");
			char *extractd_dob = strtok(NULL, ",");
			char *extracted_number = strtok(NULL, "\n");
			if (strcmp(extracted_number, phone_number) == 0) {
				fclose(Fptr);
				return true;
			}
		} fclose(Fptr);
	} return false;
}
int getAmountFromFiles(char *type_of_field) {  
    char line[BUFFER_SIZE];
	if(strcmp(type_of_field, "money") == 0) {    
		handleMoney();
	} else if(strcmp(type_of_field, "airtime") == 0) {
		handleAirtime();
	} return 0;
}
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
int handleMoney() {
	char *saved_phone_number;
	char *sent_amount;
	char *money_balance;
	char line[BUFFER_SIZE];
    FILE *transactionFile = fopen("transactions.txt", "r");
	if(transactionFile == NULL) {
	    printf("Error: Unable to open the transaction file");
	    return -1;
	}	

	while(fgets(line, sizeof(line), transactionFile)) {
		*saved_phone_number = strtok(line, ",");
		*sent_amount = strtok(NULL, "\n");
	}
	fclose(transactionFile);
	FILE* amountFile = fopen("amount_sheet.txt", "r+");
	if(amountFile == NULL) {
		printf("Error: Unable to open the transaction file");
		return -1;
	}	
	memset(line, 0, sizeof(line));	//empty the line inside the file before we use it
	while(fgets(line, sizeof(line), amountFile)) {
		saved_phone_number = strtok(line, ","); //extracted and store the variable pointer to the value
		money_balance = strtok(NULL, "\n"); //extracted the money_balance and stored it to a pointer 
	} //when the amount sheet is null then do as follows
	if(line == NULL) {   //when the lines in the file is empty means that the user has no money in his amount sheet
		fprintf(amountFile,"%s,%s", saved_phone_number, sent_amount);  //amount sheet will write the phone number and amount
		fclose(amountFile);
		printf("The money sheet has been made successfully."); 
		return 0;
	}
	int amount = atoi(sent_amount);  
	int balance = atoi(money_balance);
	balance += amount;
	fprintf(amountFile,"%s,%dMK", saved_phone_number, balance);
	fclose(amountFile);
	printf("Your money sheet has been updated successfully.");
	return 0;
}
int handleAirtime() {
	char *saved_phone_number;
	char *sent_airtime;
	char *airtime_balance;
	char line[BUFFER_SIZE];
	//handle airtime part
	FILE *airtimeTransactions = fopen("airtime_Transactions.txt", "r");
	if(airtimeTransactions == NULL) {
		printf("Error: Unable to open the airtime transactions text file");
		return -1;
	}
	memset(line, 0, sizeof(line));
	while(fgets(line, sizeof(line), airtimeTransactions)) {
		saved_phone_number = strtok(line, ",");
		sent_airtime = strtok(NULL, "\n"); 
	}
	fclose(airtimeTransactions);
	FILE *airtimeFile = fopen("airtime_sheet.txt", "r+");
	if(airtimeFile == NULL) {
		printf("Error: Unable to open the airtime sheet text file\n");
		return -1;
	}
	memset(line, 0, sizeof(line));
	while(fgets(line, sizeof(line), airtimeFile)) {
		saved_phone_number = strtok(line, ",");
		airtime_balance = strtok(NULL, "\n");
	}
	if(line == NULL) {
		fprintf(airtimeFile, "%s,%s", saved_phone_number, sent_airtime);
		fclose(airtimeFile);
		printf("The airtime sheet has been made successfully\n");
		return 0;
	}
	int amount = atoi(sent_airtime);
	int balance = atoi(airtime_balance);
	balance += amount;
	fprintf(airtimeFile, "%s,%d", saved_phone_number, balance);
	fclose(airtimeFile);
	printf("Your airtime sheet has been updated successfully\n");
	return 0;	
}