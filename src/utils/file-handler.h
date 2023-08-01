#pragma once

#include <stdbool.h>
#include <stdio.h>

// Converts a text file into a binary file
bool txt_to_bin(char const *txt_filename, char const *bin_filename,
                size_t size);

void print_bin(char const *bin_filename);