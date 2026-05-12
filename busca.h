/*busca.h
declaracao da funcao de busca recursiva por backtracking
etapas do backtracking:
	1. parte da posicao do personagem 'P'
	2. tenta mover em 4 direcoes (cima, baixo, esquerda, direita)
	3. se uma direcao leva a uma celula valida, move pra ela e chama a funcao recursivamente
	4. se nenhuma direcao leva a saida, volta (backtrack) e tenta outra direcao
	5. explora todos os caminhos possiveis e guarda o melhor (aquele com maior total de moeadas na mochila)
a pilha eh usada para registrar o caminho percorrido ate a saida
*/
#ifndef BUSCA_H
#define BUSCA_H

#include "labirinto.h"
#include "mochila.h"
#include "pilha.h"

/* armazena o melhor resultado encontrado durante a busca completa 
atualizado sempre que um novo caminho com mais moedas eh encontrado */
typedef struct{
	Pilha caminho; //copia do caminho que levou ao melhor resultado
	Mochila mochila; //copia da mochila no momento da melhor saida
	int encontrou; //1 se ao menos um caminho foi encontrado
} ResultadoBusca;

/*funcao principal de busca recursiva, esta funcao nao para ao encontrar o 
primeiro caminho, ela explora todas as possibilidades e atualiza 'melhor' sempre
que encontrar um caminho com mais moedas
parametros:
	lab = ponteiro para o labirinto
	mochila = ponteiro para a mochila do personagem (estado atual)
	caminho = pilha que acumula o caminho em exploracao
	linha, coluna = posicao atual do personagem
	delay_ms = tempo de pausa entre passo (0 = sem delay)
	melhor = ponteiro para o struct que guarda o melhor resultado
retorna void pois o resultado eh armazenado em 'melhor) */
void busca_backtracking(Labirinto *lab, Mochila *mochila, Pilha *caminho, int linha, int coluna, int delay_ms, ResultadoBusca *melhor);

/*aplica o caminho do melhor resultado no mapa, marcando as celulas com
CAMINHO ('*') para exibicao e salvamento corretos */
void busca_aplicar_melhor(Labirinto *lab, ResultadoBusca *melhor);

#endif