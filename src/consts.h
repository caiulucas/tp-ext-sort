#pragma once

#include <stdio.h>

#define AVAILABLE_TAPES 40
#define FILENAME_LENGTH 15
#define MAX_BLOCK_SIZE 20

#define CONTENT_MAX_SIZE 85

// Represents a method of sorting (internal intercalation, external
// intercalation or quick sort)
typedef enum Method {
  INTERNAL_INTERCALATION = 1,
  EXTERNAL_INTERCALATION = 2,
  QUICK_SORT = 3,
} Method;

// Represents the file situation (ascending, descending or random)
typedef enum Situation { ASC = 1, DESC = 2, RANDOM = 3 } Situation;

// Represents a register of a file
typedef struct Register {
  size_t id;
  double grade;
  char content[CONTENT_MAX_SIZE];
} Register;

// Represents a block of registers
typedef struct Block {
  Register *registers;
  size_t registers_count;
} Block;

// Represents the arguments of the program
typedef struct Input {
  size_t quantity;
  Method method;
  Situation situation;
} Input;

// Perfomance struct to measure reads, comparisons and execution time in a
// function
typedef struct Perfomance {
  size_t reads_count;
  size_t comparisons_count;
  double execution_time;
} Perfomance;