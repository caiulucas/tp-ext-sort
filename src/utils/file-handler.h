#pragma once

#include "../consts.h"
#include "../models/performance.h"
#include "../models/tape.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

// Converts a text file into a binary file
bool txt_to_bin(char const *txt_filename, char const *bin_filename, size_t size,
                Performance *perf);

// Shows a binary content in the terminal
void print_bin(char const *bin_filename);

// Converts a binary file into a text file
bool bin_to_txt(char const *bin_filename, char const *txt_filename, size_t size,
                Performance *perf);

bool bin_to_txt_desc(char const *bin_filename, char const *txt_filename,
                     size_t size, Performance *perf);

bool tapes_to_txt(char const *txt_filename, Tape *tapes, size_t size);

// Copy a file
bool cp_file(char const *origin_filename, char const *destination_file_name);

// Copy n registers of a file to another
bool cp_file_sized(char const *origin_filename,
                   char const *destination_file_name, size_t size);

void print_txt(char const *txt_filename);