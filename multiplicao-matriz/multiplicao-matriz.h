#ifndef MULTIPLICAO_MATRIZ_H
#define MULTIPLICAO_MATRIZ_H

#include "./gerenciar-matriz/gerenciar-matriz.h"
#include "./agrupador-elementos-matriz/agrupador-elementos-matriz.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void multiplicar_matrizes_sequencial(Matriz* m1, Matriz* m2, char* nomeAnalise);
void multiplicar_matrizes_paralelo_threads(Matriz* m1, Matriz* m2, char* nomeAnalise, int threadsOuProcessosDivisor);
void multiplicar_matrizes_paralelo_processos(Matriz* m1, Matriz* m2, char* nomeAnalise, int threadsOuProcessosDivisor);

void multiplicar_matrizes(Matriz* m1, Matriz* m2, char* tipoExecucao, char* nomeAnalise, int threadsOuProcessosDivisor);

FILE* gerar_arquivo_resultado(const char* nomeAnalise, const char* tipo_execucao, const char* nomeArquivo);

void* processar_elementos(void* arg);

#endif