#include "quick-sort.h"
#include "../utils/status-messages.h"
#include <float.h>
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

typedef enum { FIRST, LAST } RemoveMode;

void empty_area(Area *area) {
  area->cell_displacement = 0;
  area->first = 0;
  area->last = 0;
  area->used_cells = 0;

  for (size_t i = 0; i < AREA_SZ; i++) {
    area->cells[i].prev = 0;
    area->cells[i].next = i + 1;
  }
}

void insert_register(Area *area, Register *reg) {
  if (area->used_cells >= AREA_SZ) {
    error_msg("Memory overflow.");
    return;
  }

  size_t insertion_point = area->cell_displacement;

  for (size_t i = 0; i < area->used_cells; i++) {
    if (area->cells[i].reg.grade > reg->grade)
      break;

    insertion_point++;
  }

  for (size_t i = area->used_cells - 1; i >= insertion_point; i--)
    area->cells[i + 1] = area->cells[i];

  area->cells[insertion_point].reg = *reg;
  area->used_cells += 1;
}

void insert_in_area(Area *area, Register *reg, size_t *used_cells) {
  insert_register(area, reg);
  *used_cells = area->used_cells;
}

void write_max(FILE **file, Register reg, size_t *displacement) {
  fseek(*file, (*displacement - 1) * sizeof(Register), SEEK_SET);
  fwrite(&reg, sizeof(Register), 1, *file);
  *displacement -= 1;
}

void write_min(FILE **file, Register reg, size_t *displacement) {
  fwrite(&reg, sizeof(Register), 1, *file);
  (*displacement)++;
}

void remove_first(Area *area, Register *reg) {
  *reg = area->cells[0].reg;

  for (size_t i = 0; i < area->used_cells - 1; i++) {
    area->cells[i] = area->cells[i + 1];
  }

  (area->used_cells)--;
}

void remove_last(Area *area, Register *reg) {
  *reg = area->cells[area->used_cells - 1].reg;
  (area->used_cells)--;
}

void remove_min(Area *area, Register *reg, size_t *used_cells) {
  remove_first(area, reg);
  *used_cells = area->used_cells;
}

void remove_max(Area *area, Register *reg, size_t *used_cells) {
  remove_last(area, reg);
  *used_cells = area->used_cells;
}

void read_sup(FILE **file, Register *reg, size_t *displacement,
              bool *must_read_left) {

  fseek(*file, (*displacement - 1) * sizeof(Register), SEEK_SET);
  fread(reg, sizeof(Register), 1, *file);

  (*displacement)--;
  *must_read_left = true;
}

void read_inf(FILE **file, Register *reg, size_t *displacement,
              bool *must_read_left) {

  fread(reg, sizeof(Register), 1, *file);

  (*displacement)++;
  *must_read_left = false;
}

void partition(FILE **read_inf_file, FILE **write_inf_file,
               FILE **read_write_file, Area area, int left, int right, int *i,
               int *j) {
  size_t read_sup_dis = right;
  size_t write_sup_dis = right;
  size_t read_inf_dis = left;
  size_t write_inf_dis = left;

  size_t used_cells = 0;
  double inf_limit = DBL_MIN;
  double sup_limit = DBL_MAX;

  bool must_read_left = false;
  Register last_read;
  Register aux;

  fseek(*read_inf_file, (read_inf_dis - 1) * sizeof(Register), SEEK_SET);
  fseek(*write_inf_file, (write_inf_dis - 1) * sizeof(Register), SEEK_SET);

  *i = left - 1;
  *j = right + 1;

  while (read_sup_dis >= read_inf_dis) {
    if (used_cells < AREA_SZ - 1) {
      if (must_read_left)
        read_inf(read_write_file, &last_read, &read_inf_dis, &must_read_left);
      else
        read_sup(read_write_file, &last_read, &read_sup_dis, &must_read_left);

      continue;
    }

    if (read_sup_dis == write_sup_dis)
      read_sup(read_write_file, &last_read, &read_sup_dis, &must_read_left);
    else if (read_inf_dis == write_inf_dis)
      read_inf(read_write_file, &last_read, &read_inf_dis, &must_read_left);
    else if (must_read_left)
      read_inf(read_write_file, &last_read, &read_inf_dis, &must_read_left);
    else
      read_sup(read_write_file, &last_read, &read_sup_dis, &must_read_left);

    if (last_read.grade <= inf_limit) {
      *i = write_inf_dis;
      write_min(read_write_file, last_read, &write_inf_dis);
      continue;
    }

    if (last_read.grade >= sup_limit) {
      *j = write_sup_dis;
      write_max(read_write_file, last_read, &write_sup_dis);
      continue;
    }

    insert_in_area(&area, &last_read, &used_cells);

    if (write_inf_dis <= write_sup_dis) {
      remove_min(&area, &aux, &used_cells);
      write_min(read_write_file, aux, &write_inf_dis);
      inf_limit = aux.grade;
    } else {
      remove_max(&area, &aux, &used_cells);
      write_max(read_write_file, aux, &write_sup_dis);
      sup_limit = aux.grade;
    }
  }

  while (write_inf_dis <= write_sup_dis) {
    remove_min(&area, &aux, &used_cells);
    write_max(read_write_file, aux, &write_inf_dis);
  }
}

void quick_sort(FILE **li_file, FILE **ei_file, FILE **les_file, int left,
                int right) {
  if (right - left < 1)
    return;

  int i;
  int j;
  Area area;

  empty_area(&area);
  partition(li_file, ei_file, les_file, area, left, right, &i, &j);

  fflush(*li_file);
  fflush(*ei_file);
  fflush(*les_file);

  if (i - left < right - j) {
    quick_sort(li_file, ei_file, les_file, left, i);
    quick_sort(li_file, ei_file, les_file, j, right);
  } else {
    quick_sort(li_file, ei_file, les_file, j, right);
    quick_sort(li_file, ei_file, les_file, left, i);
  }
}

void ext_quick_sort(char const *filename, size_t size) {
  FILE *file_1 = fopen(filename, "rb+");
  FILE *file_2 = fopen(filename, "rb+");
  FILE *file_3 = fopen(filename, "rb+");

  if (!file_1 || !file_2 || !file_3) {
    error_msg("Erro ao abrir o arquivo\n");
    exit(1);
  }

  quick_sort(&file_1, &file_2, &file_3, 1, size);

  fclose(file_1);
  fclose(file_2);
  fclose(file_3);
}