/* mochila.h
a mochila eh uma lista encadeada ordenada de tesouros
a ordenacao garante que o tesouro de menor valor fique sempre na primeira
posicao da lista, conforme exige o enunciado
isso minimiza a perda ao cair numa armadilha, pois o item removido eh sempre o de
menor valor
*/
#ifndef MOCHILA_H
#define MOCHILA_H

/* no da lista encadeada representando um tesouro */
typedef struct NoTesouro{
	int valor; /* valor das moedas (1 a 100) */
	struct NoTesouro *proximo; /*ponteiro para o proximo no */
} NoTesouro;

typedef struct{
	NoTesouro *inicio; /*sempre aponta para o tesouro de menor valor */
	int total; /* soma de todos os valores na mochila */
	int quantidade; /* numero de tesouros */
} Mochila;

/* inicializa a mochila vazia */
void mochila_inicializar(Mochila *m);

/* insere um tesouro na posicao correta da lista (ordem crescente)
garante que o menor valor fique na frente */
void mochila_inserir(Mochila *m, int valor);

/* remove o tesouro da primeira posicao (menor valor)
chamado quando o personagem cai em uma armadilha
retorna o valor removido, ou -1 se a mochila estiver vazia */
int mochila_remover_primeiro(Mochila *m);

/* imprime o conteudo a mochila */
void mochila_imprimir(Mochila *m);

/* libera toda a memoria alocada pelos nos da lista */
void mochila_liberar(Mochila *m);

/* retorna 1 se a mochila estiver vazia */
int mochila_vazia(Mochila *m);

#endif