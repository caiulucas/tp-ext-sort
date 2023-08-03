#pragma once

#include "../consts.h"
#include <stdbool.h>
#include <stdio.h>

// Converts a text file into a binary file
bool txt_to_bin(char const *txt_filename, char const *bin_filename, size_t size,
                Perfomance *perfomance);

// Shows a binary content in the terminal
void print_bin(char const *bin_filename);

// Converts a binary file into a text file
bool bin_to_txt(char const *bin_filename, char const *txt_filename);

// Copy a file
bool cp_file(char const *origin_filename, char const *destintion_file_name);
