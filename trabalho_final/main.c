#include <gtk/gtk.h>
#include "button_events.h"

static void on_app_activate(GApplication *app, gpointer user_data)
{
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *load_button;
    GtkWidget *convert_json_button;
    GtkWidget *process_button;
    GtkWidget *text_view;
    GtkWidget *scrolled_window;

    window = gtk_application_window_new(GTK_APPLICATION(app));
    gtk_window_set_title(GTK_WINDOW(window), "Portal de Análise de NF-e");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    grid = gtk_grid_new();
    gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);
    gtk_container_add(GTK_CONTAINER(window), grid);

    load_button = gtk_button_new_with_label("Carregar NF-e");
    convert_json_button = gtk_button_new_with_label("Converter NF-e para JSON");
    process_button = gtk_button_new_with_label("Processar NF-e");

    text_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD);

    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(scrolled_window), text_view);
    gtk_widget_set_size_request(scrolled_window, 780, 400);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    g_signal_connect(load_button, "clicked", G_CALLBACK(on_load_button_clicked), text_view);
    g_signal_connect(convert_json_button, "clicked", G_CALLBACK(on_convert_button_clicked), text_view);
    g_signal_connect(process_button, "clicked", G_CALLBACK(on_process_button_clicked), NULL);

    gtk_grid_attach(GTK_GRID(grid), load_button, 0, 0, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), convert_json_button, 4, 0, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), process_button, 8, 0, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), scrolled_window, 0, 2, 10, 1);

    gtk_widget_show_all(window);
}

int main(int argc, char **argv)
{
    GtkApplication *app;
    int status;

    app = gtk_application_new("com.seuapp.nfe", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(on_app_activate), NULL);

    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
