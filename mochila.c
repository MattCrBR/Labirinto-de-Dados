/* mochila.c
implementacao da mochila como lista encadead ordenada (crescente)
a insercao ordenada substitui a necessidade de um algoritmo de ordenacao separado
cada tesouro ja entra na posicao certa */

#include<stdio.h>
#include<stdlib.h>
#include "mochila.h"

/* inicializa a mochila vazia */
void mochila_inicializar(Mochila *m){
	m->inicio=NULL; //lista vazia, nenhum no
	m->total=0; // nenhuma moeda
	m->quantidade=0; // nenhum tesouro
}

/* insere um tesouro na posicao correta da lista (ordem crescente)
garante que o menor valor fique na frente 
percorre a lista ate encontrar o ponto de insercao correto:
 -se o novo valor for menor que todos = insere na primeira posicao
 -caso contrario = insere entre o no atual e o proximo
*/
void mochila_inserir(Mochila *m, int valor){
	/* aloca memoria para o novo no */
	NoTesouro *novo = (NoTesouro *)malloc(sizeof(NoTesouro));
	if(novo==NULL){
		printf("[ERRO] Falha ao alocar memoria para tesouro!\n");
		return;
	}
	novo->valor=valor; // preenche o novo NoTesouro com o valor inserido
	novo->proximo=NULL; // sem proximo por enquanto
	
	/* caso 1: lista vazia OU novo valor eh menor que o primeiro (que sempre aponta para o menor valor) */
	if(m->inicio==NULL || valor < m->inicio->valor){
		novo->proximo=m->inicio;
		m->inicio=novo;
		/*ANTES:  [10] -> [50] -> NULL
		  INSERE: 5
		  DEPOIS: [5] -> [10] -> [50] -> NULL
		*/  
	}
	else {
		/* caso 2: percorre ate achar a posicao certa */
		NoTesouro *atual = m->inicio; //preenche o NoTesouro atual com o no do inicio da mochila
		while(atual->proximo != NULL && atual->proximo->valor <= valor){
			atual=atual->proximo;
			//caso o proximo nao seja NULL, e seu valor seja <= ao valor inserido, o atual se torna o proximo
		}
		/* insere novo entre atual e atual->proximo */
		novo->proximo=atual->proximo; //o proximo de novo se torna o proximo de atual
		atual->proximo=novo; //proximo de atual se torna o novo
		/* ANTES:  [10] -> [50] -> NULL
		   INSERE: 30
		   atual para em [10] pois [50] > 30
		   DEPOIS: [10] -> [30] -> [50] -> NULL
		*/
	}
	m->total+=valor;
	m->quantidade++;
}

/* remove o tesouro da primeira posicao (menor valor)
chamado quando o personagem cai em uma armadilha
retorna o valor removido, ou -1 se a mochila estiver vazia */
int mochila_remover_primeiro(Mochila *m){
	if(mochila_vazia(m)){
		printf("[AVISO] Armadilha acionada, mas a mochila esta vazia, nada foi perdido. \n");
		return -1;
	}
	NoTesouro *removido = m->inicio; //preenche NoTesouro removido com o no inicio da mochila
	int valor = removido->valor; //int valor se torna o valor do no
	m->inicio=removido->proximo; // avanca a cabeca da lista
	free(removido); // libera a memoria do no removido
	m->total-=valor;
	m->quantidade--;
	return valor;
}

/* imprime o conteudo a mochila */
void mochila_imprimir(Mochila *m){
	if(mochila_vazia(m)){
		printf("  [ mochila vazia ]\n");
		return;
	}
	NoTesouro *atual=m->inicio;
	int i=1;
	while(atual!=NULL){
		printf("  [%d] %d Moedas\n", i++, atual->valor);
		atual=atual->proximo;
	}
	printf("  TOTAL: %d moedas | Itens: %d\n", m->total, m->quantidade);
}

/* libera toda a memoria alocada pelos nos da lista */
void mochila_liberar(Mochila *m){
	NoTesouro *atual = m->inicio;
	while(atual!=NULL){
		NoTesouro *prox = atual->proximo;
		free(atual);
		atual = prox;
	}
	m->inicio=NULL;
	m->total=0;
	m->quantidade=0;
}

/* retorna 1 se a mochila estiver vazia */
int mochila_vazia(Mochila *m){
	return m->inicio==NULL; //se o inicio eh NULL, ent a lista esta vazia
}