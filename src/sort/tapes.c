#include "tapes.h"
#include "../consts.h"
#include <stdio.h>

void init_tapes(Tape *tapes) {
  for (size_t i = 0; i < AVAILABLE_TAPES; i++) {
    char filename[FILENAME_MAX];

    sprintf(filename, "tape_%zu", i);

    tapes[i].file = fopen(filename, "w+");
    tapes[i].type = i < 20 ? INPUT : OUTPUT;
  }
}

void add_block_to_tape(Tape *tape, Block *block) {
  fwrite(block->registers, sizeof(Register), block->registers_count,
         tape->file);
  tape->block_size++;
}