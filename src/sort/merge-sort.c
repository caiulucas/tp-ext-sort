
#include "merge-sort.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void merge(Register *registers, int left, int middle, int right,
           Performance *perf) {
  int left_size = (middle - left + 1);
  int right_size = (right - middle);

  Register *left_registers = (Register *)malloc(sizeof(Register) * left_size);
  Register *right_registers = (Register *)malloc(sizeof(Register) * right_size);

  for (int i = 0; i < left_size; i++) {
    left_registers[i] = registers[i + left];
  }

  for (int j = 0; j < right_size; j++) {
    right_registers[j] = registers[j + middle + 1];
  }

  int i = 0;
  int j = 0;

  for (int k = left; k <= right; k++) {
    perf->comparisons_count++;
    if (i == left_size)
      registers[k] = right_registers[j++];
    else if (j == right_size)
      registers[k] = left_registers[i++];
    else if (left_registers[i].grade <= right_registers[j].grade)
      registers[k] = left_registers[i++];
    else
      registers[k] = right_registers[j++];
  }

  free(left_registers);
  free(right_registers);
}

void merge_sort(Register *registers, int left, int right, Performance *perf) {
  if (left < right) {
    int middle = (right + left) / 2;
    merge_sort(registers, left, middle, perf);
    merge_sort(registers, middle + 1, right, perf);
    merge(registers, left, middle, right, perf);
  }
}
