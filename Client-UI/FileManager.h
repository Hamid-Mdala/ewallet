#pragma once
#define BUFFER_SIZE 1024
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool UtilitySendFileLines(const char *filename);

bool register_user(const char *phone_number, const char *password, const char *first_name, const char *last_name);
 
bool authenticate(const char* filename, const char *phone_number, const char *password);

bool searchFirstField(const char *filename, const char *phone_number);

char *returnNumber();
