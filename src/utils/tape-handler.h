#pragma once
#include "../consts.h"
#include "../models/block.h"
#include "../models/performance.h"
#include "../models/tape.h"
#include <stdbool.h>

// Initialize the tapes by creating its files.
void init_tapes(Tape *tapes, char const *filename);

// Rewinds the tapes files
void rewind_tapes(Tape *tapes);

// Closes the tapes files
void close_tapes(Tape *tapes);

// Reopens the tapes
void reopen_tapes(Tape *tapes, bool is_inverse);

// flushes all tapes
void flush_tapes(Tape *);

// Adds a new block to a tape
void add_block(Tape *tape, Block *block, Performance *perf);

// Gives the max block size in tapes
int block_size(Tape *, bool);
