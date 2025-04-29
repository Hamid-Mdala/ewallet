#include "map.h"
#include <stdio.h>
// Map Initialization in C
typedef struct map_t {
    int* keys;
    struct user_details* values;
    int size;
} map_t;

map_t* map_new() {
    map_t* map = (map_t*)malloc(sizeof(map_t));
    if(map == NULL) {
        printf("The details failed to save inside the map.");
        exit(1);
    } 
    map->keys = NULL; 
    map->values = NULL;
    map->size = 0;
    return map;
}

void map_ctr(map_t* obj) {
    obj->keys = NULL;
    obj->values = NULL;
    obj->size = 0;
}

void map_dtr(map_t* obj) {
    if (obj != NULL) {
        free(obj->keys); //this is deleting the key
        free(obj->values); //this is deleting all the values
        free(obj);
    }
}
//the value is going to be inserted with the parameters that are declared inside it
map_element_t map_insert(map_t* obj, int key, struct user_details* value) {
    int existsAt = -1;
    for (int i = 0; i < obj->size; i++) {
        if (key == obj->keys[i]) {
            existsAt = i;
            break;
        }
    }

    int index = -1;
    map_element_t insertion_result = {0}; // Initialize the insertion_result
    if (existsAt >= 0) {
        index = existsAt;
        insertion_result.was_inserted = false;
    }
    else {
        index = obj->size;
        obj->size++;
        
        int* new_keys = (int*)realloc(obj->keys, obj->size * sizeof(int));
        if (new_keys == NULL) {
            printf("Failed to allocate memory for keys.\n");
            exit(1);
        }
        obj->keys = new_keys;

        struct user_details* new_values = (struct user_details*)realloc(obj->values, obj->size * sizeof(struct user_details));
        if (new_values == NULL) {
            printf("Failed to allocate memory for values.\n");
            exit(1);
        }
        obj->values = new_values;

        obj->keys[index] = key;
        insertion_result.was_inserted = true;
    }

    obj->values[index] = *value;
    insertion_result.key = obj->keys[index];
    insertion_result.value = &obj->values[index];

    return insertion_result;
}

// Accessing Elements with at()
struct user_details* map_at(map_t* obj, int key) {
    int existsAt = -1;
    for (int i = 0; i < obj->size; i++) {
        if (key == obj->keys[i]) {
            existsAt = i;
            break;
        }
    }

    assert(existsAt >= 0);
    return &obj->values[existsAt];
}

// Checking Existence of an Element with count()
static int key_exists(map_t* obj, const int key) {
    for (int i = 0; i < obj->size; i++) {
        if (key == obj->keys[i]) {
            return i;
        }
    }
    return -1;
}

int map_count(map_t* obj, const int key) {
    return key_exists(obj, key) != -1;
}

// Checking Existence of an Element with contains()
bool map_contains(map_t* obj, const int key) {
    return key_exists(obj, key) != -1;
}

map_element_t map_find(map_t* obj, const int key) {
    int existsAt = key_exists(obj, key);
    map_element_t map_element = {0}; // Initialize the map_element
    if (existsAt == -1) {
        map_element.was_inserted = false;
        map_element.key = key;
    }
    else {
        map_element.was_inserted = true;
        map_element.key = key;
        map_element.value = &obj->values[existsAt];
    }

    return map_element;
}

// Removing all Elements in a Map with clear()
void map_clear(map_t* obj) {
    free(obj->keys);
    free(obj->values);
    obj->keys = NULL;
    obj->values = NULL;
    obj->size = 0;
}

// Removing one Element in a Map with erase()
void map_erase(map_t* obj, const int key) {
    int existsAt = key_exists(obj, key);
    if (existsAt == -1) {
        return;
    }

    for (int i = existsAt; i < obj->size - 1; i++) {
        obj->keys[i] = obj->keys[i + 1];
        obj->values[i] = obj->values[i + 1];
    }

    obj->size--;
    int* new_keys = (int*)realloc(obj->keys, obj->size * sizeof(int));
    if (new_keys == NULL && obj->size > 0) {
        printf("Failed to allocate memory for keys.\n");
        exit(1);
    }
    obj->keys = new_keys;

    struct user_details* new_values = (struct user_details*)realloc(obj->values, obj->size * sizeof(struct user_details));
    if (new_values == NULL && obj->size > 0) {
        printf("Failed to allocate memory for values.\n");
        exit(1);
    }
    obj->values = new_values;
}
