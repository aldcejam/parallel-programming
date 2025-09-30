#include "multiplicao-matriz.h"
#include <math.h> 
#include <pthread.h>

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
        return multiplicar_matrizes_paralelo_processos(m1, m2, threadsOuProcessosDivisor);
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

void* processar_elementos(void* arg) {
    clock_t start_time = clock();
    
    ThreadData* data = (ThreadData*)arg;

    for (int i = 0; i < data->qtdElementos; i++) {
        Elemento* elem = data->elementos[i];
        double soma = 0.0;
        
        for (int k = 0; k < data->m1->qtdColunas; k++) {
            soma += data->m1->data[elem->linha][k] * data->m2->data[k][elem->colunaInicio];
        }
        
        printf("Elemento [%d][%d] = %.2f\n", elem->linha, elem->colunaInicio, soma);
    }

    clock_t end_time = clock();
    double tempo_calculo = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Tempo de cálculo: %.6f segundos\n", tempo_calculo);

    return NULL;
}

Matriz* multiplicar_matrizes_paralelo_threads(Matriz* m1, Matriz* m2, int threadsOuProcessosDivisor) {
    Agrupador* matriz_separada = agrupar_elementos(m1, m2, threadsOuProcessosDivisor);

    // PROCESSAMENTO POR THREADS
    ThreadData* arrayThreads = matriz_separada->elementosAgrupados;
    int quantidadeThreads = matriz_separada->quantidadeExecutores;

    pthread_t* threads = malloc(quantidadeThreads * sizeof(pthread_t));
    
    for (int i = 0; i < quantidadeThreads; i++) {
        ThreadData* threadData = malloc(sizeof(ThreadData));
        *threadData = arrayThreads[i];
        
        pthread_create(&threads[i], NULL, processar_elementos, threadData);
    }

    for (int i = 0; i < quantidadeThreads; i++) {
        pthread_join(threads[i], NULL);
    }

    free(threads);
    free(matriz_separada->elementosAgrupados);
    free(matriz_separada);

    return NULL;
}

Matriz* multiplicar_matrizes_paralelo_processos(Matriz* m1, Matriz* m2, int threadsOuProcessosDivisor) {
    Agrupador* matriz_separada = agrupar_elementos(m1, m2, threadsOuProcessosDivisor);

    ThreadData* arrayProcessos = matriz_separada->elementosAgrupados;
    int quantidadeProcessos = matriz_separada->quantidadeExecutores;

    for (int i = 0; i < quantidadeProcessos; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("Erro no fork");
            exit(1);
        } else if (pid == 0) {
            ThreadData* data = malloc(sizeof(ThreadData));
            *data = arrayProcessos[i];
            
            processar_elementos(data);

            _exit(0);
        }
    }

    for (int i = 0; i < quantidadeProcessos; i++) {
        wait(NULL);
    }
    
    for (int i = 0; i < quantidadeProcessos; i++) {
        free(arrayProcessos[i].elementos);
    }
    free(matriz_separada->elementosAgrupados);
    free(matriz_separada);

    return NULL;
}