#pragma once

#include "../consts.h"
#include "../models/perfomance.h"
#include "../models/register.h"

// Sort a list by merging each item. [O(n*log(n)]
void merge_sort(Register *registers, int left, int right, Performance *perf);