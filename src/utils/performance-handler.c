#include "performance-handler.h"

void print_perfomance(Perfomance perfomance) {
  printf("Execution time: %lfs\n", perfomance.execution_time);
  printf("Comparisons count: %zu\n", perfomance.comparisons_count);
  printf("Reads count: %zu\n", perfomance.reads_count);
}