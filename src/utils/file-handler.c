#include "file-handler.h"
#include "../consts.h"
#include "status-messages.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void print_error_message(char const *filename) {
  char msg[100];
  sprintf(msg, "Error opening file %s\n", filename);
  error_msg(msg);
}

bool txt_to_bin(char const *txt_filename, char const *bin_filename, size_t size,
                Perfomance *perfomance) {
  clock_t start_clock = clock();

  FILE *txt_file = fopen(txt_filename, "r");

  perfomance->comparisons_count += 1;
  if (!txt_file) {
    print_error_message(txt_filename);
    return false;
  }

  FILE *bin_file = fopen(bin_filename, "wb");

  perfomance->comparisons_count += 1;
  if (!bin_file) {
    print_error_message(bin_filename);
    return false;
  }

  for (size_t i = 0; i < size; i++) {
    Register reg;

    perfomance->reads_count += 1;
    fscanf(txt_file, "%zu %lf", &reg.id, &reg.grade);
    fgets(reg.content, CONTENT_MAX_SIZE, txt_file);

    reg.grade = roundf(reg.grade * 10.0) / 10.0;
    reg.content[CONTENT_MAX_SIZE - 1] = '\0';

    fwrite(&reg, sizeof(Register), 1, bin_file);

    perfomance->comparisons_count += 1;
    if (feof(txt_file))
      break;
  }

  fclose(txt_file);
  fclose(bin_file);

  clock_t end_clock = clock();

  perfomance->execution_time =
      ((double)(end_clock - start_clock)) / CLOCKS_PER_SEC;

  return true;
}

void print_bin(char const *filename) {
  FILE *bin_file = fopen(filename, "rb");

  if (!bin_file) {
    print_error_message(filename);
    return;
  }

  Register reg;

  info_msg("Printing file...");
  while (fread(&reg, sizeof(Register), 1, bin_file)) {
    printf("%zu %lf %s\n", reg.id, reg.grade, reg.content);
  }

  fclose(bin_file);
}

bool bin_to_txt(char const *bin_filename, char const *txt_filename) {
  FILE *bin_file = fopen(bin_filename, "rb");

  if (!bin_file) {
    print_error_message(bin_filename);
    return false;
  }

  FILE *txt_file = fopen(txt_filename, "w");

  if (!txt_file) {
    print_error_message(txt_filename);
    return false;
  }

  while (!feof(bin_file)) {
    Register reg;
    fread(&reg, sizeof(Register), 1, bin_file);
    fprintf(txt_file, "%zu %.1lf %s\n", reg.id, reg.grade, reg.content);
  }

  fclose(bin_file);
  fclose(txt_file);

  char msg[100];
  sprintf(msg, "File %s created successfuly\n", txt_filename);
  success_msg(msg);

  return true;
}

bool cp_file(char const *origin_filename, char const *destintion_file_name) {
  char command[100];

  sprintf(command, "cp %s %s", origin_filename, destintion_file_name);

  char msg[100];
  if (system(command) == -1) {
    sprintf(msg, "Failed to copy %s to %s\n", origin_filename,
            destintion_file_name);

    error_msg(msg);
    return false;
  }

  sprintf(msg, "Copied %s to %s\n", origin_filename, destintion_file_name);
  success_msg(msg);
  return true;
}