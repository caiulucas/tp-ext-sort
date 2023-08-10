
#include "internal-intercalation.h"
#include "../consts.h"
#include "../models/block.h"
#include "../sort/merge-sort.h"
#include "../utils/status-messages.h"
#include "../utils/tape-handler.h"
#include "intercalation.h"
#include <time.h>

void internal_sort(char const *filename, Tape *tapes, Performance *perf) {
  FILE *file = fopen(filename, "rb+");

  if (!file) {
    error_msg("Failed to open file.");
    return;
  }

  Block block;
  size_t tape_index = 0;

  perf->reads_count += 1;
  block.registers_count =
      fread(block.registers, sizeof(Register), BLOCK_SZ, file);

  merge_sort(block.registers, 0, block.registers_count - 1, perf);

  while (block.registers_count > 0) {
    perf->reads_count += 1;
    block.registers_count =
        fread(block.registers, sizeof(Register), BLOCK_SZ, file);

    merge_sort(block.registers, 0, block.registers_count - 1, perf);

    if (block.registers_count > 0) {
      add_block(&tapes[tape_index % HALF_TAPES_SZ], &block, perf);
      tape_index++;
    }
  }
}

void internal_intercalation(Method method, char const *filename,
                            char const *out_filename, Performance *sort_perf,
                            Performance *file_perf) {
  Tape tapes[TAPES_SZ];
  init_tapes(tapes, "tapes/tape");

  clock_t start_clock = clock();
  internal_sort(filename, tapes, sort_perf);
  clock_t end_clock = clock();

  sort_perf->execution_time =
      (double)(end_clock - start_clock) / CLOCKS_PER_SEC;

  start_clock = clock();
  balanced_intercalation(out_filename, tapes, file_perf);
  end_clock = clock();

  file_perf->execution_time =
      (double)(end_clock - start_clock) / CLOCKS_PER_SEC;
}