#include "performance-handler.h"
#include "status-messages.h"

void print_performance(Performance perf) {
  printf("Execution time: %lfs\n", perf.execution_time);
  printf("Comparisons count: %zu\n", perf.comparisons_count);
  printf("Reads count: %zu\n", perf.reads_count);
  printf("Writes count: %zu\n", perf.writes_count);
}

void merge_performance(Performance *perf1, Performance *perf2,
                       Performance *result) {
  result->execution_time = perf1->execution_time + perf2->execution_time;
  result->comparisons_count =
      perf1->comparisons_count + perf2->comparisons_count;
  result->reads_count = perf1->reads_count + perf2->reads_count;
  result->writes_count = perf1->writes_count + perf2->writes_count;
}

void save_performance(const char *filename, const char *title,
                      Performance perf) {
  FILE *file = fopen(filename, "a");

  if (!file) {
    error_msg("Failed to open file.\n");
    return;
  }

  fprintf(file, "%s\n", title);
  fprintf(file, "Execution time: %lfs\n", perf.execution_time);
  fprintf(file, "Comparisons count: %zu\n", perf.comparisons_count);
  fprintf(file, "Reads count: %zu\n", perf.reads_count);
  fprintf(file, "Writes count: %zu\n\n", perf.writes_count);

  fclose(file);
}