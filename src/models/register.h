#pragma once
#include <stdio.h>

#define CONTENT_MAX_SIZE 85
// Represents a register of a file
typedef struct Register {
  size_t id;
  double grade;
  char content[CONTENT_MAX_SIZE];
} Register;