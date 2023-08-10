
#include "intercalation.h"
#include "../consts.h"
#include "../models/block.h"
#include "../sort/merge-sort.h"
#include "../utils/status-messages.h"
#include "../utils/tape-handler.h"
#include <time.h>

bool should_intercalate(Tape *tapes, bool is_intercalated) {
  int blocks_size = 0;
  int index = 0;

  if (is_intercalated)
    index = HALF_TAPES_SZ;

  for (int i = index; i < index + (HALF_TAPES_SZ); i++) {
    blocks_size += tapes[i].block_size;

    if (blocks_size > 1)
      return true;
  }

  return false;
}

int find_min_register(Register *reg, Register *registers, int *reg_indexes,
                      Performance *perf) {
  int index = 0;

  for (int i = 0; i < TAPES_SZ; i++) {
    if (reg_indexes[i] > 0) {
      index = i;
      break;
    }
  }

  for (int i = index + 1; i < BLOCK_SZ; i++) {
    perf->comparisons_count += 1;
    if (registers[i].grade < registers[index].grade && reg_indexes[i] > 0)
      index = i;
  }

  *reg = registers[index];
  return index;
}

bool has_valid_register(int *reg_indexes) {
  for (int i = 0; i < HALF_TAPES_SZ; i++) {
    if (reg_indexes[i] == 1)
      return true;
  }

  return false;
}

bool intercalate(Tape *tapes, int block_index, bool is_intercalated,
                 Performance *perf) {
  int reg_indexes[HALF_TAPES_SZ];
  int aux_indexes[HALF_TAPES_SZ];

  for (int i = 0; i < HALF_TAPES_SZ; i++) {
    reg_indexes[i] = 0;
  }

  Register registers[BLOCK_SZ];

  int write_tape_index = (((block_index - 1) % (HALF_TAPES_SZ)));

  if (!is_intercalated)
    write_tape_index += HALF_TAPES_SZ;

  int index_input_tapes = 0;

  if (is_intercalated)
    index_input_tapes = HALF_TAPES_SZ;

  for (int i = index_input_tapes; i < index_input_tapes + (HALF_TAPES_SZ);
       i++) {
    if (tapes[i].block_size < block_index) {
      aux_indexes[i] = 0;
      continue;
    }

    perf->reads_count += 1;
    fread(&(aux_indexes[i % (HALF_TAPES_SZ)]), sizeof(int), 1, tapes[i].file);
  }

  int next_index = 0;

  for (int i = 0; i < (HALF_TAPES_SZ); i++) {
    next_index += aux_indexes[i];
  }

  tapes[write_tape_index].block_size++;

  fwrite(&next_index, sizeof(int), 1, tapes[write_tape_index].file);
  perf->writes_count += 1;

  for (int i = index_input_tapes; i < (int)index_input_tapes + (HALF_TAPES_SZ);
       i++) {
    if (aux_indexes[i % (HALF_TAPES_SZ)] > 0) {
      fread(&registers[i % (HALF_TAPES_SZ)], sizeof(Register), 1,
            tapes[i].file);
      perf->reads_count += 1;
      reg_indexes[i % (HALF_TAPES_SZ)] = 1;
      aux_indexes[i % (HALF_TAPES_SZ)]--;
    }
  }

  while (has_valid_register(reg_indexes)) {
    int disp = 0;

    if (is_intercalated)
      disp = HALF_TAPES_SZ;

    Register reg = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    int index = find_min_register(&reg, registers, reg_indexes, perf);

    fwrite(&reg, sizeof(Register), 1, tapes[write_tape_index].file);
    perf->writes_count += 1;
    reg_indexes[index] = 0;

    if (aux_indexes[index] > 0) {
      fread(&registers[index], sizeof(Register), 1, tapes[index + disp].file);
      perf->reads_count += 1;
      reg_indexes[index] = 1;
      aux_indexes[index]--;
    }
  }

  flush_tapes(tapes);
  return true;
}

bool balanced_intercalation(char const *out_filename, Tape *tapes,
                            Performance *perf) {
  bool is_intercalated = false;

  while (should_intercalate(tapes, is_intercalated)) {
    flush_tapes(tapes);
    rewind_tapes(tapes);

    reopen_tapes(tapes, is_intercalated);

    int blocks_size = block_size(tapes, is_intercalated);

    for (int i = 0; i < blocks_size; i++) {
      intercalate(tapes, i + 1, is_intercalated, perf);
      flush_tapes(tapes);
    }

    is_intercalated = !is_intercalated;
  }

  flush_tapes(tapes);
  rewind_tapes(tapes);

  int tape_index = 0;

  if (is_intercalated)
    tape_index += HALF_TAPES_SZ;

  FILE *txt_file = fopen(out_filename, "w");

  for (int i = tape_index; i < (tape_index + HALF_TAPES_SZ); i++) {
    if (tapes[i].block_size > 0) {
      Register reg;

      rewind(tapes[i].file);

      int trash;
      fread(&trash, sizeof(int), 1, tapes[i].file);

      while (fread(&reg, sizeof(Register), 1, tapes[i].file)) {
        fprintf(txt_file, "%ld %f %s\n", reg.id, reg.grade, reg.content);
      }
    }
  }

  fclose(txt_file);
  close_tapes(tapes);
  return true;
}
