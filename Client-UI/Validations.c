#include "Validations.h"
#include <stdio.h>
#include <string.h>
//so this is a pointer to the reference and then gets the pointer H is reference


bool validate(const char *entry) {
    if (strlen(entry) > 3) {
        return true;
    } else {
        return false;
    }
}