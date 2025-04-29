#pragma once

#include <winsock2.h>

//#pragma comment(lib, "ws2_32.lib")  // Auto-linking (optional but helpful)

int agent_send(char *sent_string, int *sent_integer, char *flag);