#pragma once
#include "../consts.h"
#include <stdio.h>
#include <stdlib.h>

#define AREA_SZ 100

typedef unsigned int ptr_t;

typedef struct Cell {
  Register reg;
  ptr_t prev;
  ptr_t next;
} Cell;

typedef struct Area {
  Cell cells[AREA_SZ];
  ptr_t cell_displacement;
  ptr_t first;
  ptr_t last;
  size_t used_cells;
} Area;

void ext_quick_sort(char const *filename, size_t size);