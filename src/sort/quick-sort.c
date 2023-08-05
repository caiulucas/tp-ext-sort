#include "quick-sort.h"
#include "../utils/status-messages.h"
#include <float.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <time.h>

bool insert_register(Register *reg, Area *area, Performance *perf) {
  if (area->reg_count >= AREA_SZ)
    return false;

  int index = 0;

  for (size_t i = 0; i < area->reg_count; i++) {
    perf->comparisons_count++;
    if (reg->grade <= area->registers[i].grade)
      break;

    index++;
  }

  for (int i = area->reg_count - 1; i >= index; i--) {
    area->registers[i + 1] = area->registers[i];
  }

  area->registers[index] = *reg;
  (area->reg_count)++;

  return true;
}

void insert_in_area(Area *area, Register *reg, size_t *reg_count,
                    Performance *perf) {
  insert_register(reg, area, perf);
  *reg_count = area->reg_count;
}

void write_inf(FILE **file, Register reg, size_t *displacement,
               Performance *perf) {
  perf->writes_count++;
  fwrite(&reg, sizeof(Register), 1, *file);
  (*displacement)++;
}

void write_sup(FILE **file, Register reg, size_t *displacement,
               Performance *perf) {
  fseek(*file, (*displacement - 1) * sizeof(Register), SEEK_SET);

  perf->writes_count++;
  fwrite(&reg, sizeof(Register), 1, *file);
  (*displacement)--;
}

void remove_first(Area *area, Register *reg) {
  *reg = area->registers[0];

  for (size_t i = 0; i < area->reg_count - 1; i++) {
    area->registers[i] = area->registers[i + 1];
  }

  (area->reg_count)--;
}

void remove_last(Area *area, Register *reg) {
  *reg = area->registers[area->reg_count - 1];
  (area->reg_count)--;
}

void remove_inf(Area *area, Register *reg, size_t *reg_count) {
  remove_first(area, reg);
  *reg_count = area->reg_count;
}

void remove_sup(Area *area, Register *reg, size_t *reg_count) {
  remove_last(area, reg);
  *reg_count = area->reg_count;
}

void read_inf(FILE **file, Register *reg, size_t *displacement,
              bool *must_read_right, Performance *perf) {
  perf->reads_count++;
  fread(reg, sizeof(Register), 1, *file);

  (*displacement)++;
  *must_read_right = true;
}

void read_sup(FILE **file, Register *reg, size_t *displacement,
              bool *must_read_right, Performance *perf) {
  fseek(*file, (*displacement - 1) * sizeof(Register), SEEK_SET);

  perf->reads_count++;
  fread(reg, sizeof(Register), 1, *file);

  (*displacement)--;
  *must_read_right = false;
}

void partition(FILE **read_file_inf, FILE **write_file_sup,
               FILE **read_write_file, Area area, int left, int right, int *i,
               int *j, Performance *perf) {

  size_t read_sup_disp = right;
  size_t read_inf_disp = left;
  size_t write_sup_disp = right;
  size_t write_inf_disp = left;
  size_t reg_count = 0;

  double inf_limit = DBL_MIN;
  double sup_limit = DBL_MAX;

  bool must_read_right = true;

  Register last_read_reg, aux_reg;

  fseek(*read_file_inf, (read_inf_disp - 1) * sizeof(Register), SEEK_SET);
  fseek(*write_file_sup, (write_inf_disp - 1) * sizeof(Register), SEEK_SET);

  *i = left - 1;
  *j = right + 1;

  while (read_sup_disp >= read_inf_disp) {
    if (reg_count < AREA_SZ - 1) {
      if (must_read_right)
        read_sup(read_write_file, &last_read_reg, &read_sup_disp,
                 &must_read_right, perf);
      else
        read_inf(read_file_inf, &last_read_reg, &read_inf_disp,
                 &must_read_right, perf);

      insert_in_area(&area, &last_read_reg, &reg_count, perf);

      continue;
    }

    if (read_sup_disp == write_sup_disp)
      read_sup(read_write_file, &last_read_reg, &read_sup_disp,
               &must_read_right, perf);

    else if (read_inf_disp == write_inf_disp)
      read_inf(read_file_inf, &last_read_reg, &read_inf_disp, &must_read_right,
               perf);

    else if (must_read_right)
      read_sup(read_write_file, &last_read_reg, &read_sup_disp,
               &must_read_right, perf);

    else
      read_inf(read_file_inf, &last_read_reg, &read_inf_disp, &must_read_right,
               perf);

    perf->comparisons_count++;
    if (last_read_reg.grade <= inf_limit) {
      *i = write_inf_disp;
      write_inf(write_file_sup, last_read_reg, &write_inf_disp, perf);
      continue;
    }

    perf->comparisons_count++;
    if (last_read_reg.grade >= sup_limit) {
      *j = write_sup_disp;
      write_sup(read_write_file, last_read_reg, &write_sup_disp, perf);
      continue;
    }

    insert_in_area(&area, &last_read_reg, &reg_count, perf);

    if (write_inf_disp - left < right - write_sup_disp) {
      remove_inf(&area, &aux_reg, &reg_count);
      write_inf(write_file_sup, aux_reg, &write_inf_disp, perf);

      inf_limit = aux_reg.grade;
    }

    else {
      remove_sup(&area, &aux_reg, &reg_count);
      write_sup(read_write_file, aux_reg, &write_sup_disp, perf);

      sup_limit = aux_reg.grade;
    }
  }

  while (write_inf_disp <= write_sup_disp) {
    remove_inf(&area, &aux_reg, &reg_count);
    write_inf(write_file_sup, aux_reg, &write_inf_disp, perf);
  }
}

void quick_sort(FILE **inf_read_file, FILE **inf_write_file,
                FILE **read_write_file, int left, int right,
                Performance *perf) {

  int i;
  int j;

  if (right - left < 1)
    return;

  Area area;
  area.reg_count = 0;

  partition(inf_read_file, inf_write_file, read_write_file, area, left, right,
            &i, &j, perf);

  fflush(*inf_read_file);
  fflush(*inf_write_file);
  fflush(*read_write_file);

  if (i - left < right - j) {
    quick_sort(inf_read_file, inf_write_file, read_write_file, left, i, perf);
    quick_sort(inf_read_file, inf_write_file, read_write_file, j, right, perf);
  } else {
    quick_sort(inf_read_file, inf_write_file, read_write_file, j, right, perf);
    quick_sort(inf_read_file, inf_write_file, read_write_file, left, i, perf);
  }
}

void ext_quick_sort(char const *filename, size_t size, Performance *perf) {
  FILE *inf_read_file = fopen(filename, "rb+");
  FILE *inf_write_file = fopen(filename, "rb+");
  FILE *read_write_file = fopen(filename, "rb+");

  if (!inf_read_file || !inf_write_file || !read_write_file) {
    error_msg("Failed to open file.\n");
    return;
  }

  size_t start_clock = clock();
  quick_sort(&inf_read_file, &inf_write_file, &read_write_file, 1, size, perf);
  size_t end_clock = clock();

  perf->execution_time = (double)(end_clock - start_clock) / CLOCKS_PER_SEC;

  fclose(inf_read_file);
  fclose(inf_write_file);
  fclose(read_write_file);

  return;
}