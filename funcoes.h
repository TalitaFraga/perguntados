#ifndef FUNCOES_H
#define FUNCOES_H

#include <gtk/gtk.h>

#define MAX_LINE_SIZE 1024
#define MAX_FIELD_SIZE 256

typedef struct perguntados {
  char pergunta[200];
  char alternativa_a[40];
  char alternativa_b[40];
  char alternativa_c[40];
  char resposta[2];
  struct perguntados *prox;
  struct perguntados *ant;
  int selecionado;
} perguntados;

typedef struct jogador {
  char nome[30];
  int pontos;
  struct jogador *prox;
  struct jogador *ant;
} jogador;

void criar_lista_perguntas(perguntados **head);
int quantidade_perguntas(FILE *file);
int jogar(perguntados **head, int total_perguntas, char *nome_jogador);
void gravar_pontuacao(char *nome_jogador, int pontos);
void remover(perguntados **head, perguntados *aux);
void criar_ranking(jogador **head);
int cont_ranking(jogador **head);
void ordenar_ranking(jogador **head, int cont);
void exibir_ranking(jogador **head);
int introducao(jogador **head);
void limparListaRanking(jogador **head);

#endif
