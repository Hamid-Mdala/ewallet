#pragma once
#define BUFFER_SIZE 1024
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool helperSendFileLines(const char *filename);

bool register_user(const char *phone_number, const char *password, const char *first_name, const char *last_name);
 
bool authenticate(const char* filename, const char *phone_number, const char *password);

bool searchFirstField(const char *filename, const char *phone_number);

char *returnNumber();

bool handlingNotification
 (int *current_balance, char *flag);  //flag could be used to identify the type of notification the customer needs

int getAmountFromFiles(char *type_of_field);