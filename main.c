#include <unistd.h>
#include <stdio.h>
#include "./gerenciar-matriz/gerenciar-matriz.h"

void validarMatriz(int qtdColunasM1, int qtdLinhasM2) {  // Remover parâmetros não usados
    if (qtdColunasM1 != qtdLinhasM2) {
        fprintf(stderr, "Matriz 1 e Matriz 2 não podem ser multiplicadas, pois o número de colunas da Matriz 1 é diferente do número de linhas da Matriz 2.\n");
        exit(EXIT_FAILURE);
    }
}

int main() {
    int max_threads = sysconf(_SC_NPROCESSORS_ONLN);
    printf("Número máximo de threads: %d\n", max_threads);
    
    int qtdLinhasM1, qtdColunasM1, qtdLinhasM2, qtdColunasM2;
    printf("Digite as dimensões das matrizes: (1º Matriz: linhas1 colunas1. 2º Matriz: linhas2 colunas2)\n");
    scanf("%d %d %d %d", &qtdLinhasM1, &qtdColunasM1, &qtdLinhasM2, &qtdColunasM2);
    validarMatriz(qtdColunasM1, qtdLinhasM2);

    Matriz* matrizEsquerda = alocar_matriz(qtdLinhasM1, qtdColunasM1);
    Matriz* matrizDireita = alocar_matriz(qtdLinhasM2, qtdColunasM2);

    char tipoExecucao;
    printf("Escolha o tipo de execução - (S)equencial ou (PT)aralela Thread, (PP)aralela Processos: ");
    scanf(" %c", &tipoExecucao);

    if (tipoExecucao == 'S' || tipoExecucao == 's') {
        gerar_matriz_aleatoria(matrizEsquerda);
        gerar_matriz_aleatoria(matrizDireita);
        salvar_matriz_arquivo(matrizEsquerda, "_original");
        salvar_matriz_arquivo(matrizDireita, "_original");
    } else if (tipoExecucao == 'P' || tipoExecucao == 'p') {
        char nomeArquivoM1[50], nomeArquivoM2[50];
        printf("Digite os nomes dos arquivos das matrizes (ex: 3x3.txt):\n");
        scanf("%s %s", nomeArquivoM1, nomeArquivoM2);
        
        matrizEsquerda = carregar_matriz_arquivo(nomeArquivoM1);
        matrizDireita = carregar_matriz_arquivo(nomeArquivoM2);
        
        if (!matrizEsquerda || !matrizDireita) {
            fprintf(stderr, "Erro ao carregar as matrizes dos arquivos.\n");
            return EXIT_FAILURE;
        }
    } else {
        fprintf(stderr, "Tipo de execução inválido. Use 'S' para sequencial ou 'P' para paralelo.\n");
        return EXIT_FAILURE;
    }


    return 0;
}