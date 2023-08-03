#include "consts.h"
#include "sort/merge-sort.h"
#include "sort/quick-sort.h"
#include "sort/tapes.h"
#include "utils/file-handler.h"
#include "utils/performance-handler.h"
#include "utils/status-messages.h"
#include <stdio.h>
#include <stdlib.h>

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

  cp_file("tmp/provao.bin", "tmp/provao-cp.bin");
  FILE *file_1 = fopen("tmp/provao-cp.bin", "rb+");

  if (!file_1) {
    error_msg("Error opening file");
  }
  FILE *file_2 = fopen("tmp/provao-cp.bin", "rb+");

  if (!file_2) {
    error_msg("Error opening file");
  }
  FILE *file_3 = fopen("tmp/provao-cp.bin", "rb+");

  if (!file_3) {
    error_msg("Error opening file");
  }

  switch (input.method) {
  case INTERNAL_INTERCALATION:
    // TODO;
    break;
  case EXTERNAL_INTERCALATION:
    // TODO;
    break;
  case QUICK_SORT:
    ext_quick_sort("tmp/provao-cp.bin", input.quantity);
    bin_to_txt("tmp/provao-cp.bin", "out/provao-1.txt");
    bin_to_txt("tmp/provao.bin", "out/provao.txt");
    break;
  }

  return EXIT_SUCCESS;
}
