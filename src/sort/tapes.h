#pragma once
#include "../consts.h"
#include "../models/block.h"
#include "../models/perfomance.h"
#include <stdbool.h>
#include <stdio.h>

#define BLOCK_SZ 20
#define TAPES_SZ 40
#define HALF_TAPES_SZ 20

typedef enum TapeType { INPUT, OUTPUT } TapeType;

typedef struct Tape {
  TapeType type;
  FILE *file;
  int block_size;
} Tape;

// Initialize the tapes by creating its files.
void init_tapes(Tape *tapes, char const *filename);

// Rewinds the tapes files
void rewind_tapes(Tape *tapes);

// Closes the tapes files
void close_tapes(Tape *tapes);

// Reopens the tapes
void reopen_tapes(Tape *tapes, bool is_inverse);

// Dá um fflush em todas as fitas
void flush_tapes(Tape *);

// Adds a new block to a tape
void add_block(Tape *tape, Block *block, Performance *perf);

// Gives the max block size in tapes
int block_size(Tape *, bool);
