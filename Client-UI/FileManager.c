#include "FileManager.h"
#include "time.h"
#include <stdio.h>
#include <string.h>
#include "client.h"
#include <stdlib.h>  //using it for strtoll function
int handleMoney();
int handleAirtime();

//this handles the 
bool UtilitySendFileLines(const char *filename) {
	FILE *readFile = fopen(filename, "r");
	if(readFile == NULL) {
		system("CLS");
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
        printf("Your query file has being saved\n");
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
