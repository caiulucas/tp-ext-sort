#include "merge-sort.h"
#include <stdlib.h>

// Merges a list of items
void merge(Register *registers, size_t left, size_t middle, size_t right) {
  size_t left_sz = middle - left + 1;
  size_t right_sz = right - middle;

  Register *left_registers = (Register *)malloc(sizeof(Register) * left_sz);
  Register *right_registers = (Register *)malloc(sizeof(Register) * right_sz);

  for (size_t i = 0; i < left_sz; i++)
    left_registers[i] = registers[i + left];

  for (size_t i = 0; i < right_sz; i++)
    right_registers[i] = registers[i + middle + 1];

  size_t left_index = 0;
  size_t right_index = 0;

  for (size_t i = left; i <= right; i++) {
    if (right_index == right_sz ||
        left_registers[left_index].id < right_registers[right_index].id) {
      registers[i] = left_registers[left_index];
      left_index++;
    } else {
      registers[i] = right_registers[right_index];
      right_index++;
    }
  }

  free(left_registers);
  free(right_registers);
}

void merge_sort(Register *registers, int left, int right) {
  if (left == right)
    return;

  int middle = (left + right) / 2;

  merge_sort(registers, left, middle);
  merge_sort(registers, middle + 1, right);

  merge(registers, left, middle, right);
}