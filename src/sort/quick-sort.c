#include "quick-sort.h"
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
  if (area->used_cells == AREA_SZ) {
    printf("Memória cheia");
    return;
  }

  size_t displacement = area->cell_displacement;
  area->cell_displacement = area->cells[displacement].next;
  area->cells[displacement].reg = *reg;
  area->used_cells++;

  if (area->used_cells == 1) {
    area->first = displacement;
    area->last = displacement;
    area->cells[displacement].next = 0;
    area->cells[displacement].prev = 0;
    return;
  }

  size_t pos = area->first;

  if (reg->grade < area->cells[pos].reg.grade) {
    area->cells[displacement].prev = 0;
    area->cells[displacement].next = pos;
    area->first = displacement;
    return;
  }

  size_t insertion_index = area->cells[pos].next;

  while (insertion_index != 0 &&
         area->cells[insertion_index].reg.grade < reg->grade) {
    pos = insertion_index;
    insertion_index = area->cells[pos].next;
  }

  if (!insertion_index) {
    area->cells[displacement].prev = pos;
    area->cells[displacement].next = 0;
    area->cells[pos].next = displacement;
    area->last = displacement;
    return;
  }

  area->cells[displacement].prev = pos;
  area->cells[displacement].next = area->cells[pos].next;
  area->cells[pos].next = displacement;
  pos = area->cells[displacement].next;
  area->cells[pos].prev = displacement;
}

void remove_first(Area *area, Register *reg) {
  if (!area->used_cells)
    return;

  *reg = area->cells[area->first].reg;
  ptr_t next_temp = area->cells[area->first].next;

  area->cells[area->first].next = area->cell_displacement;
  area->cell_displacement = area->first;
  area->first = next_temp;

  if (area->first < AREA_SZ)
    area->cells[area->first].prev = 0;
  area->cell_displacement--;
}

void remove_last(Area *area, Register *reg) {
  if (!area->used_cells)
    return;

  *reg = area->cells[area->last].reg;
  ptr_t prev_temp = area->cells[area->last].prev;

  area->cells[area->last].next = area->cell_displacement;
  area->cell_displacement = area->last;
  area->last = prev_temp;

  if (area->last < AREA_SZ)
    area->cells[area->last].next = 0;
  area->cell_displacement--;
}

void read_file(FILE **file, Register *reg, size_t *displacement,
               bool *must_read_left) {
  fread(reg, sizeof(Register), 1, *file);

  if (!must_read_left) {
    *displacement += 1;
    *must_read_left = true;
  } else {
    *displacement -= 1;
    *must_read_left = false;
  }
}

void write_file(FILE **file, Register reg, size_t *displacement,
                bool is_displacement_sup) {
  fseek(*file, (*displacement - 1) * sizeof(Register), SEEK_SET);
  fwrite(&reg, sizeof(Register), 1, *file);

  if (is_displacement_sup)
    *displacement -= 1;
  else
    *displacement += 1;
}

void remove_in_area(Area *area, Register *reg, int *area_nr, RemoveMode mode) {
  switch (mode) {
  case FIRST:
    remove_first(area, reg);
    break;
  case LAST:
    remove_last(area, reg);
    break;
  }

  *area_nr = area->used_cells;
}

void insert_in_area(Area *area, Register *reg, int *area_nr) {
  insert_register(area, reg);
  *area_nr = area->used_cells;
}

void partition(FILE **read_inf_file, FILE **write_inf_file,
               FILE **read_write_file, Area area, int left, int right, int *i,
               int *j) {
  size_t read_sup_dis = right;
  size_t write_sup_dis = right;
  size_t read_inf_dis = left;
  size_t write_inf_dis = left;
  int area_nr = 0;
  size_t inf_limit = INT_MIN;
  size_t sup_limit = INT_MAX;

  bool must_read_left = true;
  Register last_read;
  Register aux;

  fseek(*read_inf_file, (read_inf_dis - 1) * sizeof(Register), SEEK_SET);
  fseek(*write_inf_file, (write_inf_dis - 1) * sizeof(Register), SEEK_SET);

  *i = left - 1;
  *j = right + 1;

  while (read_sup_dis >= read_inf_dis) {
    if (area_nr < AREA_SZ - 1) {
      if (must_read_left)
        read_file(read_write_file, &last_read, &read_inf_dis, &must_read_left);
      else
        read_file(read_write_file, &last_read, &read_sup_dis, &must_read_left);

      continue;
    }

    if (read_sup_dis == write_sup_dis)
      read_file(read_write_file, &last_read, &read_sup_dis, &must_read_left);
    else if (read_inf_dis == write_inf_dis)
      read_file(read_write_file, &last_read, &read_inf_dis, &must_read_left);
    else if (must_read_left)
      read_file(read_write_file, &last_read, &read_sup_dis, &must_read_left);
    else
      read_file(read_write_file, &last_read, &read_inf_dis, &must_read_left);

    if (last_read.id > sup_limit) {
      *j = write_sup_dis;
      write_file(read_write_file, last_read, &write_sup_dis, true);
      continue;
    }
    if (last_read.id < inf_limit) {
      *i = write_inf_dis;
      write_file(read_write_file, last_read, &write_inf_dis, false);
      continue;
    }

    insert_in_area(&area, &last_read, &area_nr);

    if (write_inf_dis <= write_sup_dis) {
      // remove_min(&area, &reg, &area_nr);
      write_file(read_write_file, aux, &write_inf_dis, false);
      inf_limit = aux.id;
    } else {
      // remove_max(&area, &reg, &area_nr);
      write_file(read_write_file, aux, &write_sup_dis, true);
      sup_limit = aux.id;
    }
  }

  while (write_inf_dis <= write_sup_dis) {
    // remove_min(&area, &reg, &area_nr);
    write_file(read_write_file, aux, &write_inf_dis, false);
  }
}

void ext_quick_sort(FILE **li_file, FILE **ei_file, FILE **les_file, int left,
                    int right) {
  if (right - left < 1)
    return;

  int i;
  int j;
  Area area;

  empty_area(&area);
  partition(li_file, ei_file, les_file, area, left, right, &i, &j);

  if (i - left < right - j) {
    ext_quick_sort(li_file, ei_file, les_file, left, i);
    ext_quick_sort(li_file, ei_file, les_file, j, right);
  } else {
    ext_quick_sort(li_file, ei_file, les_file, j, right);
    ext_quick_sort(li_file, ei_file, les_file, left, i);
  }
}