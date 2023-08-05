#pragma once
#include "../consts.h"
#include <stdio.h>
#include <stdlib.h>

#define AREA_SZ 100

typedef unsigned int ptr_t;

typedef struct Area {
  Register registers[AREA_SZ];
  size_t reg_count;
} Area;

void ext_quick_sort(char const *filename, size_t size, Performance *perf);