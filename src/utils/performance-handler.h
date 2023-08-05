#pragma once

#include "../consts.h"

// Print the performance of a function
void print_performance(Performance perf);

// Merge two performances
void merge_performance(Performance *perf1, Performance *perf2,
                       Performance *result);

void save_performance(char const *filename, char const *title,
                      Performance perf);