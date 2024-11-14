#include <gtk/gtk.h>
#include "button_events.h"

static void on_app_activate(GApplication *app, gpointer user_data)
{
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *load_button;
    GtkWidget *validate_button;
    GtkWidget *process_button;

    window = gtk_application_window_new(GTK_APPLICATION(app));
    gtk_window_set_title(GTK_WINDOW(window), "Portal de Análise de NF-e");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    load_button = gtk_button_new_with_label("Carregar NF-e");
    validate_button = gtk_button_new_with_label("Validar NF-e");
    process_button = gtk_button_new_with_label("Processar NF-e");

    // Conecta cada botão à função de callback correspondente
    g_signal_connect(load_button, "clicked", G_CALLBACK(on_load_button_clicked), NULL);
    g_signal_connect(validate_button, "clicked", G_CALLBACK(on_validate_button_clicked), NULL);
    g_signal_connect(process_button, "clicked", G_CALLBACK(on_process_button_clicked), NULL);

    gtk_grid_attach(GTK_GRID(grid), load_button, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), validate_button, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), process_button, 0, 1, 2, 1);

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
