/* busca. c
backtracking completo: explora todos os caminhos e guarda o melhor
o valor de cada 'T' eh gerado uma unica vez ao carregar (em labirinto_carregar) e
fica guardado em lab->valores_tesouro[], isso garante que comparar caminhos 
diferentes seja justo, o mesmo tesouro sempre vale o mesmo, independente da 
ordem de exploracao

para evitar a remocao de um tesouro da tentativa anterior caso dois tesouros 
tivessem o mesmo valor, usamos uma flag 'removido' para parar apos a primeira 
remocao, garantindo que apenas um no seja removido

pilha_empilhar na entrada de cada celula, pilha_desempilhar ao sair
o snapshot do melhor cmainho eh feito por copia de struct (valor), o que eh 
seguro porque Pilha usa array estatico
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include "busca.h"
#include "labirinto.h"
#include "mochila.h"
#include "pilha.h"

/* clock_t é um tipo de dado do time.h, funciona como um int mas eh usado
especificamente pra guardar valores de tempo do processador
clock() retorna quantos ticks o processador deu desde o inicio do programa
o processador da CLOCKS_PER_SEC ticks por segundo, geralmente 1000 ticks = 1s
a funcao pega o tempo inicial naquele momento do processador, no while, subtrai
o tempo ATUAL com o inicial para ver a diferenca de tempo, e compara com o ms
para criar um delay
*/
void dormir(int ms){
	clock_t inicio = clock(); //pega o tempo atual em ticks do processador
	while((clock()-inicio)<(ms*CLOCKS_PER_SEC / 1000)); //fica em loop ate o tempo em ms passar
}

static int direcao_linha[4]={-1, 1, 0, 0}; //-1: cima, 1: baixo
static int direcao_coluna[4]={0, 0, -1, 1}; //-1: esquerda, 1: direita

static int pode_mover(Labirinto *lab, int linha, int col){
	
	/* verifica se a posicao esta dentro dos limites do labirinto 
	se a linha eh menor que 0 ou eh maior/igual a qunatidade de linhas do
	labirinto, retorna 0, o mesmo vale pra colunas*/
	if(linha<0 || linha>=lab->linhas) return 0; 
	if(col<0 || col>=lab->colunas) return 0;
	
	/*verifica se a celula eh navegavel, pode mover pra corredor, tesouro,
	armadilha ou saida, nao pode mover pra parede nem celula ja visitada */
	char c = labirinto_get(lab, linha, col);
	return(c==CORREDOR || c==TESOURO || c==ARMADILHA || c==SAIDA);
}

