#include "../consts.h"
#include <stdio.h>

#pragma once

typedef enum TapeType { INPUT, OUTPUT } TapeType;

typedef struct Tape {
  TapeType type;
  FILE *file;
  size_t block_size;
} Tape;

void add_block_to_tape(Tape *tape, Block *block);
