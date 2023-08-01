#include "intercalation.h"
#include "../consts.h"
#include "../sort/merge-sort.h"
#include "../sort/tapes.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

Block init_block() {
  Block *block = (Block *)malloc(sizeof(Block));
  block->registers = (Register *)malloc(sizeof(Register) * MAX_BLOCK_SIZE);
  block->registers_count = 0;

  return *block;
}

void generate_blocks(Tape *tapes, char *filename) {
  FILE *file = fopen(filename, "rb+");

  Block current_block = init_block();

  if (!file) {
    printf("File %s not found\n", filename);
    return;
  }

  size_t tapes_count = 0;

  while (
      fread(current_block.registers, sizeof(Register), MAX_BLOCK_SIZE, file)) {
    current_block.registers_count = ftell(file) / sizeof(Register);

    merge_sort(current_block.registers, 0, current_block.registers_count - 1);

    if (current_block.registers_count > 0)
      add_block_to_tape(&tapes[tapes_count % 20], &current_block);
  }
}