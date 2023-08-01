#include "status-messages.h"
#include <stdio.h>

void success_msg(char const *msg) {
  printf("[%sSUCCESS%s] %s", GREEN_COLOR, RESET_COLOR, msg);
}

void info_msg(char const *msg) {
  printf("[%sINFO%s] %s", BLUE_COLOR, RESET_COLOR, msg);
}

void error_msg(char const *msg) {
  printf("[%sERROR%s] %s", RED_COLOR, RESET_COLOR, msg);
}