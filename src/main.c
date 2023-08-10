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
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char **argv) {
  if (argc < 4) {
    error_msg("Missing arguments\n");
    info_msg("Example: ./ordena <method> <quantity> <situation>");
    return EXIT_FAILURE;
  }

  Input input;
  input.should_print = false;
  input.method = (Method)atoi(argv[1]);
  input.quantity = atoi(argv[2]);
  input.situation = (Situation)atoi(argv[3]);

  if (argc >= 5 && !strcmp(argv[4], "-p")) {
    input.should_print = true;
  }

  Performance sort_performance = {0, 0, 0, 0};
  Performance file_performance = {0, 0, 0, 0};

  char filename[100];
  switch (input.situation) {
  case ASC:
    strcpy(filename, "tmp/provao_asc.bin");
    break;
  case DESC:
    strcpy(filename, "tmp/provao_desc.bin");
    break;
  case RANDOM:
    strcpy(filename, "tmp/provao.bin");
    break;
  }

  char out_filename[100];
  switch (input.method) {
  case INTERNAL_INTERCALATION:
    cp_file_sized(filename, "tmp/provao_cp.bin", input.quantity + BLOCK_SZ);
    if (input.should_print)
      print_bin("tmp/provao_cp.bin");
    sprintf(out_filename, "out/method-1/output-%lu.txt", time(NULL));
    internal_intercalation(input.method, "tmp/provao_cp.bin", out_filename,
                           &sort_performance, &file_performance);
    break;
  case EXTERNAL_INTERCALATION:
    cp_file_sized(filename, "tmp/provao_cp.bin", input.quantity);
    if (input.should_print)
      print_bin("tmp/provao_cp.bin");
    sprintf(out_filename, "out/method-2/output-%lu.txt", time(NULL));
    external_intercalation("tmp/provao_cp.bin", out_filename, &sort_performance,
                           &file_performance);
    break;
  case QUICK_SORT:
    cp_file(filename, "tmp/provao_cp.bin");
    ext_quick_sort("tmp/provao_cp.bin", input.quantity, &sort_performance);
    sprintf(out_filename, "out/method-3/output-%lu.txt", time(NULL));
    bin_to_txt("tmp/provao_cp.bin", out_filename, input.quantity,
               &file_performance);
    break;
  }
  bin_to_txt("tmp/provao_cp.bin", "out/provao_out.txt", input.quantity,
             &file_performance);

  if (input.should_print)
    print_txt(out_filename);

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

  sprintf(filename, "docs/perf-m%d-q%ld-s%d-%lu.txt", input.method,
          input.quantity, input.situation, time(NULL));

  save_performance(filename, "Sorting performance:", sort_performance);
  save_performance(filename, "File handling:", file_performance);
  save_performance(filename, "Total performance:", total_perf);

  return EXIT_SUCCESS;
}
