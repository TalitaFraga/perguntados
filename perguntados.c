#include <gtk/gtk.h>
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

 int main(int argc, char **argv) {
  srand(time(0));
  GtkWidget *dialog, *spin_button, *content_area, *entry;
  GtkDialogFlags flags;
  gint num_jogadores, num_perguntas;
  gint pontos1=0;
  gint pontos2=0;
  gchar *nome_jogador1, *nome_jogador2;
  perguntados *head = NULL;
  jogador *head2 = NULL;
  gtk_init(&argc, &argv);

  // Pergunta a quantidade de jogadores
  flags = GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT;
  dialog = gtk_dialog_new_with_buttons("Quantidade de jogadores", NULL, flags, "OK", GTK_RESPONSE_OK, NULL);
  gtk_window_set_default_size(GTK_WINDOW(dialog), 400, 200);
  content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
  spin_button = gtk_spin_button_new_with_range(1, 2, 1);
  gtk_container_add(GTK_CONTAINER(content_area), spin_button);
  gtk_widget_show_all(dialog);
  gtk_dialog_run(GTK_DIALOG(dialog));
  num_jogadores = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_button));
  gtk_widget_destroy(dialog);

  // Pergunta o nome do primeiro jogador
  dialog = gtk_dialog_new_with_buttons("Nome do primeiro jogador", NULL, flags, "OK", GTK_RESPONSE_OK, NULL);
  gtk_window_set_default_size(GTK_WINDOW(dialog), 400, 200);
  content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
  entry = gtk_entry_new();
  gtk_container_add(GTK_CONTAINER(content_area), entry);
  gtk_widget_show_all(dialog);
  gtk_dialog_run(GTK_DIALOG(dialog));
  nome_jogador1 = g_strdup(gtk_entry_get_text(GTK_ENTRY(entry)));
  gtk_widget_destroy(dialog);

  // Pergunta o nome do segundo jogador, se houver
  if (num_jogadores == 2) {
    dialog = gtk_dialog_new_with_buttons("Nome do segundo jogador", NULL, flags, "OK", GTK_RESPONSE_OK, NULL);
    gtk_window_set_default_size(GTK_WINDOW(dialog), 400, 200);
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    entry = gtk_entry_new();
    gtk_container_add(GTK_CONTAINER(content_area), entry);
    gtk_widget_show_all(dialog);
    gtk_dialog_run(GTK_DIALOG(dialog));
    nome_jogador2 = g_strdup(gtk_entry_get_text(GTK_ENTRY(entry)));
    gtk_widget_destroy(dialog);
  }

  // Pergunta a quantidade de perguntas
  dialog = gtk_dialog_new_with_buttons("Quantidade de perguntas", NULL, flags, "OK", GTK_RESPONSE_OK, NULL);
  gtk_window_set_default_size(GTK_WINDOW(dialog), 400, 200);
  content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
  spin_button = gtk_spin_button_new_with_range(1, 100, 1); // Substituir 100 pela quantidade máxima de perguntas
  gtk_container_add(GTK_CONTAINER(content_area), spin_button);
  gtk_widget_show_all(dialog);
  gtk_dialog_run(GTK_DIALOG(dialog));
  num_perguntas = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_button));
  gtk_widget_destroy(dialog);
  criar_lista_perguntas(&head);

   for (int i = 0; i < num_perguntas; i++) {
  GtkWidget *dialog;
  GtkDialogFlags flags = GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT;

  dialog = gtk_message_dialog_new(NULL, flags, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "\nTurno do %s:\n", nome_jogador1);
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
  pontos1 += jogar(&head, num_perguntas, nome_jogador1);

  if (num_jogadores == 2) {

    dialog = gtk_message_dialog_new(NULL, flags, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "\nTurno do %s:\n", nome_jogador2);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    pontos2 += jogar(&head, num_perguntas, nome_jogador2);
  }
}
    gravar_pontuacao(nome_jogador1, pontos1);
    if (num_jogadores == 2) {
      gravar_pontuacao(nome_jogador2, pontos2);
    }
    sleep(1);
    

  g_print("Número de jogadores: %d\n", num_jogadores);
  g_print("Nome do primeiro jogador: %s\n", nome_jogador1);
  if (num_jogadores == 2) {
    g_print("Nome do segundo jogador: %s\n", nome_jogador2);
  }
  g_print("Número de perguntas: %d\n", num_perguntas);

    criar_ranking(&head2);
    int cont=cont_ranking(&head2);
    ordenar_ranking(&head2,cont);
    exibir_ranking(&head2);
    
  return 0;
}
    
    
void criar_lista_perguntas(perguntados **head) {
  FILE *file = fopen("perguntas.csv", "r");
  if (file == NULL) {
    printf("Erro ao abrir o arquivo\n");
    return;
  }
  char pergunta[MAX_FIELD_SIZE];
  char alternativa_a[MAX_FIELD_SIZE];
  char alternativa_b[MAX_FIELD_SIZE];
  char alternativa_c[MAX_FIELD_SIZE];
  char resposta[MAX_FIELD_SIZE];

  while (fscanf(file, "%[^|]|%[^|]|%[^|]|%[^|]|%s", pergunta, alternativa_a, alternativa_b, alternativa_c, resposta) != EOF) {
    perguntados *novo = (perguntados *)malloc(sizeof(perguntados));
    printf("alocação ok\n");

    if (novo != NULL) {
    printf("Entrei aqui!\n");
      strcpy(novo->pergunta, pergunta);
      strcpy(novo->alternativa_a, alternativa_a);
      strcpy(novo->alternativa_b, alternativa_b);
      strcpy(novo->alternativa_c, alternativa_c);
      strcpy(novo->resposta, resposta);
      novo->prox = NULL;

      printf("Passei por aqui!\n");

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
        printf("Novo nó adicionado ao final da lista.\n");
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

int jogar(perguntados **head, int num_perguntas, char *nome_jogador) {
  srand(time(0));
  int num_pergunta = rand() % num_perguntas + 1;

  int i = 1;
  perguntados *atual = *head;

  while (atual != NULL) {
    if (i == num_pergunta) {
      GtkWidget *dialog, *label, *content_area;
      GtkDialogFlags flags;

      flags = GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT;
      dialog = gtk_dialog_new_with_buttons(atual->pergunta, NULL, flags, atual->alternativa_a, 1, atual->alternativa_b, 2, atual->alternativa_c, 3, NULL);
      content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
      gtk_widget_show_all(dialog);

      int result = gtk_dialog_run(GTK_DIALOG(dialog));
      char resposta;
      switch (result) {
        case 1:
          resposta = 'A';
          break;
        case 2:
          resposta = 'B';
          break;
        case 3:
          resposta = 'C';
          break;
        default:
          resposta = ' ';
          break;
      }

      gtk_widget_destroy(dialog);

      char *resposta_correta;
      switch (atual->resposta[0]) {
        case 'A':
          resposta_correta = atual->alternativa_a;
          break;
        case 'B':
          resposta_correta = atual->alternativa_b;
          break;
        case 'C':
          resposta_correta = atual->alternativa_c;
          break;
        default:
          resposta_correta = " ";
          break;
      }

      if (resposta == atual->resposta[0]) {
        dialog = gtk_message_dialog_new(NULL, flags, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Resposta correta! Você ganhou 1 ponto.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        remover(head,atual);
        return 1;
      } else {
        dialog = gtk_message_dialog_new(NULL, flags, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Resposta incorreta. A resposta correta era: %s", resposta_correta);
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        remover(head,atual);
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
      if (aux->pontos < aux->prox->pontos) {
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
    g_print("%s %d\n", aux->nome, aux->pontos);
    aux = aux->prox;
  }
}