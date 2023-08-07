#include "tapes.h"
#include "../utils/status-messages.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init_tapes(Tape *tapes, char const *base_filename) {
  for (size_t i = 0; i < TAPES_SZ; i++) {
    tapes[i].block_size = 0;

    char tape_filename[100];
    sprintf(tape_filename, "%s-%zu.bin", base_filename, i);

    if (!(tapes[i].file = fopen(tape_filename, "wb+"))) {
      error_msg("Failed to create tape.\n");
    }
  }
}

void add_block(Tape *tape, Block *block, Performance *perf) {
  perf->writes_count += 2;
  fwrite(&block->registers_count, sizeof(int), 1, tape->file);
  fwrite(block->registers, sizeof(Register), block->registers_count,
         tape->file);

  tape->block_size += 1;
}

int block_size(Tape *tapes, bool is_intercalation) {
  int block_size = -1;
  int index = 0;

  if (is_intercalation)
    index = TAPES_SZ / 2;

  for (int i = index; i < index + (TAPES_SZ / 2); i++) {
    if (tapes[i].block_size > block_size)
      block_size = tapes[i].block_size;
  }

  return block_size;
}

void flush_tapes(Tape *tapes) {
  for (size_t i = 0; i < TAPES_SZ; i++) {
    fflush(tapes[i].file);
  }
}

void rewind_tapes(Tape *tapes) {
  for (size_t i = 0; i < TAPES_SZ; i++) {
    rewind(tapes[i].file);
  }
}

void reopen_tapes(Tape *tapes, bool is_inverse) {
  if (is_inverse) {
    for (int i = 0; i < TAPES_SZ / 2; i++) {
      tapes[i].file = freopen(NULL, "wb+", tapes[i].file);
      tapes[i].block_size = 0;
    }
    return;
  }

  for (int i = TAPES_SZ / 2; i < TAPES_SZ; i++) {
    tapes[i].file = freopen(NULL, "wb+", tapes[i].file);
    tapes[i].block_size = 0;
  }
}

void close_tapes(Tape *tapes) {
  for (int i = 0; i < TAPES_SZ; i++) {
    fclose(tapes[i].file);
  }
}

void imprimeFita(Tape *fita) {

  if (fita->block_size == 0)
    return;

  rewind(fita->file);

  int numeroAlunos = 0;
  Register aluno;

  int bloco = 1;

  while (fread(&numeroAlunos, sizeof(int), 1, fita->file)) {

    // cout << "\tBLOCO: " << bloco << endl << endl;

    for (int i = 0; i < numeroAlunos; i++) {
      fread(&aluno, sizeof(Register), 1, fita->file);
      // cout << "\t\tInscrição: " << aluno.numeroInscricao
      //      << "\tNota: " << aluno.nota << endl;
    }

    // cout << "----------------------------------------\n";
    bloco++;
  }
}

void imprimeFitas(Tape *tapes) {

  for (int i = 0; i < TAPES_SZ; i++) {
    // cout << "Fita " << i << endl;
    imprimeFita(&tapes[i]);
    // cout << "-------------------------------------------\n";
  }
}

// Auxiliar --------------------------------------------------

void converteFitaTexto(Tape fita) {

  int numeroAlunos;
  Register aluno;

  FILE *arqT;
  arqT = fopen("testeFita0.txt", "w");

  while (fread(&numeroAlunos, sizeof(int), 1, fita.file)) {

    fprintf(arqT, "%d\n", numeroAlunos);

    for (int i = 0; i < numeroAlunos; i++) {
      fread(&aluno, sizeof(Register), 1, fita.file);
      fprintf(arqT, "%08ld %04.1f %s\n", aluno.id, aluno.grade, aluno.content);
    }
  }

  fflush(arqT);
  fclose(arqT);
}