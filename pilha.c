/* pilha.c
implementa as funcoes da pilha
usa um array estatico para evitar alocacao dinamica,
levando em consideracao que o tamanho maximo eh 40*40
*/
#include<stdio.h>
#include "pilha.h"

/* inicializa a pilha (topo=-1 é pilha vazia) */
void pilha_inicializar(Pilha *p){
	p->topo=-1;
}

/* retorna 1 se estiver vazia, 0 caso contrario */
int pilha_vazia(Pilha *p){
	return p->topo==-1;
}

/* retorna 1 se estiver cheia, 0 caso contrario */
int pilha_cheia(Pilha *p){
	return p->topo==MAX_PILHA - 1;
}

/* empilha uma posicao, retorna 1 caso sucesso, 0 se cheia */
int pilha_empilhar(Pilha *p, Posicao pos){
	if(pilha_cheia(p)){
		printf("[ERRO] Pilha cheia!\n");
		return 0;
	}
	p->topo++;
	p->dados[p->topo]=pos; /*os dados de p, na posicao onde está o topo, se tornam os dados de pos*/
	return 1;	
}

/* desempilha e retorna a posicao do topo */
Posicao pilha_desempilhar(Pilha *p){
	Posicao pos={-1, -1}; /* posicao invalida como sentinela */
	if(pilha_vazia(p)){
		printf("[ERRO] Tentativa de desempilhar pilha vazia!\n");
		return pos;
	}
	pos = p->dados[p->topo]; /*-1,-1 recebe os dados de p, na posicao de onde esta o topo, tornando a posicao invalida*/
	p->topo--; /*retrocede o topo */
	return pos; /*retorna pois cada posicao desempilhada eh uma posicao do caminho percorrido */	
}

/* apenas consulta o topo */
Posicao pilha_topo(Pilha *p){
	Posicao pos={-1, -1};
	if(!pilha_vazia(p)){
		pos=p->dados[p->topo];
	}
	return pos;
}