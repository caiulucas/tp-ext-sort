#pragma once
#include "../consts.h"
#include "../models/perfomance.h"
#include "../models/register.h"
#include <stdio.h>
#include <stdlib.h>

#define AREA_SZ 100

typedef struct Area {
  size_t reg_count;
  Register registers[AREA_SZ];
} Area;

void ext_quick_sort(char const *filename, size_t size, Performance *perf);