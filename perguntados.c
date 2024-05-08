#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LINE_SIZE 1024
#define MAX_FIELD_SIZE 256
#define MAX_FIELDS 5

typedef struct perguntados {
  char pergunta[50];
  char alternativa_a[10];
  char alternativa_b[10];
  char alternativa_c[10];
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

int main() {
  srand(time(0));
  int quantidade_perguntas, num_jogadores;
  perguntados *head = NULL;
  jogador *head2 = NULL;
  printf("Digite a quantidade de jogadores (1 ou 2):\n");
  scanf("%d", &num_jogadores);
  printf("Digite a quantidade de perguntas que deseja jogar:\n");
  scanf("%d", &quantidade_perguntas);

  criar_lista_perguntas(&head);

  char jogador1[50], jogador2[50];
  int pontos1 = 0, pontos2 = 0;

  printf("Digite o nome do primeiro jogador:\n");
  scanf("%s", jogador1);

  if (num_jogadores == 2) {
    printf("Digite o nome do segundo jogador:\n");
    scanf("%s", jogador2);
  }

  for (int i = 0; i < quantidade_perguntas; i++) {
    printf("\nTurno do %s:\n", jogador1);
    pontos1 += jogar(&head, quantidade_perguntas, jogador1);
    if (num_jogadores == 2) {
      printf("\nTurno do %s:\n", jogador2);
      pontos2 += jogar(&head, quantidade_perguntas, jogador2);
    }
  }

  printf("\n%s %d pontos\n", jogador1, pontos1);
  if (num_jogadores == 2) {
    printf(" %s %d pontos\n", jogador2, pontos2);
  }

  gravar_pontuacao(jogador1, pontos1);
  if (num_jogadores == 2) {
    gravar_pontuacao(jogador2, pontos2);
  }

  criar_ranking(&head2);
  int cont = cont_ranking(&head2);
  ordenar_ranking(&head2, cont);
  exibir_ranking(&head2);

  return 0;
}

void criar_lista_perguntas(perguntados **head) {
  FILE *file = fopen("perguntas.csv", "r");
  if (file == NULL) {
    printf("Erro ao abrir o arquivo\n");
    return;
  }
  char line[MAX_LINE_SIZE];
  while (fgets(line, sizeof(line), file) != NULL) {
    // printf("Linha lida: %s", line);
    char *token = strtok(line, "|");

    perguntados *novo = (perguntados *)malloc(sizeof(perguntados));
    // printf("alocação ok\n");

    if (novo != NULL) {
      strcpy(novo->pergunta, token);
      token = strtok(NULL, "|");
      strcpy(novo->alternativa_a, token);
      token = strtok(NULL, "|");
      strcpy(novo->alternativa_b, token);
      token = strtok(NULL, "|");
      strcpy(novo->alternativa_c, token);
      token = strtok(NULL, "|");
      strcpy(novo->resposta, token);
      novo->prox = NULL;

      if (*head == NULL) {
        novo->ant = NULL;
        *head = novo;
      } else {
        perguntados *aux = *head;
        while (aux->prox != NULL) {
          aux = aux->prox;
        }
        aux->prox = novo;
        novo->ant = aux;
        // printf("Novo nó adicionado ao final da lista.\n");
      }
    }
  }
  fclose(file);
}

int quantidade_perguntas(FILE *file) {
  int linhas = 0;
  char ch;
  while (!feof(file)) {
    ch = fgetc(file);
    if (ch == '\n') {
      linhas++;
    }
  }
  rewind(file);
  return linhas;
}

void remover(perguntados **head, perguntados *aux) {
  if (aux == *head) {
    *head = aux->prox;
    if (*head) {
      (*head)->ant = NULL;
    }
  } else {
    if (aux->ant != NULL) {
      aux->ant->prox = aux->prox;
    }
    if (aux->prox != NULL) {
      aux->prox->ant = aux->ant;
    }
  }
  free(aux);
}

int jogar(perguntados **head, int total_perguntas, char *nome_jogador) {
  srand(time(0));
  int num_pergunta = rand() % total_perguntas + 1;

  int i = 1;
  perguntados *atual = *head;
  while (atual != NULL) {
    if (i == num_pergunta) {
      printf("Pergunta %d:\n", i);
      printf("Pergunta: %s\n", atual->pergunta);
      printf("Alternativa A: %s\n", atual->alternativa_a);
      printf("Alternativa B: %s\n", atual->alternativa_b);
      printf("Alternativa C: %s\n", atual->alternativa_c);
      printf("\n");
      atual->selecionado = 1;

      char resposta;
      printf("Digite sua resposta (A, B ou C):\n");
      scanf(" %c", &resposta);

      if (resposta == atual->resposta[0]) {
        printf("Resposta correta! Você ganhou 1 ponto.\n");
        remover(head, atual);
        return 1;
      } else {
        printf("Resposta incorreta. A resposta correta era: %c\n",
               atual->resposta[0]);
        remover(head, atual);
        return 0;
      }
      break;
    }
    atual = atual->prox;
    i++;
  }
  return 0;
}

void gravar_pontuacao(char *nome_jogador, int pontos) {
  FILE *file = fopen("pontuacao.csv", "a");
  if (file == NULL) {
    printf("Erro ao abrir o arquivo\n");
    return;
  }
  fprintf(file, "%s, %d pontos\n", nome_jogador, pontos);
  fclose(file);
}

void criar_ranking(jogador **head) {
  FILE *file = fopen("pontuacao.csv", "r");
  if (file == NULL) {
    printf("Erro ao abrir o arquivo\n");
    return;
  }
  char line[MAX_LINE_SIZE];
  while (fgets(line, sizeof(line), file) != NULL) {
    // printf("Linha lida: %s", line);
    char *token = strtok(line, ",");

    jogador *novo = (jogador *)malloc(sizeof(jogador));
    // printf("alocação ok\n");

    if (novo != NULL) {
      strcpy(novo->nome, token);
      token = strtok(NULL, ",");
      novo->pontos = atoi(token);
      novo->prox = NULL;

      if (*head == NULL) {
        novo->prox = NULL;
        *head = novo;
      } else {
        jogador *aux = *head;
        while (aux->prox != NULL) {
          aux = aux->prox;
        }
        aux->prox = novo;
        // printf("Novo nó adicionado ao final da lista.\n");
      }
    }
  }
  fclose(file);
}

int cont_ranking(jogador **head) {
  if (*head == NULL) {
    return 0;
  }
  jogador *aux = *head;
  int cont = 0;
  while (aux != NULL) {
    cont++;
    aux = aux->prox;
  }
  return cont;
}

void ordenar_ranking(jogador **head, int cont) {
  int trocado;
  do {
    jogador *aux = *head;
    jogador *ant_aux = NULL; // Ponteiro para o elemento anterior
    trocado = 0;

    while (aux->prox != NULL) {
      if (aux->pontos > aux->prox->pontos) {
        trocado = 1;
        jogador *tmp = aux->prox;
        aux->prox = tmp->prox;
        tmp->prox = aux;

        if (ant_aux != NULL) {
          ant_aux->prox = tmp;
        } else {
          *head = tmp; // Atualiza o cabeçalho da lista se o primeiro elemento foi trocado
        }

        // Atualiza os ponteiros anterior se for uma lista duplamente encadeada
        if (aux->prox != NULL) {
          aux->prox->ant = aux; // Ajuste do ponteiro anterior do novo próximo de aux
        }
        tmp->ant = ant_aux;
        aux->ant = tmp;

        ant_aux = tmp; // Atualizar o prev_aux para a posição correta
      } else {
        ant_aux = aux;
        aux = aux->prox;
      }
    }
  } while (trocado);
}


void exibir_ranking(jogador **head) {
  jogador *aux = *head;
  while (aux != NULL) {
    printf("%s %d\n", aux->nome, aux->pontos);
    aux = aux->prox;
  }
}