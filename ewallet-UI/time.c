#include "time.h"
char* findTime() {
    struct tm ptr;
    time_t t;
    static char buffer[26]; // asctime_s requires a buffer of at least 26 characters
    t = time(NULL);
    localtime_s(&ptr, &t);
    asctime_s(buffer, sizeof(buffer), &ptr);
    return buffer;
}