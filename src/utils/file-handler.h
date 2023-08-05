#pragma once

#include "../consts.h"
#include <stdbool.h>
#include <stdio.h>

// Converts a text file into a binary file
bool txt_to_bin(char const *txt_filename, char const *bin_filename, size_t size,
                Performance *perf);

// Shows a binary content in the terminal
void print_bin(char const *bin_filename);

// Converts a binary file into a text file
bool bin_to_txt(char const *bin_filename, char const *txt_filename, size_t size,
                Performance *perf);

// Copy a file
bool cp_file(char const *origin_filename, char const *destination_file_name);
