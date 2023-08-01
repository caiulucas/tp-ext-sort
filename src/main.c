#include "consts.h"
#include "sort/merge-sort.h"
#include "sort/tapes.h"
#include "utils/file-handler.h"
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

  txt_to_bin("provao.txt", "tmp/provao.bin", input.quantity);

  switch (input.method) {
  case INTERNAL_INTERCALATION:
    // TODO;
    break;
  case EXTERNAL_INTERCALATION:
    // TODO;
    break;
  case QUICK_SORT:
    // TODO;
    break;
  }

  return EXIT_SUCCESS;
}
