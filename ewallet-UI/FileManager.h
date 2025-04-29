#pragma once
#define BUFFER_SIZE 1024
#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
const struct  map_t *user_details;

bool register_user
 (char *username, char *password, char *id, char *first_name,
 char *last_name, char *gender, char *dob, char *phone_number);
 
bool counting_notification();

void checkResult(const map_element_t result);

bool loading_in_map(map_t *user_details);

bool authenticate_password(char *phone_number, char *password);

bool search_for_field(char *filename, char *phone_number);

bool handlingNotification
 (int *current_balance, char *flag);  //flag could be used to identify the type of notification the customer needs

int getAmountFromFiles(char *type_of_field);