#ifndef MULTIPLICAO_MATRIZ_H
#define MULTIPLICAO_MATRIZ_H

#include "./gerenciar-matriz/gerenciar-matriz.h"

Matriz* multiplicar_matrizes_sequencial(Matriz* m1, Matriz* m2, char* nomeAnalise);
Matriz* multiplicar_matrizes_paralelo_threads(Matriz* m1, Matriz* m2, int threadsOuProcessosDivisor);
Matriz* multiplicar_matrizes_paralelo_processos(Matriz* m1, Matriz* m2);

Matriz* multiplicar_matrizes(Matriz* m1, Matriz* m2, char* tipoExecucao, char* nomeAnalise, int threadsOuProcessosDivisor);

#endif