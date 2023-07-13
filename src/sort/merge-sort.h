#pragma once

typedef struct {
  int key;
  char name[50];
} Item;

// Sort a list by merging each item. [O(n*log(n)]
void merge_sort(Item *items, int left, int right);