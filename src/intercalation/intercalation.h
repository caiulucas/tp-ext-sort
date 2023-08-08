#pragma once

#include "../consts.h"
#include "../models/method.h"
#include "../models/perfomance.h"
#include <stdbool.h>
#include <stdio.h>

typedef struct RegisterIndex {
  int index;
  bool is_valid;
} RegisterIndex;

void internal_intercalation(Method method, char const *filename,
                            Performance *file_perf, Performance *sort_perf);