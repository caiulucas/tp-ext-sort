#include "merge-sort.h"
#include <stddef.h>
#include <stdlib.h>

// Merges a list of items
void merge(Item *items, size_t left, size_t middle, size_t right) {
  size_t left_sz = middle - left + 1;
  size_t right_sz = right - middle;

  Item *left_items = (Item *)malloc(sizeof(Item) * left_sz);
  Item *right_items = (Item *)malloc(sizeof(Item) * right_sz);

  for (size_t i = 0; i < left_sz; i++)
    left_items[i] = items[i + left];

  for (size_t i = 0; i < right_sz; i++)
    right_items[i] = items[i + middle + 1];

  size_t left_index = 0;
  size_t right_index = 0;

  for (size_t i = left; i <= right; i++) {
    if (right_index == right_sz ||
        left_items[left_index].key < right_items[right_index].key) {
      items[i] = left_items[left_index];
      left_index++;
    } else {
      items[i] = right_items[right_index];
      right_index++;
    }
  }

  free(left_items);
  free(right_items);
}

void merge_sort(Item *items, int left, int right) {
  if (left == right)
    return;

  int middle = (left + right) / 2;

  merge_sort(items, left, middle);
  merge_sort(items, middle + 1, right);

  merge(items, left, middle, right);
}