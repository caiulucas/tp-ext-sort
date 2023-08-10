#include "heap.h"

bool compare(HeapRegister *reg_1, HeapRegister *reg_2, Performance *perf) {
  if (reg_1->tagged != reg_2->tagged)
    return reg_2->tagged;

  return reg_1->reg.grade > reg_2->reg.grade;
}

void rebuild_heap(HeapRegister *registers, int left, int right,
                  Performance *perf) {
  int i = left;
  int j = i * 2 + 1;
  HeapRegister aux = registers[i];

  while (j <= right) {
    if (j < right && compare(&registers[j], &registers[j + 1], perf))
      j = j + 1;

    if (!compare(&aux, &registers[j], perf))
      break;

    registers[i] = registers[j];
    i = j;
    j = i * 2 + 1;
  }

  registers[i] = aux;
}

void init_heap(Heap *heap, Performance *perf) {
  int left = (heap->reg_count / 2) - 1;

  while (left >= 0) {
    rebuild_heap(heap->registers, left, heap->reg_count - 1, perf);
    left--;
  }
}
