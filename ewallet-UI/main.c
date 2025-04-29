//prepoceser directives/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
//user made directives/libraries
#include "map.h"
#include "time.h"
#include "server.h"
#include "client.h"
#include "FileManager.h"
#include "Validations.h"
#include "Implementations.h"
#define BUFFER_SIZE 1024
#pragma comment(lib, "ws2_32.lib")  // Auto-linking (optional but helpful)

bool exists;
int choice;
char phone_number[10];
const char value[20] = "user";
char *make_search_query() {
	FILE *fptr = fopen("Query.txt", "r"); //mode r reads and writes content to the file without changing the existing content
    if (fptr == NULL) {
        printf("Error: Unable to open the Query file/n");
        exit(1);
    }
    else {
        char line[300];
        while (fgets(line, sizeof(line), fptr)) {
            char* field1 = strtok(line, ","); //this states: service_provider
            char* field2 = strtok(NULL, "\n"); //this states: phone number
            if (strcmp(field1, "service_provider") == 0) {
                return field2;   //this return the phone number 
            }
        } return false;
    }
}

void registration_of_user() {
	FILE *Fptr = fopen("user_data.txt", "a");
	if(Fptr == NULL) {
		printf("Error: Unable to open user data file.");
		exit(1);
	} else {
		//registration_form
	    char username[100];
	    printf("Enter username: ");
	    scanf("%s", username);
	    //its a char pointer and its beign used to point to the first value of the username
	    char *username_ptr = &username[100]; //usename starts with H for username Hamid 
	    //as for now im giving the referenced value into my functions 
	    exists = validateUsername(&username_ptr);
	    if (exists) {
	        char password[100];
	        printf("Enter password: ");
	        scanf("%s", password);
	        char *password_ptr = &password[100];
	        exists = validatePassword(&password_ptr);
	        if (exists) {
	            char id[100];
	            printf("Enter ID: ");
	            scanf("%s", id);
	        	char *id_ptr = &id[100];
	            exists = validateId(&id_ptr);
	            if (exists) {
	                char first_name[100];
	                printf("Enter First Name: ");
	                scanf("%s", first_name);
	                char *first_name_ptr = &first_name[100];
	                exists = validateFirstName(&first_name_ptr);
	                if (exists) {
	                    char last_name[100];
	                    printf("Enter Last Name: ");
	                    scanf("%s", last_name);
	                    char *last_name_ptr = &last_name[100];
	                    exists = validateLastName(&last_name_ptr);
	                    if (exists) {
	                        char gender[100];
	                        printf("Enter Gender: ");
	                        scanf("%s", gender);
	                        char *gender_ptr = &gender[100];
	                        exists = validateGender(&gender_ptr);
	                        if (exists) {
	                            char dob[100];
	                            printf("Enter Date Of Birth: ");
	                            scanf("%s", dob);
	                            char *dob_ptr = &dob[100];
	                            exists = validateDOB(&dob_ptr);
	                            if (exists) {
	                                printf("Enter Phone Number: ");
	                                scanf("%s", phone_number);
	                                char *phone_number_ptr = &phone_number[100];
	                                exists = validatePhone(phone_number_ptr);
	                                if (exists) {
	                                    exists = search_for_field("user_data.txt", phone_number_ptr);
	                                    if (exists) {
	                                        printf("The phone doesn't belong to you, someone is having these number in our records.\n");
	                                    }
	                                    else {
	                                        register_user
											(username_ptr, password_ptr, id_ptr, 
											first_name_ptr, last_name_ptr, gender_ptr, dob_ptr, phone_number_ptr);
	                                    }
	                                }
	                                else {
	                                    printf("Error: Invalid phone number.\n");
	                                }
	                            }
	                            else {
	                                printf("Error: Invalid date of birth.\n");
	                            }
	                        }
	                        else {
	                            printf("Error: Invalid gender.\n");
	                        }
	                    }
	                    else {
	                        printf("Error: Invalid last name.\n");
	                    }
	                }
	                else {
	                    printf("Error: Invalid first name.\n");
	                }
	            }
	            else {
	                printf("Error: Invalid ID.\n");
	            }
	        }
	        else {
	            printf("Error: Invalid password.\n");
	        }
	    }
	    else {
	        printf("Error: Invalid username.\n");
	    }
	}
   
}
static bool enter_choice() {
    printf("Enter your choice option: ");
    scanf("%d", &choice);
    system("CLS");
    return customerAfterNavigationDashboard(choice);
}

static bool beforeNavigationDashboard() {
	FILE *fptr1 = fopen("Query.txt", "a");
	if(fptr1 == NULL) {
		printf("Error: Unable to open the Query file/n");
		exit(1);
	} else {
		exists = make_search_query();
		if(exists) {
			char *digits_on_phone_number = make_search_query();
			if(strlen(digits_on_phone_number) == 10) {
				printf("1. BUY BUNDLES\n");
		        //feature added later: 2.PAY BILLS like electricity, water, house rent, school fees & MIFI internet bills
		        printf("2. SEND CASH TO ANOTHER NUMBER\n");  	//send cash to another client
		        printf("3. VIEW ACCOUNT DETAILS\n");   			//details which you registered with
		        printf("4. VIEW INTO UPCOMING EVENTS\n");  			//competitions(sports,educational), lectures & spiritual events  
		        printf("5. NOTIFICATION()\n");        //view the bundles history that you bought...
		        printf("6. CONTRIBUTE TO CHARITABLE ORGANIZATION/WORLDLIFE ORGANIZATION\n");  
				//Assume we have a link with MALAWI WORLD VISION, HABITANT, & WATERAID MALAWI 
				printf("7. EXIT\n");
			 	printf("Enter your choice option: ");
    			scanf("%d", &choice);
    			system("CLS");
    			return customerAfterNavigationDashboard(choice);
			} else if(strlen(digits_on_phone_number) == 7) {
				printf("1. SERVICES\n");//send money to the customers
				printf("2. PUBLISH EVENTS THAT ARE HAPPENING\n");  //competitions(sports, educational), lectures, & spiritual events
				printf("3. EXIT\n");
				printf("Enter your choice option: ");
    			scanf("%d", &choice);
    			system("CLS");
    			return agentAfterNavigationDashboard(choice);
				//if the agent publishes more the will have more money
			} else {
				printf("Please enter your valid phone number giving by the company\n");
				printf("If you are unable to reset your registration then contant the nearest branch company brand\n");
			}
	    } else {
	        registration_of_user();
	    } 
	} return true;
}
int main(int argc, char** argv[]) {
    printf("Welcome to reliable internet service provider.\n");
    map_t* user_details = map_new();
    map_ctr(user_details);
    beforeNavigationDashboard();
    map_dtr(user_details);
    return 0;
}
/*what when I search for the number in the user data file
and if the number exists then able to view the dashboard
else the number has to register
*/
