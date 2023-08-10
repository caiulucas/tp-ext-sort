
#pragma once
#include "../models/method.h"
#include "../models/performance.h"
#include "../models/tape.h"

void internal_intercalation(char const *filename, char const *out_filename,
                            Performance *sort_perf, Performance *file_perf);
