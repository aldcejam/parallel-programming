#include <stdio.h>
#include "alocar-matriz.h"

Matriz* alocar_matriz(int qtdLinhas, int qtdColunas) {
    Matriz *matriz = (Matriz*)malloc(sizeof(Matriz));

    if (!matriz) {
        fprintf(stderr, "Erro ao alocar memória para a estrutura da matriz\n");
        return NULL;
    }

    matriz->qtdColunas = qtdColunas;
    matriz->qtdLinhas = qtdLinhas;

    matriz->data = (double**)malloc(qtdLinhas * sizeof(double*));

    if(!matriz->data) {
        fprintf(stderr, "Erro ao alocar memória para as linhas da matriz\n");
        free(matriz);
        return;
    }

    for (int i = 0; i < qtdLinhas; i++) {
        matriz->data[i] = (double*)malloc(qtdColunas * sizeof(double*));

        if (!matriz->data[i]) {
            fprintf(stderr, "Erro ao alocar memória para a linha %d\n", i);

            for (int j = 0; j < i; j++) {
                free(matriz->data[j]);
            }

            free(matriz->data);
            free(matriz);
            return NULL;
        }
    }

    return NULL;
}



