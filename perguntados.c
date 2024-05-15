#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "funcoes.h"

 int main(int argc, char **argv) {
  srand(time(0));
  GtkWidget *dialog, *spin_button, *content_area, *entry, *error_dialog;
  GtkDialogFlags flags;
  int num_jogadores, num_perguntas;
  
  char *nome_jogador1, *nome_jogador2;
  perguntados *head = NULL;
  jogador *head2 = NULL;
  gtk_init(&argc, &argv);
  int escolha=0;


  while(escolha != 3) {
  
    escolha = introducao(&head2);
    if (escolha == 1) {
    int pontos1=0;
    int pontos2=0;
    flags = GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT;
    dialog = gtk_dialog_new_with_buttons("Quantidade de jogadores", NULL, flags, "OK", GTK_RESPONSE_OK, NULL);
    gtk_window_set_default_size(GTK_WINDOW(dialog), 300, 100);
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    spin_button = gtk_spin_button_new_with_range(1, 2, 1);
    gtk_container_add(GTK_CONTAINER(content_area), spin_button);
    gtk_widget_show_all(dialog);
    gtk_dialog_run(GTK_DIALOG(dialog));
    num_jogadores = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_button));
    gtk_widget_destroy(dialog);
    while (gtk_events_pending()) gtk_main_iteration();

dialog = gtk_dialog_new_with_buttons("Nome do primeiro jogador", NULL, flags, "OK", GTK_RESPONSE_OK, NULL);
gtk_window_set_default_size(GTK_WINDOW(dialog), 300, 100);
content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
entry = gtk_entry_new();
gtk_container_add(GTK_CONTAINER(content_area), entry);
gtk_widget_show_all(dialog);

FILE *file;
char line[1024];
gboolean name_exists;

do {
    gtk_dialog_run(GTK_DIALOG(dialog));
    nome_jogador1 = g_strdup(gtk_entry_get_text(GTK_ENTRY(entry)));
    if (nome_jogador1 == NULL || strlen(nome_jogador1) == 0) {
      error_dialog = gtk_message_dialog_new(GTK_WINDOW(dialog),
                                            GTK_DIALOG_DESTROY_WITH_PARENT,
                                            GTK_MESSAGE_ERROR,
                                            GTK_BUTTONS_CLOSE,
                                            "Erro ao obter o nome do primeiro jogador, Digite um nome válido");
      gtk_dialog_run(GTK_DIALOG(error_dialog));
      gtk_widget_destroy(error_dialog);
    } else {
      file = fopen("pontuacao.csv", "a+");
      fseek(file, 0, SEEK_END);
      if (ftell(file) == 0) {
        name_exists = FALSE;
      } else {
        rewind(file);
        name_exists = FALSE;
        while (fgets(line, sizeof(line), file)) {
          if (strstr(line, nome_jogador1) != NULL) {
            name_exists = TRUE;
            break;
          }
        }
      }
      fclose(file);

      if (name_exists) {
        error_dialog = gtk_message_dialog_new(GTK_WINDOW(dialog),
                                              GTK_DIALOG_DESTROY_WITH_PARENT,
                                              GTK_MESSAGE_ERROR,
                                              GTK_BUTTONS_CLOSE,
                                              "O nome do jogador já existe, por favor escolha outro nome");
        gtk_dialog_run(GTK_DIALOG(error_dialog));
        gtk_widget_destroy(error_dialog);
      }
    }
} while (nome_jogador1 == NULL || strlen(nome_jogador1) == 0 || name_exists);

gtk_widget_destroy(dialog);
while (gtk_events_pending()) gtk_main_iteration();


    if (num_jogadores == 2) {
      
GtkWidget *error_dialog;
dialog = gtk_dialog_new_with_buttons("Nome do segundo jogador", NULL, flags, "OK", GTK_RESPONSE_OK, NULL);
gtk_window_set_default_size(GTK_WINDOW(dialog), 300, 100);
content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
entry = gtk_entry_new();
gtk_container_add(GTK_CONTAINER(content_area), entry);
gtk_widget_show_all(dialog);

do {
    gtk_dialog_run(GTK_DIALOG(dialog));
    nome_jogador2 = g_strdup(gtk_entry_get_text(GTK_ENTRY(entry)));
    if (nome_jogador2 == NULL || strlen(nome_jogador2) == 0) {
      error_dialog = gtk_message_dialog_new(GTK_WINDOW(dialog),
                                            GTK_DIALOG_DESTROY_WITH_PARENT,
                                            GTK_MESSAGE_ERROR,
                                            GTK_BUTTONS_CLOSE,
                                            "ERRO AO OBTER O NOME DO SEGUNDO JOGADOR, DIGITE UM NOME VALIDO");
      gtk_dialog_run(GTK_DIALOG(error_dialog));
      gtk_widget_destroy(error_dialog);
    } else if (strcmp(nome_jogador1, nome_jogador2) == 0) {
      error_dialog = gtk_message_dialog_new(GTK_WINDOW(dialog),
                                            GTK_DIALOG_DESTROY_WITH_PARENT,
                                            GTK_MESSAGE_ERROR,
                                            GTK_BUTTONS_CLOSE,
                                            "O NOME DO SEGUNDO JOGADOR DEVE SER DIFERENTE DO PRIMEIRO");
      gtk_dialog_run(GTK_DIALOG(error_dialog));
      gtk_widget_destroy(error_dialog);
    } else {
      file = fopen("pontuacao.csv", "r");
      name_exists = FALSE;
      while (fgets(line, sizeof(line), file)) {
        if (strstr(line, nome_jogador2) != NULL) {
          name_exists = TRUE;
          break;
        }
      }
      fclose(file);

      if (name_exists) {
        error_dialog = gtk_message_dialog_new(GTK_WINDOW(dialog),
                                              GTK_DIALOG_DESTROY_WITH_PARENT,
                                              GTK_MESSAGE_ERROR,
                                              GTK_BUTTONS_CLOSE,
                                              "O NOME DO JOGADOR JÁ EXISTE, POR FAVOR ESCOLHA OUTRO NOME");
        gtk_dialog_run(GTK_DIALOG(error_dialog));
        gtk_widget_destroy(error_dialog);
      }
    }
} while (nome_jogador2 == NULL || strlen(nome_jogador2) == 0 || name_exists || strcmp(nome_jogador1, nome_jogador2) == 0);

gtk_widget_destroy(dialog);
while (gtk_events_pending()) gtk_main_iteration();


    }

    dialog = gtk_dialog_new_with_buttons("QUANTIDADE DE PERGUNTAS", NULL, flags, "OK", GTK_RESPONSE_OK, NULL);
    gtk_window_set_default_size(GTK_WINDOW(dialog), 300, 100);
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    spin_button = gtk_spin_button_new_with_range(1, 100, 1); // Substituir 100 pela quantidade máxima de perguntas
    gtk_container_add(GTK_CONTAINER(content_area), spin_button);
    gtk_widget_show_all(dialog);
    gtk_dialog_run(GTK_DIALOG(dialog));
    num_perguntas = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_button));
    gtk_widget_destroy(dialog);
    while (gtk_events_pending()) gtk_main_iteration();
    criar_lista_perguntas(&head);

    for (int i = 0; i < num_perguntas; i++) {
    GtkWidget *dialog;
    GtkDialogFlags flags = GTK_DIALOG_MODAL |GTK_DIALOG_DESTROY_WITH_PARENT;

    dialog = gtk_message_dialog_new(NULL, flags, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "\nRodada do(a) %s:\n", nome_jogador1);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    while (gtk_events_pending()) gtk_main_iteration();
    pontos1 += jogar(&head, num_perguntas, nome_jogador1);

    if (num_jogadores == 2) {

      dialog = gtk_message_dialog_new(NULL, flags, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "\nRadada do(a) %s:\n", nome_jogador2);
      gtk_dialog_run(GTK_DIALOG(dialog));
      gtk_widget_destroy(dialog);
      while (gtk_events_pending()) gtk_main_iteration();
      pontos2 += jogar(&head, num_perguntas, nome_jogador2);
    }
    
  }
      gravar_pontuacao(nome_jogador1, pontos1);
      if (num_jogadores == 2) {
        gravar_pontuacao(nome_jogador2, pontos2);
      }
      sleep(1);

    limparListaRanking(&head2); 
    criar_ranking(&head2);
  

