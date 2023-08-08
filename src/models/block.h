#pragma once
#include "register.h"

// Represents a block of registers
typedef struct Block {
  Register registers[20];
  int registers_count;
} Block;
