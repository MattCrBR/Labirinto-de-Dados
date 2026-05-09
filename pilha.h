/*
pilha.h
definicao da estrutura da pilha usada no backtracking
a pilha guarda as posicoes (linha, coluna) ja visitadas
durante a busca pelo caminho no labirinto
*/
#ifndef PILHA_H
#define PILHA_H

/* tamanoh maximo (40*40=1600 celulas) */
#define MAX_PILHA 1600

/* uma posicao no labirinto */
typedef struct {
	int linha;
	int coluna;
} Posicao;

/* estrutura da pilha */
typedef struct {
	Posicao dados[MAX_PILHA];
	int topo;
} Pilha;

/* inicializa a pilha (topo=-1 é pilha vazia) */
void pilha_inicializar(Pilha *p);

/* retorna 1 se estiver vazia, 0 caso contrario */
int pilha_vazia(Pilha *p);

/* retorna 1 se estiver cheia, 0 caso contrario */
int pilha_cheia(Pilha *p);

/* empilha uma posicao, retorna 1 caso sucesso, 0 se cheia */
int pilha_empilhar(Pilha *p, Posicao pos);

/* desempilha e retorna a posicao do topo */
Posicao pilha_desempilhar(Pilha *p);

/* apenas consulta o topo */
Posicao pilha_topo(Pilha *p);

#endif