#include "agrupador-elementos-matriz.h"

Agrupador* agrupar_elementos(Matriz* m1, Matriz* m2, int divisor) {
    int qtdElementos = m1->qtdLinhas * m2->qtdColunas;
    int quantidadeThreads = (int)ceil((double)qtdElementos / divisor);
    int qtdMaxElementosPorThread = (int)ceil((double)qtdElementos / quantidadeThreads);
        
    Elemento* elementos = malloc(qtdElementos * sizeof(Elemento));
    for (int i = 0, idx = 0; i < m1->qtdLinhas; i++) {
        for (int j = 0; j < m2->qtdColunas; j++, idx++) {
            elementos[idx] = (Elemento){m2->qtdColunas, i, j};
        }
    }

    ThreadData* arrayThreads = malloc(quantidadeThreads * sizeof(ThreadData));
    
    for (int i = 0, idx = 0; i < quantidadeThreads; i++) {
        int elementosNestaThread = (i == quantidadeThreads - 1) 
            ? qtdElementos - (i * qtdMaxElementosPorThread) 
            : qtdMaxElementosPorThread;
        
        Elemento** elementosThread = malloc(elementosNestaThread * sizeof(Elemento*));
        
        for (int j = 0; j < elementosNestaThread; j++, idx++) {
            elementosThread[j] = &elementos[idx];
        }
        
        arrayThreads[i] = (ThreadData){
            .m1 = m1,
            .m2 = m2,
            .elementos = elementosThread,
            .qtdElementos = elementosNestaThread,
            .id_executor = 0,
            .nomeAnalise = NULL,        
            .tipo_execucao = NULL,
            .versao = 0
        };
    }

    Agrupador* agrupador = malloc(sizeof(Agrupador));
    agrupador->elementosAgrupados = arrayThreads;
    agrupador->quantidadeExecutores = quantidadeThreads;

    return agrupador;
}
