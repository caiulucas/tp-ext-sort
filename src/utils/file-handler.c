#include "file-handler.h"
#include "../consts.h"
#include "status-messages.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_error_message(char const *filename) {
  char *msg = (char *)malloc(sizeof(char) * 100);
  sprintf(msg, "Error opening file %s\n", filename);
  error_msg(msg);
  printf("%s", msg);
  free(msg);
}

bool txt_to_bin(char const *txt_filename, char const *bin_filename,
                size_t size) {
  FILE *txt_file = fopen(txt_filename, "r");

  if (!txt_file) {
    print_error_message(txt_filename);
    return false;
  }

  FILE *bin_file = fopen(bin_filename, "wb");

  if (!bin_file) {
    print_error_message(bin_filename);
    return false;
  }

  for (size_t i = 0; i < size; i++) {
    Register reg;
    fscanf(txt_file, "%zu %lf", &reg.id, &reg.grade);
    fgets(reg.content, CONTENT_MAX_SIZE, txt_file);

    reg.grade = roundf(reg.grade * 10.0) / 10.0;
    reg.content[CONTENT_MAX_SIZE - 1] = '\0';

    fwrite(&reg, sizeof(Register), 1, bin_file);

    if (feof(txt_file))
      break;
  }

  fclose(txt_file);
  fclose(bin_file);

  return true;
}

void print_bin(char const *filename) {
  FILE *bin_file = fopen(filename, "rb");

  if (!bin_file) {
    print_error_message(filename);
    return;
  }

  Register reg;

  while (fread(&reg, sizeof(Register), 1, bin_file)) {
    printf("%zu %lf %s\n", reg.id, reg.grade, reg.content);
  }

  fclose(bin_file);
}
