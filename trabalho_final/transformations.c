#include <gtk/gtk.h>
#include "xml_to_json.h"
#include "validate_json.h"

static void on_back_button_clicked(GtkButton *button, gpointer user_data)
{
    GtkStack *stack = GTK_STACK(user_data);
    gtk_stack_set_visible_child_name(stack, "main-view");
}

void on_nf_clicked(GtkButton *button, gpointer user_data)
{
    GtkWidget **views = (GtkWidget **)user_data;
    const char *xml_content = (const char *)g_object_get_data(G_OBJECT(button), "xml_content");
    const char *json_content = (const char *)g_object_get_data(G_OBJECT(button), "json_content");

    GtkTextBuffer *xml_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(views[0]));
    gtk_text_buffer_set_text(xml_buffer, xml_content, -1);

    GtkTextBuffer *json_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(views[1]));
    gtk_text_buffer_set_text(json_buffer, json_content, -1);
}

void set_tab1_content(GtkWidget *tab_view)
{
    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(tab_view), hbox, TRUE, TRUE, 0);

    GtkWidget *list_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_pack_start(GTK_BOX(hbox), list_vbox, FALSE, FALSE, 0);

    GtkWidget *xml_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(xml_view), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(xml_view), FALSE);
    GtkWidget *xml_scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(xml_scroll), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(xml_scroll), xml_view);
    gtk_box_pack_start(GTK_BOX(hbox), xml_scroll, TRUE, TRUE, 0);

    GtkWidget *json_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(json_view), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(json_view), FALSE);
    GtkWidget *json_scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(json_scroll), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(json_scroll), json_view);
    gtk_box_pack_start(GTK_BOX(hbox), json_scroll, TRUE, TRUE, 0);

    GtkWidget *views[2];
    views[0] = xml_view;
    views[1] = json_view;

    char *nfe_files[] = {
        read_file("notas/NFE1.xml"),
        read_file("notas/NFE2.xml"),
        read_file("notas/NFE3.xml"),
        read_file("notas/NFE4.xml"),
        read_file("notas/NFE5.xml"),
        read_file("notas/NFE6.xml")};

    for (int i = 0; i < 6; i++)
    {
        if (!nfe_files[i])
        {
            fprintf(stderr, "Error reading XML for NFE%d\n", i + 1);
            continue;
        }

        char *json_content = xml_to_json(nfe_files[i]);
        if (!json_content)
        {
            fprintf(stderr, "Error converting XML to JSON for NFE%d\n", i + 1);
            free(nfe_files[i]);
            continue;
        }

        GtkWidget *button = gtk_button_new_with_label(g_strdup_printf("NFE%d", i + 1));
        g_object_set_data_full(G_OBJECT(button), "xml_content", nfe_files[i], g_free);
        g_object_set_data_full(G_OBJECT(button), "json_content", json_content, g_free);
        g_signal_connect(button, "clicked", G_CALLBACK(on_nf_clicked), views);
        gtk_box_pack_start(GTK_BOX(list_vbox), button, FALSE, FALSE, 0);
    }
}

void set_tab2_content(GtkWidget *tab_view)
{
    GtkWidget *label = gtk_label_new("Conteúdo específico da Aba 2");
    gtk_box_pack_start(GTK_BOX(tab_view), label, TRUE, TRUE, 0);
}

void set_tab3_content(GtkWidget *tab_view)
{
    GtkWidget *label = gtk_label_new("Conteúdo específico da Aba 3");
    gtk_box_pack_start(GTK_BOX(tab_view), label, TRUE, TRUE, 0);
}

void set_tab4_content(GtkWidget *tab_view)
{
    GtkWidget *label = gtk_label_new("Conteúdo específico da Aba 4");
    gtk_box_pack_start(GTK_BOX(tab_view), label, TRUE, TRUE, 0);
}

void set_tab5_content(GtkWidget *tab_view)
{
    GtkWidget *label = gtk_label_new("Conteúdo específico da Aba 5");
    gtk_box_pack_start(GTK_BOX(tab_view), label, TRUE, TRUE, 0);
}

void add_tab_views(GtkNotebook *notebook)
{
    for (int i = 1; i <= 5; i++)
    {
        GtkWidget *tab_view = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
        gtk_notebook_append_page(notebook, tab_view, gtk_label_new(g_strdup_printf("Aba %d", i)));

        switch (i)
        {
        case 1:
            set_tab1_content(tab_view);
            break;
        case 2:
            set_tab2_content(tab_view);
            break;
        case 3:
            set_tab3_content(tab_view);
            break;
        case 4:
            set_tab4_content(tab_view);
            break;
        case 5:
            set_tab5_content(tab_view);
            break;
        }
    }
}

GtkWidget *create_transformations_view(GtkStack *stack)
{
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    GtkWidget *back_button = gtk_button_new_with_label("Voltar para Main");
    gtk_box_pack_start(GTK_BOX(vbox), back_button, FALSE, FALSE, 0);
    g_signal_connect(back_button, "clicked", G_CALLBACK(on_back_button_clicked), stack);

    GtkWidget *notebook = gtk_notebook_new();
    add_tab_views(GTK_NOTEBOOK(notebook));
    gtk_box_pack_start(GTK_BOX(vbox), notebook, TRUE, TRUE, 0);

    return vbox;
}