GtkWidget *image, *box;
char *message2;
if (num_jogadores == 2) {
  if (pontos1 == pontos2) {
    message2 = g_strdup_printf("PONTUAÇÃO: %s = %d, %s = %d\nEMPATE\n", nome_jogador1, pontos1, nome_jogador2, pontos2);
  }
  else{
    char *winner = pontos1 > pontos2 ? nome_jogador1 : nome_jogador2;
  
    message2 = g_strdup_printf("PONTUAÇÃO: %s = %d, %s = %d\nVENCEDOR: %s\n", nome_jogador1, pontos1, nome_jogador2, pontos2, winner);
  }
} else {
  message2 = g_strdup_printf("PONTUAÇÃO: %s = %d\n", nome_jogador1, pontos1);
}

dialog = gtk_dialog_new_with_buttons("RESULTADO DO JOGO", NULL, flags, "Jogar novamente", 1, "Sair", 2, NULL);
gtk_window_set_default_size(GTK_WINDOW(dialog), 300, 100);

box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
gtk_container_add(GTK_CONTAINER(content_area), box);

GtkWidget *label = gtk_label_new(message2);
gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);

if (num_jogadores == 2 && pontos1 != pontos2) {
  image = gtk_image_new_from_icon_name("emblem-ok-symbolic", GTK_ICON_SIZE_DIALOG);
  gtk_box_pack_start(GTK_BOX(box), image, FALSE, FALSE, 0);
}

gtk_widget_show_all(dialog);
int result = gtk_dialog_run(GTK_DIALOG(dialog));
gtk_widget_destroy(dialog);
while (gtk_events_pending()) gtk_main_iteration();


    if (result == 1) {
        escolha = 1;
        continue;
    }else if (result == 2) {
        break;
    }
  }

  else if (escolha == 2) {
    if (head2 == NULL) {
      criar_ranking(&head2);
    }
    int cont = cont_ranking(&head2);
    ordenar_ranking(&head2, cont);
    exibir_ranking(&head2);
    escolha=0;   
    continue; 
      }

  else if (escolha == 3) {
        exit(0);
      }
      
      return 0;
    }
 }
