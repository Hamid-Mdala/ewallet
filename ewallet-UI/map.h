#pragma once 
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
// Forward declaration of UserDetails struct
struct user_details {
    // declaring the variables inside struct
    char* details_field1; //username
    char* details_field2; //password
    char* details_field3; //ID
    char* details_field4; //first_name
    char* details_field5; //last_name
    char* details_field6; //sex
    char* details_field7; //date of birth
    char* details_field8; //phone number 
};

// Map Initialization in C
typedef struct map_t map_t;
map_t* map_new();
void map_ctr(map_t* obj);
void map_dtr(map_t* obj);

// Adding Map Elements in C
typedef struct map_element_t {
    int key;
    struct user_details* value;
    bool was_inserted;
} map_element_t;
map_element_t map_insert(map_t* obj, int key, struct user_details* value);

// Accessing Elements with at()
struct user_details* map_at(map_t* obj, int key);

// Checking Existence of an Element with count()
int map_count(map_t* obj, int key);

// Checking Existence of an Element with contains()
bool map_contains(map_t* obj, int key);

// Finding an Element by its key with find()
map_element_t map_find(map_t* obj, int key);

// Removing all Elements in a Map with clear()
void map_clear(map_t* obj);

// Removing one Element in a Map with erase()
void map_erase(map_t* obj, int key);