#ifndef BUTTON_EVENTS_H
#define BUTTON_EVENTS_H

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <glib.h>
#include <libxml/parser.h>

static void on_load_button_clicked(GtkWidget *widget, gpointer data)
{
    GtkWidget *dialog;
    gchar *file_path;
    xmlDocPtr doc;
    g_print("Carregar NF-e clicked!\n");
    dialog = gtk_file_chooser_dialog_new("Abrir Arquivo XML",
                                         GTK_WINDOW(data),
                                         GTK_FILE_CHOOSER_ACTION_OPEN,
                                         "_Cancelar", GTK_RESPONSE_CANCEL,
                                         "_Abrir", GTK_RESPONSE_ACCEPT,
                                         NULL);
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
    {
        file_path = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        g_print("Arquivo selecionado: %s\n", file_path);
        doc = xmlParseFile(file_path);
        if (doc == NULL)
        {
            g_print("Erro ao carregar o XML!\n");
        }
        else
        {
            g_print("XML carregado com sucesso!\n");
            xmlFreeDoc(doc);
        }
        g_free(file_path);
    }

    gtk_widget_destroy(dialog);
}

static void on_validate_button_clicked(GtkWidget *widget, gpointer data)
{
    g_print("Validar NF-e clicked!\n");
}

static void on_process_button_clicked(GtkWidget *widget, gpointer data)
{
    g_print("Processar NF-e clicked!\n");
}

#endif // BUTTON_EVENTS_H
