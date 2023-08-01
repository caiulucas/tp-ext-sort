#pragma once

// Text color escape codes
#define RESET_COLOR "\033[0m"
#define RED_COLOR "\033[31m"
#define GREEN_COLOR "\033[32m"
#define BLUE_COLOR "\033[34m"

// Print a message in the following format: [SUCCESS] <message>
void success_msg(char const *msg);

// Print a message in the following format: [INFO] <message>
void info_msg(char const *msg);

// Print a message in the following format: [ERROR] <message>
void error_msg(char const *msg);