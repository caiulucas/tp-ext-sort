#pragma once
#include "method.h"
#include "situation.h"
#include <stdio.h>

// Represents the arguments of the program
typedef struct Input {
  size_t quantity;
  Method method;
  Situation situation;
  bool should_print;
} Input;
