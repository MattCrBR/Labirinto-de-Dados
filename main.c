/* main.c
ponto de entrada do programa "Labirinto de Dados"
responsabilidades:
	1. inicializar o gerador de numeros aleatorios
	2. carregar o labirinto do arquivo "mapa.txt"
	3. inicializar a mochila e a pilha do caminho
	4. chamar o algoritmo de busca completa (todos os caminhos)
	5. exibir o melhor resultado (maior o numero de moeadas)
	6. liberar memoria alocada
para trocar o labirinto, edite ARQUIVO_MAPA
*/

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include "labirinto.h"
#include "pilha.h"
#include "mochila.h"
#include "busca.h"

/*arquivos de entrada e saida */
#define ARQUIVO_MAPA "mapa.txt"
#define ARQUIVO_SOLUCAO "solucao.txt"

/* delay entre cada passo da visualizacao (em milissegundos)
0 = sem pausa (recomendado para busca completa, mais rapido)
50 = levemente animado 
100 = mais lento, facil de acompanhar (labirintos pequenos)
*/
#define DELAY_MS 50

int main(void){
	/* inicializa o gerador de numeros aleatorios com a hora atual */
	srand((unsigned int)time(NULL));
	
	/*1. carrega o labirinto */
	Labirinto lab;
	if(!labirinto_carregar(&lab, ARQUIVO_MAPA)){
		printf("[FATAL] Nao foi possivel carregar o labirinto, encerrando\n");
		return 1;
	}
	printf("Labirinto carregado: %d linhas x %d colunas\n", lab.linhas, lab.colunas);
	printf("Inicio: (%d,%d) | Saida: (%d, %d)\n", lab.inicio_linha, lab.inicio_coluna, lab.saida_linha, lab.saida_coluna);	
	printf("\nATENCAO: a busca explorara todos os caminhos possiveis\n");
	printf("para encontrar o que maximiza o numero de tesouros.\n");
	printf("Pressione ENTER para iniciar...\n");
	getchar();
	
	/*2. inicializa as estruturas de dados */
	Mochila mochila;
	mochila_inicializar(&mochila);
	Pilha caminho;
	pilha_inicializar(&caminho);
	
	/* estrutura que guardara o melhor resultado encontrado */
	ResultadoBusca melhor;
	melhor.encontrou=0; //ainda nao achou nenhum caminho
	mochila_inicializar(&melhor.mochila);
	pilha_inicializar(&melhor.caminho);
	
	/*3. executa o backtracking completo */
	printf("\n[BUSCA] Explorando todos os caminhos...\n\n");
	busca_backtracking(&lab, &mochila, &caminho, lab.inicio_linha, lab.inicio_coluna, DELAY_MS, &melhor);
	
	/*4. aplica o melhor caminho no mapa e exibe o resultado */
	printf("\n");
	printf("<------------------------------------------------------------------------------------------------\n");
	if(melhor.encontrou){
		printf("        MISSAO CUMPRIDA! MELHOR CAMINHO ENCONTRADO!      \n");
        printf("------------------------------------------------------------------------------------------------>\n\n");
        
        /* marca o melhor caminho no mapa com '*' */
        busca_aplicar_melhor(&lab, &melhor);
        
        /* imprime o mapa final com o caminho marcado */
        labirinto_imprimir(&lab, melhor.mochila.quantidade, melhor.mochila.total);
        printf("\n--- MOCHILA FINAL (MELHOR RESULTADO) ---\n");
        mochila_imprimir(&melhor.mochila);
        printf("\n--- CAMINHO PERCORRIDO ---\n");
        printf("Tamanho do caminho: %d passos\n", melhor.caminho.topo + 1);
        
        /* salva a solucao em arquivo */
        labirinto_salvar_solucao(&lab, ARQUIVO_SOLUCAO);
	}
	else {
		printf("    SEM SAIDA ENCONTRADA!     \n");
        printf("<------------------------------------------------------------------------------------------------\n");
        printf("O labirinto nao possui caminho valido de P ate S\n");
	}
	/* 5. libera memoria */
	mochila_liberar(&mochila);
	mochila_liberar(&melhor.mochila); //libera copia do melhor resultado
	return 0;
}