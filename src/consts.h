#pragma once

#define AVAILABLE_TAPES 20

typedef struct Register {
  int id;
  float grade;
  char state[3];
  char city[51];
  char course[31];
} Register;