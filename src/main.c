#include "consts.h"
#include "sort/merge-sort.h"
#include "sort/quick-sort.h"
#include "sort/tapes.h"
#include "utils/file-handler.h"
#include "utils/performance-handler.h"
#include "utils/status-messages.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv) {
  // Tape tape;

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

  cp_file("tmp/provao.bin", "tmp/provao-cp.bin");

  switch (input.method) {
  case INTERNAL_INTERCALATION:
    // TODO;
    break;
  case EXTERNAL_INTERCALATION:
    // TODO;
    break;
  case QUICK_SORT:
    ext_quick_sort("tmp/provao-cp.bin", input.quantity, &sort_performance);
    bin_to_txt("tmp/provao-cp.bin", "out/provao-1.txt", input.quantity,
               &file_performance);
    break;
  }

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

  char filename[100];
  sprintf(filename, "docs/performance-%lu.txt", time(NULL));

  save_performance(filename, "Sorting performance:", sort_performance);
  save_performance(filename, "File handling:", file_performance);
  save_performance(filename, "Total performance:", total_perf);

  return EXIT_SUCCESS;
}
