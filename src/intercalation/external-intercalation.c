#include "external-intercalation.h"
#include "../intercalation/intercalation.h"
#include "../models/method.h"
#include "../sort/heap.h"
#include "../utils/status-messages.h"
#include "../utils/tape-handler.h"
#include <stdio.h>
#include <time.h>

void sort(char const *filename, Tape *tapes, Performance *perf) {
  FILE *file = fopen(filename, "rb+");

  if (!file) {
    error_msg("Failed to open file.\n");
    return;
  }

  Heap heap;
  heap.reg_count = 0;
  int tapes_count = 0;
  int registers_count = 0;

  fwrite(&registers_count, sizeof(int), 1, tapes[tapes_count].file);
  perf->writes_count += 1;
  tapes[tapes_count].block_size++;

  int index = 0;

  for (int i = 0; i < HALF_TAPES_SZ; i++) {
    if (feof(file))
      break;

    perf->reads_count += 1;
    fread(&heap.registers[index].reg, sizeof(Register), 1, file);
    heap.registers[i].tagged = false;
    heap.reg_count++;
    index++;
  }

  init_heap(&heap, perf);

  while (heap.reg_count > 0) {
    bool has_tagged_reg = true;

    for (int i = 0; i < heap.reg_count; i++) {
      if (heap.registers[i].tagged == false) {
        has_tagged_reg = false;
        break;
      }
    }

    if (has_tagged_reg) {
      for (int i = 0; i < heap.reg_count; i++) {
        heap.registers[i].tagged = false;
      }
      init_heap(&heap, perf);

      tapes_count++;
      tapes[tapes_count % HALF_TAPES_SZ].block_size += 1;
      registers_count = 0;

      fwrite(&registers_count, sizeof(int), 1,
             tapes[tapes_count % HALF_TAPES_SZ].file);
      perf->writes_count += 1;
    }

    fwrite(&heap.registers[0].reg, sizeof(Register), 1,
           tapes[tapes_count % HALF_TAPES_SZ].file);
    perf->writes_count += 1;
    registers_count++;

    fseek(tapes[tapes_count % HALF_TAPES_SZ].file,
          -(registers_count * sizeof(Register)) - sizeof(int), SEEK_END);

    fwrite(&registers_count, sizeof(int), 1,
           tapes[tapes_count % HALF_TAPES_SZ].file);
    perf->writes_count += 1;

    fseek(tapes[tapes_count % HALF_TAPES_SZ].file, 0, SEEK_END);

    double first_reg_grade = heap.registers[0].reg.grade;

    perf->reads_count += 1;
    if (!fread(&heap.registers[0].reg, sizeof(Register), 1, file)) {
      heap.registers[0] = heap.registers[heap.reg_count - 1];
      heap.reg_count--;
      rebuild_heap(heap.registers, 0, heap.reg_count, perf);
    } else {
      perf->comparisons_count += 1;
      if (heap.registers[0].reg.grade < first_reg_grade)
        heap.registers[0].tagged = true;

      rebuild_heap(heap.registers, 0, heap.reg_count, perf);
    }
  }

  fclose(file);
}

void external_intercalation(char const *filename, char const *out_filename,
                            Performance *sort_perf, Performance *file_perf) {
  Tape tapes[TAPES_SZ];
  init_tapes(tapes, "tapes/tape");

  clock_t start_clock = clock();
  sort(filename, tapes, sort_perf);
  clock_t end_clock = clock();

  sort_perf->execution_time =
      (double)(end_clock - start_clock) / CLOCKS_PER_SEC;

  start_clock = clock();
  balanced_intercalation(out_filename, tapes, file_perf);
  end_clock = clock();

  file_perf->execution_time =
      (double)(end_clock - start_clock) / CLOCKS_PER_SEC;
}