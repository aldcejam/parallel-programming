#include <unistd.h>
#include <stdio.h>
#include "gerenciar-matriz/gerenciar-matriz.h"
#include "multiplicao-matriz/multiplicao-matriz.h" 

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
    
    char opcao;
    printf("Todos os lados iguais? (s/n): ");
    scanf(" %c", &opcao);
    
    if (opcao == 's' || opcao == 'S') {
        int lado;
        printf("Digite o tamanho do lado: ");
        scanf("%d", &lado);
        
        qtdLinhasM1 = lado;
        qtdColunasM1 = lado;
        qtdLinhasM2 = lado;
        qtdColunasM2 = lado;
    } else {
        printf("Digite as dimensões das matrizes: (1º Matriz: linhas1 colunas1. 2º Matriz: linhas2 colunas2)\n");
        scanf("%d %d %d %d", &qtdLinhasM1, &qtdColunasM1, &qtdLinhasM2, &qtdColunasM2);
    }
    
    validarMatriz(qtdColunasM1, qtdLinhasM2);

    Matriz* matrizEsquerda = alocar_matriz(qtdLinhasM1, qtdColunasM1);
    gerar_matriz_aleatoria(matrizEsquerda);

    Matriz* matrizDireita = alocar_matriz(qtdLinhasM2, qtdColunasM2);
    gerar_matriz_aleatoria(matrizDireita);

    printf("Qual nome da analise? (Ex: Matriz_3-3): ");
    char nomeAnalise[50];
    scanf("%49s", nomeAnalise);

/*     Matriz* matrizProduto = multiplicar_matrizes(matrizEsquerda, matrizDireita, "S", nomeAnalise, 2);
    Matriz* matrizProdutoPT = multiplicar_matrizes(matrizEsquerda, matrizDireita, "PT", nomeAnalise, 3); */
    Matriz* matrizProdutoPP = multiplicar_matrizes(matrizEsquerda, matrizDireita, "PP", nomeAnalise, 3);

    return 0;
}