/*copia profunda da lista encadeada da mochila
necessario para salvar um snapshot independente ao encontrar a saida, se
simplesmente fizesse melhor->mochila=*mochila, os dois ponteiros inicio 
apontariam pro mesmo lugar na memoria, e quando o backtraking modificasse a
original, a copia tambem mudaria */
static void mochila_copiar(Mochila *dest, Mochila *orig){
	/* libera qualquer coisa que ja tenha no destino e copia os contadores */
	mochila_liberar(dest);
	dest->total=orig->total;
	dest->quantidade=orig->quantidade;
	dest->inicio=NULL;
	
	/* ptr eh um ponteiro pra ponteiro, ele comeca apontando pro inicio da 
	mochila destino, a ideia eh que a cada no criado, ptr avanca pra apontar
	pro campo proximo do ultimo no criado, assim cada novo no eh encaixado
	no lugar certo automaticamente */
	NoTesouro **ptr=&dest->inicio;
	NoTesouro *atual=orig->inicio;
	while(atual!=NULL){
		NoTesouro *novo = (NoTesouro *)malloc(sizeof(NoTesouro));
		if(!novo){
			printf("[ERRO] Falha ao copiar mochila!\n");
			return;
		}
		novo->valor=atual->valor;
		novo->proximo=NULL;
		
		/*encaixa o no novo no lugar que ptr aponta, depois avanca ptr pro campo 
		proximo do novo no, e avanca atual pro proximo no da lista original*/
		*ptr=novo;
		ptr=&novo->proximo;
		atual=atual->proximo;
	}
}

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
void busca_backtracking(Labirinto *lab, Mochila *mochila, Pilha *caminho, int linha, int coluna, int delay_ms, ResultadoBusca *melhor){
	/* --- caso base: chegamos na saida --- */
	if(linha==lab->saida_linha && coluna == lab->saida_coluna){ //verifica se posicao atual eh a saida
		Posicao pos = {linha, coluna};
		pilha_empilhar(caminho, pos); //empilha a posicao da saida no caminho
		
		/*se eh o primeiro caminho encontrado, ou se tem mais moedas que o
		melhor anterior, atualiza o melhor resultado
		a pilha pode ser copiada diretamente com *caminho porque usa um 
		array estatico */
		if(!melhor->encontrou || mochila->total>melhor->mochila.total){
			melhor->encontrou=1;
			melhor->caminho=*caminho;
			/* copia da pilha (array estatico) */
			mochila_copiar(&melhor->mochila, mochila);
			printf("[MELHOR] Total: %d moedas (%d tesouros) | Passos: %d\n", mochila->total, mochila->quantidade, caminho->topo+1);
		}
		/* remove a saida do carrinho e retorna, continua explorando outros 
		caminhos, nao encerra o programa, apenas essa chamada recursiva */
		pilha_desempilhar(caminho);
		return;
	}
	
	/*salva o conteudo original (celula atual) e marca como visitado pra 
	nao passar duas vezes no mesmo lugar */
	char celula_original = labirinto_get(lab, linha, coluna);
	labirinto_set(lab, linha, coluna, VISITADO);
	
	/* empilha a posicao atual no caminho em construcao */
	Posicao pos = {linha, coluna};
	pilha_empilhar(caminho, pos);
	
	/*visualizacao*/
	labirinto_imprimir(lab, mochila->quantidade, mochila->total);
	if(delay_ms>0) dormir(delay_ms);
	
	/* explora as 4 direcoes */
	for(int d=0;d<4;d++){
		int nl=linha+direcao_linha[d];
		int nc=coluna+direcao_coluna[d];
		if(!pode_mover(lab, nl, nc)) continue; // se nao puder, pula pra proxima direacao
		
		/*salva o que tem na celula destino e inicializa as variaveis de controle
		do backtrack com -1*/
		char celula_destino = labirinto_get(lab, nl, nc);
		int valor_adicionado = -1;
		int valor_perdido = -1;
		
		/* se eh tesouro, pega o valor fixo que foi gerado no carregamento do mapa
		e insere na mochila */
		if(celula_destino == TESOURO){
			valor_adicionado=labirinto_get_valor(lab, nl, nc);
			mochila_inserir(mochila, valor_adicionado);
			printf("[TESOURO] Coletado! Valor: %d moedas\n", valor_adicionado);
		}
		
		/* se eh armadilha, perde o menor tesouro da mochila (*inicio) e salva o
		valor perdido pra poder devolver no backtrack */
		if(celula_destino==ARMADILHA){
			valor_perdido=mochila_remover_primeiro(mochila);
			if(valor_perdido>0){
				printf("[ARMADILHA] Perdeu %d moedas!\n", valor_perdido);
			}
		}
		/* move o personagem para nova posicao visualmente */
		labirinto_set(lab, nl, nc, PERSONAGEM);
		
		/* chamada recursiva */
		busca_backtracking(lab, mochila, caminho, nl, nc, delay_ms, melhor);
		
		/* o backtracking comeca aqui, restaura a celula destino ao que era 
		antes, desfaz o movimento, para que outro caminho possa passar por ela */
		labirinto_set(lab, nl, nc, celula_destino);
		
		/* se tinha coletado um tesouro nessa tentativa, remove ele da mochila
		percorre a lista procurando o no com aquele valor e remove apenas um 
		usando o break
		*/
		if(valor_adicionado>0){
			NoTesouro *ant=NULL;
			NoTesouro *cur=mochila->inicio;
			while(cur!=NULL){
				if(cur->valor==valor_adicionado){
					
					/*se ant eh NULL significa que o no esta na cabeca da lista
					entao avanca o inicio, senao, conecta o no anterior diretamente
					ao proximo, pulando o no removido */
					if(ant==NULL){
						mochila->inicio=cur->proximo;
					}
					/* o no nao eh a cabeca, esta no meio ou final da lista */
					else {
						ant->proximo=cur->proximo;
						mochila->total-=cur->valor;
						mochila->quantidade--;
						free(cur);
						break; // remove apenas um no
					}
				}
				/*avanca o current e o anterior caso nao atinjam as condicoes */
				ant = cur;
				cur = cur->proximo;
			}
		}
		/* se tinha caido numa armadilha, devolve o tesouro que foi perdido */
		if(valor_perdido > 0){
			mochila_inserir(mochila, valor_perdido);
		}
	}
	/* restaura a celula atual e remove a posicao do caminho
	backtrack completo */
	labirinto_set(lab, linha, coluna, celula_original);
	pilha_desempilhar(caminho);
	//aqui a funcao termina e retorna automaticamente pras outras chamadas
}

/*aplica o caminho do melhor resultado no mapa, marcando as celulas com
CAMINHO ('*') para exibicao e salvamento corretos, exceto a saida e o
personagem que mantem seus simbolos originais */
void busca_aplicar_melhor(Labirinto *lab, ResultadoBusca *melhor){
	if(!melhor->encontrou) return; //nao encontrou nenhum caminho, n faz nada e retorna
	Pilha *p = &melhor->caminho; //cria um ponteiro pra pilha do melhor caminho
	
	/*percorre a pilha do indice 0 ate o topo, pra cada posicao pega a linha e coluna
	armazenadas */
	for(int i=0;i<=p->topo;i++){
		int l=p->dados[i].linha;
		int c=p->dados[i].coluna;
		
		/*pega o caractere daquela posicao no mapa, se nao for a saida nem o personagem,
		marca com '*' */
		char cel=labirinto_get(lab, l, c);
		if(cel!=SAIDA && cel!=PERSONAGEM){
			labirinto_set(lab, l, c, CAMINHO);
		}
	}
}