#pragma once
#include <stdio.h>

typedef enum TapeType { INPUT, OUTPUT } TapeType;

typedef struct Tape {
  TapeType type;
  FILE *file;
  int block_size;
} Tape;