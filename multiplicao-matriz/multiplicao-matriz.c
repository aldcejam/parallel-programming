#include "multiplicao-matriz.h"
#include <math.h> 

FILE* gerar_arquivo_resultado(const char* nomeAnalise, const char* tipo_execucao, const char* nomeArquivo) {
    if (!tipo_execucao || !nomeAnalise || !nomeArquivo) return NULL;

    int tamanho = snprintf(
        NULL, 0,
        "resultados/%s/%s/%s.txt",
        nomeAnalise, tipo_execucao, nomeArquivo
    );

    char* caminho = malloc(tamanho + 1);
    if (!caminho) return NULL;

    snprintf(
        caminho, tamanho + 1,
        "resultados/%s/%s/%s.txt",
        nomeAnalise, tipo_execucao, nomeArquivo
    );

    char* dir_path = malloc(tamanho + 1);
    if (!dir_path) {
        free(caminho);
        return NULL;
    }
    
    snprintf(dir_path, tamanho + 1, "resultados/%s/%s", nomeAnalise, tipo_execucao);
    
    char* p = dir_path;
    while (*p != '\0') {
        if (*p == '/') {
            *p = '\0';
            mkdir(dir_path, 0755);
            *p = '/';
        }
        p++;
    }
    mkdir(dir_path, 0755);
    free(dir_path);

    FILE* arquivo = fopen(caminho, "w");
    free(caminho);
    
    return arquivo;
}

Matriz* multiplicar_matrizes(Matriz* m1, Matriz* m2, char* tipoExecucao, char* nomeAnalise, int threadsOuProcessosDivisor) {
    if (strcmp(tipoExecucao, "S") == 0 || strcmp(tipoExecucao, "s") == 0) {
        return multiplicar_matrizes_sequencial(m1, m2, nomeAnalise);
    } else if (strcmp(tipoExecucao, "PP") == 0 || strcmp(tipoExecucao, "pp") == 0) {
        return multiplicar_matrizes_paralelo_processos(m1, m2);
    } else if (strcmp(tipoExecucao, "PT") == 0 || strcmp(tipoExecucao, "pt") == 0) {
        return multiplicar_matrizes_paralelo_threads(m1, m2, threadsOuProcessosDivisor);
    } else {
        fprintf(stderr, "Tipo de execução inválido. Use 'S', 'PT' ou 'PP'.\n");
        return NULL;
    }
}

Matriz* multiplicar_matrizes_sequencial(Matriz* m1, Matriz* m2, char* nomeAnalise) {
    char nomeArquivo[100];
    snprintf(nomeArquivo, sizeof(nomeArquivo), "%dx%d", m1->qtdLinhas, m2->qtdColunas);
    FILE* arquivo = gerar_arquivo_resultado(nomeAnalise, "sequencial", nomeArquivo);
    Matriz* produto = alocar_matriz(m1->qtdLinhas, m2->qtdColunas);
    if (!produto) return NULL;

    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo para escrita");
        return NULL;
    }
    // =============================== //
    clock_t start_time = clock();
    
    fprintf(arquivo, "%d %d\n", m1->qtdLinhas, m2->qtdColunas);
    for (int i = 0; i < m1->qtdLinhas; i++) {
        for (int j = 0; j < m2->qtdColunas; j++) {
            double soma = 0.0;
            for (int k = 0; k < m1->qtdColunas; k++) {
                soma += m1->data[i][k] * m2->data[k][j];
            }
            produto->data[i][j] = soma;
            fprintf(arquivo, "c%d%d = %.4f\n", i, j, soma);
        }
    }
    
    clock_t end_time = clock();
    // =============================== //

    double tempo_calculo = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    fprintf(arquivo, "%.6f", tempo_calculo);

    fclose(arquivo);
    return produto;
}

typedef struct {
    int qtdColunas;
    int linha;
    int colunaInicio;
} Elemento;

