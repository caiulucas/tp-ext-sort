
#pragma once
#include "../models/method.h"
#include "../models/performance.h"
#include "../models/tape.h"

void internal_intercalation(Method method, char const *filename,
                            Performance *file_perf, Performance *sort_perf);
