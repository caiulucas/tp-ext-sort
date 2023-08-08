#include "consts.h"
#include "intercalation/external-intercalation.h"
#include "intercalation/intercalation.h"
#include "intercalation/internal-intercalation.h"
#include "models/input.h"
#include "sort/merge-sort.h"
#include "sort/quick-sort.h"
#include "utils/file-handler.h"
#include "utils/performance-handler.h"
#include "utils/status-messages.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char **argv) {
  if (argc < 4) {
    error_msg("Missing arguments\n");
    info_msg("Example: ./exe <method> <quantity> <situation>");
    return EXIT_FAILURE;
  }

  Input input;
  input.method = (Method)atoi(argv[1]);
  input.quantity = atoi(argv[2]);
  input.situation = (Situation)atoi(argv[3]);

  Performance sort_performance = {0, 0, 0, 0};
  Performance file_performance = {0, 0, 0, 0};

  char filename[100];
  switch (input.situation) {
  case ASC:
    strcpy(filename, "tmp/provao-asc.bin");
    break;
  case DESC:
    strcpy(filename, "tmp/provao-desc.bin");
    break;
  case RANDOM:
    strcpy(filename, "tmp/provao.bin");
    break;
  }

  switch (input.method) {
  case INTERNAL_INTERCALATION:
    cp_file_sized(filename, "tmp/provao-cp.bin", input.quantity);
    internal_intercalation(input.method, "tmp/provao-cp.bin", &sort_performance,
                           &file_performance);
    break;
  case EXTERNAL_INTERCALATION:
    cp_file_sized(filename, "tmp/provao-cp.bin", input.quantity);
    external_intercalation(input.method, "tmp/provao-cp.bin", &sort_performance,
                           &file_performance);
    break;
  case QUICK_SORT:
    cp_file(filename, "tmp/provao-cp.bin");
    ext_quick_sort("tmp/provao-cp.bin", input.quantity, &sort_performance);
    break;
  }

  bin_to_txt("tmp/provao-cp.bin", "out/provao-out.txt", input.quantity,
             &file_performance);

  printf("\n");
  info_msg("Sorting performance:\n");
  print_performance(sort_performance);

  printf("\n");
  info_msg("File handling performance:\n");
  print_performance(file_performance);

  Performance total_perf;
  merge_performance(&sort_performance, &file_performance, &total_perf);

  printf("\n");
  info_msg("Total performance:\n");
  print_performance(total_perf);

  strcpy(filename, "");
  sprintf(filename, "docs/performance-%lu.txt", time(NULL));

  save_performance(filename, "Sorting performance:", sort_performance);
  save_performance(filename, "File handling:", file_performance);
  save_performance(filename, "Total performance:", total_perf);

  return EXIT_SUCCESS;
}
