#pragma once
#include "../consts.h"
#include "../models/performance.h"
#include "../models/register.h"
#include "../models/tape.h"
#include <stdbool.h>

typedef struct HeapRegister {
  bool tagged;
  Register reg;
} HeapRegister;

typedef struct Heap {
  HeapRegister registers[HALF_TAPES_SZ];
  int reg_count;
} Heap;

void init_heap(Heap *heap, Performance *perf);

void rebuild_heap(HeapRegister *heap_registers, int left, int right,
                  Performance *perf);
