#include "gerenciar-matriz.h"

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
        return NULL;
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

    return matriz;
}

void desalocar_matriz(Matriz* matriz) {
    if (matriz) {
        if (matriz->data) {
            for (int i = 0; i < matriz->qtdLinhas; i++) {
                free(matriz->data[i]);
            }
            free(matriz->data);
        }
        free(matriz);
    }
}

void gerar_matriz_aleatoria(Matriz* matriz) {
    if (!matriz) return;
    srand(time(NULL));

    for (int i = 0; i < matriz->qtdLinhas; i++) {
        for (int j = 0; j < matriz->qtdColunas; j++) {
            matriz->data[i][j] = (double)(rand() % 10000) / 100.0;
        }
    }
}

void salvar_matriz_arquivo(Matriz* matriz, const char* sufixo) {
    if (!matriz) return;
    
    struct stat st = {0};
    if (stat("matrizes", &st) == -1) {
        mkdir("matrizes", 0700);
    }
    
    char nome_arquivo[100];
    sprintf(nome_arquivo, "matrizes/%dx%d%s.txt", matriz->qtdLinhas, matriz->qtdColunas, sufixo);

    FILE *arquivo = fopen(nome_arquivo, "w");
    if (arquivo == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo %s para escrita.\n", nome_arquivo);
        return;
    }
    
    fprintf(arquivo, "%d %d\n", matriz->qtdLinhas, matriz->qtdColunas);
    
    for (int i = 0; i < matriz->qtdLinhas; i++) {
        for (int j = 0; j < matriz->qtdColunas; j++) {
            fprintf(arquivo, "%.4f | ", matriz->data[i][j]); 
        }
        fprintf(arquivo, "\n");
    }
    
    fclose(arquivo);
    printf("Matriz de %dx%d salva em: %s\n", matriz->qtdLinhas, matriz->qtdColunas, nome_arquivo);
}

Matriz* carregar_matriz_arquivo(const char* nome_arquivo) {
    char caminho_arquivo[200];
    
    if (strstr(nome_arquivo, "matrizes/") == NULL && strstr(nome_arquivo, "matrizes\\") == NULL) {
        sprintf(caminho_arquivo, "matrizes/%s", nome_arquivo);
    } else {
        strcpy(caminho_arquivo, nome_arquivo);
    }
    
    FILE *arquivo = fopen(caminho_arquivo, "r");
    if (arquivo == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo %s para leitura.\n", caminho_arquivo);
        return NULL;
    }

    int qtdLinhas, qtdColunas;
    
    if (fscanf(arquivo, "%d %d", &qtdLinhas, &qtdColunas) != 2) {
        fprintf(stderr, "Erro ao ler as dimensões da matriz no arquivo %s.\n", nome_arquivo);
        fclose(arquivo);
        return NULL;
    }
    
    Matriz* matriz = alocar_matriz(qtdLinhas, qtdColunas);
    if (matriz == NULL) {
        fclose(arquivo);
        return NULL;
    }

    char linha[1024];
    fgets(linha, sizeof(linha), arquivo);
    
    for (int i = 0; i < qtdLinhas; i++) {
        if (fgets(linha, sizeof(linha), arquivo) == NULL) {
            fprintf(stderr, "Erro: Linhas insuficientes no arquivo %s.\n", nome_arquivo);
            desalocar_matriz(matriz);
            fclose(arquivo);
            return NULL;
        }
        
        char *token = strtok(linha, "|");
        for (int j = 0; j < qtdColunas && token != NULL; j++) {
            while (*token == ' ') token++;
            
            char *endptr;
            double valor = strtod(token, &endptr);
            if (endptr == token) {
                fprintf(stderr, "Erro ao converter token '%s' para double.\n", token);
                desalocar_matriz(matriz);
                fclose(arquivo);
                return NULL;
            }
            
            matriz->data[i][j] = valor;
            token = strtok(NULL, "|");
        }
    }
    
    fclose(arquivo);
    printf("Matriz de %dx%d carregada de: %s\n", qtdLinhas, qtdColunas, nome_arquivo);
    return matriz;
}

void imprimir_matriz(Matriz* matriz) {
    if (!matriz) return;

    printf("Matriz %dx%d:\n", matriz->qtdLinhas, matriz->qtdColunas);
    for (int i = 0; i < matriz->qtdLinhas; i++) {
        for (int j = 0; j < matriz->qtdColunas; j++) {
            printf("%.4f | ", matriz->data[i][j]);
        }
        printf("\n");
    }
}