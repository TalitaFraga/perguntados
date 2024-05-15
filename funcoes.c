
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

    if (novo != NULL) {
      strcpy(novo->pergunta, pergunta);
      strcpy(novo->alternativa_a, alternativa_a);
      strcpy(novo->alternativa_b, alternativa_b);
      strcpy(novo->alternativa_c, alternativa_c);
      strcpy(novo->resposta, resposta);
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
        gtk_window_set_default_size(GTK_WINDOW(dialog), 500, 100);
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
        while (gtk_events_pending()) gtk_main_iteration();
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
          dialog = gtk_message_dialog_new(NULL, flags, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Resposta correta! Você ganhou 1 ponto. ✅");
          gtk_dialog_run(GTK_DIALOG(dialog));
          while (gtk_events_pending()) gtk_main_iteration();
          gtk_widget_destroy(dialog);
          remover(head,atual);
          return 1;
        } else {
          dialog = gtk_message_dialog_new(NULL, flags, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Resposta incorreta ❌. A resposta correta era: %s", resposta_correta);
          gtk_dialog_run(GTK_DIALOG(dialog));
          while (gtk_events_pending()) gtk_main_iteration();
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
    char *token = strtok(line, ",");

    jogador *novo = (jogador *)malloc(sizeof(jogador));
    // printf("alocação ok\n");
    if (novo == NULL) {
    printf("Erro na alocação de memória\n");
    return;
  }
    if (novo != NULL) {
      strcpy(novo->nome, token);
      token = strtok(NULL, ",");
      if (token == NULL) {
    return;
}
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
  GtkWidget *dialog, *text_view, *content_area, *scrolled_window;
  GtkTextBuffer *buffer;
  GtkDialogFlags flags;
  flags = GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT;
  dialog = gtk_dialog_new_with_buttons("Ranking", NULL, flags, "OK", GTK_RESPONSE_OK, NULL);
  gtk_window_set_default_size(GTK_WINDOW(dialog), 400, 200);

  content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
  scrolled_window = gtk_scrolled_window_new(NULL, NULL);
  gtk_container_add(GTK_CONTAINER(content_area), scrolled_window);

  gtk_widget_set_size_request(scrolled_window, 400, 200);

  text_view = gtk_text_view_new();
  gtk_container_add(GTK_CONTAINER(scrolled_window), text_view);

  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

  while (aux != NULL) {
    char ranking_info[50];
    sprintf(ranking_info, "%s %d\n", aux->nome, aux->pontos);
    gtk_text_buffer_insert_at_cursor(buffer, ranking_info, -1);
    aux = aux->prox;
  }

  gtk_widget_show_all(dialog);
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
  while (gtk_events_pending()) gtk_main_iteration();
}



int introducao(jogador **head) {
  GtkWidget *dialog, *content_area, *label;
  GtkDialogFlags flags;
  flags = GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT;
  dialog = gtk_dialog_new_with_buttons("Bem vindos ao perguntados", NULL, flags, "Jogar", 1, "Ranking", 2, "Sair", 3, NULL);
  gtk_window_set_default_size(GTK_WINDOW(dialog), 300, 100);
  
  label = gtk_label_new("OLÁ! 😊 PRONTOS PARA COMEÇAR O JOGO?");
  content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
  gtk_container_add(GTK_CONTAINER(content_area), label);
  
  gtk_widget_show_all(dialog);
  int result = gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);

  return result;
}


void limparListaRanking(jogador **head) {
    jogador *atual = *head;
    jogador *proximo;

    while (atual != NULL) {
        proximo = atual->prox; 
        free(atual);            
        atual = proximo;       
    }

    *head = NULL;  
}