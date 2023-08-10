#include "file-handler.h"
#include "../consts.h"
#include "../models/register.h"
#include "status-messages.h"
#include <math.h>
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
                Performance *perf) {
  clock_t start_clock = clock();

  FILE *txt_file = fopen(txt_filename, "r");

  perf->comparisons_count += 1;
  if (!txt_file) {
    print_error_message(txt_filename);
    return false;
  }

  FILE *bin_file = fopen(bin_filename, "wb");

  perf->comparisons_count += 1;
  if (!bin_file) {
    print_error_message(bin_filename);
    return false;
  }

  for (size_t i = 0; i < size; i++) {
    Register reg;

    perf->reads_count += 1;
    fscanf(txt_file, "%zu %lf", &reg.id, &reg.grade);
    fgets(reg.content, CONTENT_MAX_SIZE, txt_file);

    reg.grade = roundf(reg.grade * 10.0) / 10.0;
    reg.content[CONTENT_MAX_SIZE - 1] = '\0';

    fwrite(&reg, sizeof(Register), 1, bin_file);

    perf->comparisons_count += 1;
    if (feof(txt_file))
      break;
  }

  fclose(txt_file);
  fclose(bin_file);

  clock_t end_clock = clock();

  perf->execution_time = ((double)(end_clock - start_clock)) / CLOCKS_PER_SEC;

  return true;
}

void print_bin(char const *filename) {
  FILE *bin_file = fopen(filename, "rb");

  if (!bin_file) {
    print_error_message(filename);
    return;
  }

  Register reg;

  printf("\n");
  info_msg("Printing file...\n");
  while (fread(&reg, sizeof(Register), 1, bin_file)) {
    printf("%zu %lf %s\n", reg.id, reg.grade, reg.content);
  }
  printf("\n");

  fclose(bin_file);
}

void delete_file(char const *filename) {
  if (remove(filename) != 0) {
    char msg[100];
    sprintf(msg, "File not found to remove %s.\n", filename);
    info_msg(msg);
  } else {

    char msg[100];
    sprintf(msg, "%s was removed.\n", filename);
    info_msg(msg);
  }
}

bool bin_to_txt(char const *bin_filename, char const *txt_filename, size_t size,
                Performance *perf) {
  clock_t start_clock = clock();

  delete_file(txt_filename);

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

  for (size_t i = 0; i < size; i++) {
    if (feof(bin_file))
      break;

    Register reg;
    fread(&reg, sizeof(Register), 1, bin_file);
    perf->reads_count++;
    fprintf(txt_file, "%zu %.1lf %s\n", reg.id, reg.grade, reg.content);
  }

  fclose(bin_file);
  fclose(txt_file);

  char msg[100];
  sprintf(msg, "File %s created successfully\n", txt_filename);
  success_msg(msg);

  clock_t end_clock = clock();
  perf->execution_time += ((double)(end_clock - start_clock)) / CLOCKS_PER_SEC;
  return true;
}

bool bin_to_txt_desc(char const *bin_filename, char const *txt_filename,
                     size_t size, Performance *perf) {
  clock_t start_clock = clock();

  delete_file(txt_filename);

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

  fseek(bin_file, 0, SEEK_END);
  size_t disp = ftell(bin_file);
  printf("%zu", disp);
  for (size_t i = 0; i < size; i++) {
    fseek(bin_file, disp - sizeof(Register) * (i + 1), SEEK_SET);
    Register reg;
    fread(&reg, sizeof(Register), 1, bin_file);
    perf->reads_count++;
    fprintf(txt_file, "%zu %.1lf %s\n", reg.id, reg.grade, reg.content);
  }

  fclose(bin_file);
  fclose(txt_file);

  char msg[100];
  sprintf(msg, "File %s created successfully\n", txt_filename);
  success_msg(msg);

  clock_t end_clock = clock();
  perf->execution_time += ((double)(end_clock - start_clock)) / CLOCKS_PER_SEC;
  return true;
}

bool tapes_to_txt(char const *txt_filename, Tape *tapes, size_t size) {
  FILE *txt_file = fopen(txt_filename, "a");

  if (!txt_file) {
    error_msg("Failed to open file.");
    return false;
  }

  for (size_t i = 0; i < size; i++) {
    if (tapes[i].block_size < 0)
      return true;

    Register reg;

    fread(NULL, sizeof(int), 1, tapes[i].file);
    fread(&reg, sizeof(Register), 1, tapes[i].file);
    fprintf(txt_file, "%ld %.1f %s", reg.id, reg.grade, reg.content);
  }

  return true;
}

bool cp_file(char const *origin_filename, char const *destination_file_name) {
  char command[100];

  sprintf(command, "cp %s %s", origin_filename, destination_file_name);

  char msg[100];
  if (system(command) == -1) {
    sprintf(msg, "Failed to copy %s to %s\n", origin_filename,
            destination_file_name);

    error_msg(msg);
    return false;
  }

  sprintf(msg, "Copied %s to %s\n", origin_filename, destination_file_name);
  success_msg(msg);
  return true;
}

bool cp_file_sized(char const *origin_filename,
                   char const *destination_filename, size_t size) {

  FILE *origin_file = fopen(origin_filename, "rb");

  if (!origin_file) {
    error_msg("Unable to open origin file.\n");
    return false;
  }

  delete_file(destination_filename);

  FILE *destination_file = fopen(destination_filename, "wb");

  if (!destination_file) {
    error_msg("Unable to open destination file.\n");
    return false;
  }

  for (size_t i = 0; i < size; i++) {
    if (feof(origin_file))
      break;

    Register reg;
    fread(&reg, sizeof(Register), 1, origin_file);
    fwrite(&reg, sizeof(Register), 1, destination_file);
  }

  char msg[100];
  sprintf(msg, "Copied %s to %s\n", origin_filename, destination_filename);
  success_msg(msg);

  fclose(origin_file);
  fclose(destination_file);
  return true;
}

void print_txt(char const *txt_filename) {
  FILE *txt_file = fopen(txt_filename, "r");
  char line[120];

  while (fgets(line, 100, txt_file)) {
    printf("%s", line);
  }

  fclose(txt_file);
}