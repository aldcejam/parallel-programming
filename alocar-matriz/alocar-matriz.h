#ifndef ALLOCAR_MATRIZ_H
#define ALLOCAR_MATRIZ_H

#include <stdio.h>

typedef struct {
    int qtdLinhas;
    int qtdColunas;
    int **data;
} Matriz;

Matriz* alocar_matriz(int qtdLinhas, int qtdColunas);

#endif