#include "tape-handler.h"
#include "../utils/status-messages.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init_tapes(Tape *tapes, char const *base_filename) {
  for (size_t i = 0; i < TAPES_SZ; i++) {
    tapes[i].block_size = 0;

    char tape_filename[100];
    sprintf(tape_filename, "%s-%zu.bin", base_filename, i);

    if (!(tapes[i].file = fopen(tape_filename, "wb+"))) {
      error_msg("Failed to create tape.\n");
    }
  }
}

void add_block(Tape *tape, Block *block, Performance *perf) {
  perf->writes_count += 2;
  fwrite(&block->registers_count, sizeof(int), 1, tape->file);
  fwrite(block->registers, sizeof(Register), block->registers_count,
         tape->file);

  tape->block_size += 1;
}

int block_size(Tape *tapes, bool is_intercalation) {
  int block_size = -1;
  int index = 0;

  if (is_intercalation)
    index = HALF_TAPES_SZ;

  for (int i = index; i < index + (HALF_TAPES_SZ); i++) {
    if (tapes[i].block_size > block_size)
      block_size = tapes[i].block_size;
  }

  return block_size;
}

void flush_tapes(Tape *tapes) {
  for (size_t i = 0; i < TAPES_SZ; i++) {
    fflush(tapes[i].file);
  }
}

void rewind_tapes(Tape *tapes) {
  for (size_t i = 0; i < TAPES_SZ; i++) {
    rewind(tapes[i].file);
  }
}

void reopen_tapes(Tape *tapes, bool is_inverse) {
  if (is_inverse) {
    for (int i = 0; i < HALF_TAPES_SZ; i++) {
      tapes[i].file = freopen(NULL, "wb+", tapes[i].file);
      tapes[i].block_size = 0;
    }
    return;
  }

  for (int i = HALF_TAPES_SZ; i < TAPES_SZ; i++) {
    tapes[i].file = freopen(NULL, "wb+", tapes[i].file);
    tapes[i].block_size = 0;
  }
}

void close_tapes(Tape *tapes) {
  for (int i = 0; i < TAPES_SZ; i++) {
    fclose(tapes[i].file);
  }
}
