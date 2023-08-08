#pragma once

#include "../consts.h"
#include "../models/method.h"
#include "../models/performance.h"
#include "../models/tape.h"
#include <stdbool.h>
#include <stdio.h>

typedef struct RegisterIndex {
  int index;
  bool is_valid;
} RegisterIndex;

bool balanced_intercalation(char const *out_filename, Tape *tapes,
                            Performance *perf);