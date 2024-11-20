#ifndef BUTTON_EVENTS_H
#define BUTTON_EVENTS_H

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <glib.h>
#include <libxml/parser.h>
#include "xml_to_json.h"

xmlChar *xml_content;

static void on_load_button_clicked(GtkWidget *widget, gpointer data)
{
    GtkWidget *text_view = GTK_WIDGET(data);
    GtkWidget *dialog;
    gchar *file_path;
    xmlDocPtr doc;
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
            gchar *content_str = (gchar *)xml_content;
            gchar *new_content = g_strstr_len(content_str, xml_size, "\n");
            if (new_content != NULL)
            {
                new_content++;
                buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
                gtk_text_buffer_set_text(buffer, new_content, strlen(new_content));
            }
            else
            {
                buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
                gtk_text_buffer_set_text(buffer, content_str, xml_size);
            }
            xmlFreeDoc(doc);
        }
        g_free(file_path);
    }

    gtk_widget_destroy(dialog);
}

static void on_convert_button_clicked(GtkWidget *widget, gpointer data)
{
    char *json_str;
    g_print("Converter NF-e para JSON clicked!\n");
    printf("XML:\n%s\n", (const char *)xml_content);
    json_str = xml_to_json((const char *)xml_content);

    GtkWidget *text_view = GTK_WIDGET(data);
    GtkTextBuffer *buffer;
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    gtk_text_buffer_set_text(buffer, json_str, strlen(json_str));

    printf("JSON:\n%s\n", json_str);
}

static void on_process_button_clicked(GtkWidget *widget, gpointer data)
{
    g_print("Processar NF-e clicked!\n");
}

#endif // BUTTON_EVENTS_H
