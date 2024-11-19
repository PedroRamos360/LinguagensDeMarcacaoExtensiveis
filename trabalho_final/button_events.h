#ifndef BUTTON_EVENTS_H
#define BUTTON_EVENTS_H

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <glib.h>
#include <libxml/parser.h>

static void on_load_button_clicked(GtkWidget *widget, gpointer data)
{
    GtkWidget *text_view = GTK_WIDGET(data);
    GtkWidget *dialog;
    gchar *file_path;
    xmlDocPtr doc;
    xmlChar *xml_content;
    int xml_size;
    GtkTextBuffer *buffer;
    g_print("Carregar NF-e clicked!\n");
    dialog = gtk_file_chooser_dialog_new("Abrir Arquivo XML",
                                         GTK_WINDOW(gtk_widget_get_toplevel(widget)),
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
            xmlDocDumpFormatMemory(doc, &xml_content, &xml_size, 1);
            g_print("Conteudo do XML:\n%s\n", (char *)xml_content);
            buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
            gtk_text_buffer_set_text(buffer, (const gchar *)xml_content, xml_size);
            xmlFree(xml_content);
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
