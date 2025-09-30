#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "gerenciar-matriz/gerenciar-matriz.h"
#include "multiplicao-matriz/multiplicao-matriz.h"

void validarMatriz(int qtdColunasM1, int qtdLinhasM2) {
    if (qtdColunasM1 != qtdLinhasM2) {
        fprintf(stderr, "Matriz 1 e Matriz 2 não podem ser multiplicadas, pois o número de colunas da Matriz 1 é diferente do número de linhas da Matriz 2.\n");
        exit(EXIT_FAILURE);
    }
}

int main() {
    int max_threads = sysconf(_SC_NPROCESSORS_ONLN);
    printf("Número máximo de threads: %d\n", max_threads);

    // =============== TESTE E1 - DIFERENTES TAMANHOS ===============
    printf("\n=== TESTE E1 - DIFERENTES TAMANHOS ===\n");
    
    int tamanhos[] = {100, 200, 400, 800, 1600};
    int num_tamanhos = sizeof(tamanhos) / sizeof(tamanhos[0]);
    
    for (int t = 0; t < num_tamanhos; t++) {
        int tamanho = tamanhos[t];
        printf("\n--- Tamanho %dx%d ---\n", tamanho, tamanho);
        
        Matriz* m1 = alocar_matriz(tamanho, tamanho);
        Matriz* m2 = alocar_matriz(tamanho, tamanho);
        gerar_matriz_aleatoria(m1);
        gerar_matriz_aleatoria(m2);
        
        int divisor = (tamanho * tamanho) / 4;
        char nomeAnalise[50];
        snprintf(nomeAnalise, sizeof(nomeAnalise), "E1_%dx%d", tamanho, tamanho);
        
        for(int i = 0; i < 10; i++){
            printf("Execução %d/10\n", i+1);
            multiplicar_matrizes(m1, m2, "S", nomeAnalise, 1);
            multiplicar_matrizes(m1, m2, "PT", nomeAnalise, divisor);
            multiplicar_matrizes(m1, m2, "PP", nomeAnalise, divisor);
        }
        
        desalocar_matriz(m1);
        desalocar_matriz(m2);
    }

    // =============== TESTE E2 - DIFERENTES VALORES DE P ===============
    printf("\n=== TESTE E2 - DIFERENTES VALORES DE P ===\n");
    
    int tamanho_fixo = 1600; // Tamanho que faz o sequencial demorar ~2 minutos
    printf("--- Matriz %dx%d ---\n", tamanho_fixo, tamanho_fixo);
    
    Matriz* m1_e2 = alocar_matriz(tamanho_fixo, tamanho_fixo);
    Matriz* m2_e2 = alocar_matriz(tamanho_fixo, tamanho_fixo);
    gerar_matriz_aleatoria(m1_e2);
    gerar_matriz_aleatoria(m2_e2);
    
    int P_base = tamanho_fixo * tamanho_fixo;
    int valores_P[] = {P_base/4, P_base/2, P_base, 2*P_base, 4*P_base};
    int num_valores_P = sizeof(valores_P) / sizeof(valores_P[0]);
    
    for (int p = 0; p < num_valores_P; p++) {
        int divisor = valores_P[p];
        printf("\n--- P = %d ---\n", divisor);
        
        char nomeAnalise[50];
        snprintf(nomeAnalise, sizeof(nomeAnalise), "E2_P_%d", divisor);
        
        for(int i = 0; i < 10; i++){
            printf("Execução %d/10\n", i+1);
            multiplicar_matrizes(m1_e2, m2_e2, "PT", nomeAnalise, divisor);
            multiplicar_matrizes(m1_e2, m2_e2, "PP", nomeAnalise, divisor);
        }
    }

    printf("\n=== TODOS OS TESTES CONCLUÍDOS ===\n");
    return 0;
}