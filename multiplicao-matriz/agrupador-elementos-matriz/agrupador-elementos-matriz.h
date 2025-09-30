#ifndef AGRUPADOR_ELEMENTOS_MATRIZ_H
#define AGRUPADOR_ELEMENTOS_MATRIZ_H

#include "../../gerenciar-matriz/gerenciar-matriz.h"
#include <math.h> 

typedef struct {
    int qtdColunas;
    int linha;
    int colunaInicio;
} Elemento;

typedef struct {
    Matriz* m1;
    Matriz* m2;
    Elemento** elementos;
    int qtdElementos;
    int id_executor;           
    char* nomeAnalise;         
    char* tipo_execucao;       
} ThreadData;

void* processar_elementos(void* arg);

typedef struct {
    ThreadData* elementosAgrupados;
    int quantidadeExecutores;    
} Agrupador;

Agrupador* agrupar_elementos(Matriz* m1, Matriz* m2, int divisor);

#endif