Matriz* multiplicar_matrizes_paralelo_threads(Matriz* m1, Matriz* m2, int threadsOuProcessosDivisor) {
    int qtdElementos = m1->qtdLinhas * m2->qtdColunas;
    int quantidadeThreads = (int)ceil((double)qtdElementos / threadsOuProcessosDivisor);
    int qtdMaxElementosPorThread = (int)ceil((double)qtdElementos / quantidadeThreads);
    printf("Quantidade de elementos: %d\n", qtdElementos);
    printf("Quantidade de threads: %d\n", quantidadeThreads);
    printf("Elementos por thread (máximo): %d\n", qtdMaxElementosPorThread);

    Elemento* elementos = malloc(qtdElementos * sizeof(Elemento));

    int idx = 0;
    for (int i = 0; i < m1->qtdLinhas; i++) {
        for (int j = 0; j < m2->qtdColunas; j++) {
            elementos[idx].linha = i;
            elementos[idx].colunaInicio = j;
            elementos[idx].qtdColunas = m2->qtdColunas;
            idx++;
        }
    }

    Elemento*** arrayThreads = malloc(quantidadeThreads * sizeof(Elemento**));

    for (int i = 0; i < quantidadeThreads; i++) {
        int elementosNestaThread;
        
        if (i == quantidadeThreads - 1) {
            elementosNestaThread = qtdElementos - (i * qtdMaxElementosPorThread);
        } else {
            elementosNestaThread = qtdMaxElementosPorThread;
        }
        
        arrayThreads[i] = malloc(elementosNestaThread * sizeof(Elemento*));
    }

    idx = 0;
    for (int i = 0; i < quantidadeThreads; i++) {
        int elementosNestaThread;
        
        if (i == quantidadeThreads - 1) {
            elementosNestaThread = qtdElementos - (i * qtdMaxElementosPorThread);
        } else {
            elementosNestaThread = qtdMaxElementosPorThread;
        }
        
        for (int j = 0; j < elementosNestaThread; j++) {
            arrayThreads[i][j] = &elementos[idx];
            idx += 1;
        }
    }

    // PERCORRER E VERIFICAR O ARRAY DE ARRAYS DE PONTEIROS
    printf("=== VERIFICAÇÃO DO ARRAY DE ARRAYS DE PONTEIROS ===\n");
    printf("Quantidade total de threads: %d\n", quantidadeThreads);
    printf("Quantidade total de elementos: %d\n", qtdElementos);
    printf("Elementos por thread (máximo): %d\n\n", qtdMaxElementosPorThread);

    int totalElementosVerificados = 0;
    
    for (int i = 0; i < quantidadeThreads; i++) {
        int elementosNestaThread;
        
        if (i == quantidadeThreads - 1) {
            elementosNestaThread = qtdElementos - (i * qtdMaxElementosPorThread);
        } else {
            elementosNestaThread = qtdMaxElementosPorThread;
        }
        
        printf("Thread %d: %d elementos\n", i, elementosNestaThread);
        printf("----------------------------------------\n");
        
        for (int j = 0; j < elementosNestaThread; j++) {
            Elemento* elem = arrayThreads[i][j];
            printf("  [%d][%d] -> linha=%d, coluna=%d, qtdColunas=%d\n", 
                   i, j, elem->linha, elem->colunaInicio, elem->qtdColunas);
            
            // Verificar se o ponteiro aponta para o elemento correto
            int elementoIndex = elem - elementos; // cálculo do índice no array original
            printf("        Ponteiro aponta para elementos[%d]\n", elementoIndex);
            
            totalElementosVerificados++;
        }
        printf("\n");
    }

    printf("=== RESUMO ===\n");
    printf("Total de elementos verificados: %d\n", totalElementosVerificados);
    printf("Total de elementos criados: %d\n", qtdElementos);
    printf("Status: %s\n", (totalElementosVerificados == qtdElementos) ? "CORRETO" : "ERRO");

    // Liberar memória (em uma implementação real você usaria antes de liberar)
    for (int i = 0; i < quantidadeThreads; i++) {
        free(arrayThreads[i]);
    }
    free(arrayThreads);
    free(elementos);

    return NULL;
}

Matriz* multiplicar_matrizes_paralelo_processos(Matriz* m1, Matriz* m2) {
    fprintf(stderr, "Multiplicação paralela com processos não implementada ainda.\n");
    desalocar_matriz(m1);
    desalocar_matriz(m2);
    return NULL;
}

