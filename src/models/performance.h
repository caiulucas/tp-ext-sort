#pragma once
#include <stdio.h>

// Performance struct to measure reads, comparisons and execution time in a
// function
typedef struct Performance {
  size_t reads_count;
  size_t writes_count;
  size_t comparisons_count;
  double execution_time;
} Performance;