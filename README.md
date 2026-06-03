# Labirinto de Dados

> AC2 — Estrutura de Dados | Facens | Prof. Luciano Freire

Um caçador de tesouros precisa atravessar um labirinto antigo. O programa encontra automaticamente **o caminho que maximiza o valor coletado**, explorando todas as rotas possíveis via backtracking recursivo.

---

## O que o programa faz

O labirinto é lido de um arquivo `.txt`. O algoritmo percorre cada célula, coleta tesouros, reage a armadilhas e registra o melhor resultado ao chegar na saída. Ao final, o caminho vencedor é exibido no terminal e salvo em `solucao.txt`.

O diferencial é que **não para no primeiro caminho encontrado** — continua explorando tudo e só encerra quando não há mais caminhos possíveis.

---

## Compilar e rodar

```bash
gcc main.c labirinto.c busca.c pilha.c mochila.c -o labirinto
./labirinto
```

O programa lê `mapa.txt` na pasta atual. Para trocar o labirinto, substitua o arquivo ou edite a constante `ARQUIVO_MAPA` em `main.c`.

---

## Ajustando a velocidade

Em `main.c`:

```c
#define DELAY_MS 50
```

| Valor | Efeito |
|-------|--------|
| `0` | Sem pausa — recomendado para labirintos grandes |
| `50` | Animação leve |
| `100` | Passo a passo, fácil de acompanhar |

---

## Formato do mapa

```
10x10
##########
#P      T#
# ###### #
#   A    #
#########
#        #
#  ##### #
#       S#
##########
```

A primeira linha define as dimensões (`LINHASxCOLUNAS`). O mapa pode ter até 40×40.

| Caractere | Significado |
|-----------|-------------|
| `P` | Início |
| `S` | Saída |
| `#` | Parede |
| ` ` | Corredor |
| `T` | Tesouro (1–100 moedas, valor sorteado ao carregar) |
| `A` | Armadilha (remove o menor tesouro da mochila) |

---

## Arquitetura

```
main.c
└── busca.c          ← backtracking recursivo
    ├── labirinto.c  ← leitura e manipulação do mapa
    ├── pilha.c      ← rastreia o caminho (array estático)
    └── mochila.c    ← gerencia tesouros (lista encadeada ordenada)
```

A mochila é mantida em **ordem crescente** para que o menor tesouro fique sempre na frente — minimizando perdas ao cair em armadilhas.

---

## Equipe

| Nome | 
|------|
| Matheus Cravo Purcena Gonçalves |
| Samuel de Moraes Delgado |
| Giovanni de Luca Gomieri |
| Guilherme Alves dos Santos |