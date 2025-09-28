#ifndef GERENCIAR_MATRIZ_H
#define GERENCIAR_MATRIZ_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h> 
#include <time.h>

typedef struct {
    int qtdLinhas;
    int qtdColunas;
    double **data;
} Matriz;

Matriz* alocar_matriz(int qtdLinhas, int qtdColunas);
void desalocar_matriz(Matriz* matriz); 
void gerar_matriz_aleatoria(Matriz* matriz);
void salvar_matriz_arquivo(Matriz* matriz, const char* sufixo);
Matriz* carregar_matriz_arquivo(const char* nome_arquivo);

#endif