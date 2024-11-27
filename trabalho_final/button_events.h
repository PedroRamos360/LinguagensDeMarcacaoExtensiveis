#ifndef BUTTON_EVENTS_H
#define BUTTON_EVENTS_H

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <glib.h>
#include <libxml/parser.h>
#include "validate_json.h"
#include "xml_to_json.h"

typedef struct
{
    GtkWidget *text_view;
    GtkWidget *schema_view;
} TextViews;

char *nfe_content;
char *schema_content;

static void load_text_file(GtkWidget *widget, gpointer data, char **output)
{
    GtkWidget *text_view = GTK_WIDGET(data);
    GtkWidget *dialog;
    gchar *file_path;
    GtkTextBuffer *buffer;
    gsize file_size;
    GError *error = NULL;

    g_print("Carregar arquivo de texto clicked!\n");
    dialog = gtk_file_chooser_dialog_new("Abrir Arquivo de Texto",
                                         GTK_WINDOW(gtk_widget_get_toplevel(widget)),
                                         GTK_FILE_CHOOSER_ACTION_OPEN,
                                         "_Cancelar", GTK_RESPONSE_CANCEL,
                                         "_Abrir", GTK_RESPONSE_ACCEPT,
                                         NULL);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
    {
        file_path = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        g_print("Arquivo selecionado: %s\n", file_path);

        if (!g_file_get_contents(file_path, output, &file_size, &error))
        {
            g_print("Erro ao carregar o arquivo de texto: %s\n", error->message);
            g_clear_error(&error);
        }
        else
        {
            g_print("Arquivo carregado com sucesso! Tamanho: %zu bytes\n", file_size);

            buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
            gtk_text_buffer_set_text(buffer, *output, file_size);
        }

        g_free(file_path);
    }

    gtk_widget_destroy(dialog);
}

static void on_load_button_clicked(GtkWidget *widget, gpointer data)
{
    load_text_file(widget, data, &nfe_content);
}

static void on_load_schema_clicked(GtkWidget *widget, gpointer data)
{
    load_text_file(widget, data, &schema_content);
}

static void on_convert_button_clicked(GtkWidget *widget, gpointer data)
{
    char *json_str;
    g_print("Converter NF-e para JSON clicked!\n");
    json_str = xml_to_json(nfe_content);
    GtkWidget *text_view = GTK_WIDGET(data);
    GtkTextBuffer *buffer;
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    gtk_text_buffer_set_text(buffer, json_str, strlen(json_str));
    *nfe_content = *json_str;
}

static void on_validate_button_clicked(GtkButton *button, gpointer user_data)
{
    int result = validate_json(nfe_content, schema_content);
    GtkWidget *parent = gtk_widget_get_toplevel(GTK_WIDGET(button));

    GtkMessageType message_type = result ? GTK_MESSAGE_INFO : GTK_MESSAGE_ERROR;
    const char *message_text = result ? "Validation succeeded!" : "Validation failed!";

    GtkWidget *dialog = gtk_message_dialog_new(
        GTK_WINDOW(parent),
        GTK_DIALOG_DESTROY_WITH_PARENT,
        message_type, GTK_BUTTONS_OK,
        "%s",
        message_text);

    gtk_window_set_title(GTK_WINDOW(dialog), result ? "Success" : "Error");

    gtk_dialog_run(GTK_DIALOG(dialog));

    gtk_widget_destroy(dialog);
}

#endif // BUTTON_EVENTS_H
