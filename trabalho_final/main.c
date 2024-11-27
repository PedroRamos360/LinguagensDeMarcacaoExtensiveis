#include <gtk/gtk.h>
#include "button_events.h"
#include "dashboard.h"

static void on_dashboard_button_clicked(GtkButton *button, gpointer user_data)
{
    GtkWidget *window = GTK_WIDGET(user_data);

    GtkWidget *dashboard_view = create_dashboard_view(window);

    gtk_container_remove(GTK_CONTAINER(window), gtk_bin_get_child(GTK_BIN(window)));
    gtk_container_add(GTK_CONTAINER(window), dashboard_view);

    gtk_widget_show_all(window);
}

static void on_app_activate(GApplication *app, gpointer user_data)
{
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *load_button;
    GtkWidget *convert_json_button;
    GtkWidget *validate_button;
    GtkWidget *text_view;
    GtkWidget *schema_view;
    GtkWidget *scrolled_window;
    GtkWidget *hbox;
    GtkWidget *load_schema;
    GtkWidget *dashboard_button;

    window = gtk_application_window_new(GTK_APPLICATION(app));
    gtk_window_set_title(GTK_WINDOW(window), "Portal de Análise de NF-e");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    grid = gtk_grid_new();
    gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);
    gtk_container_add(GTK_CONTAINER(window), grid);

    load_button = gtk_button_new_with_label("Carregar NF-e");
    load_schema = gtk_button_new_with_label("Carregar Schema");
    convert_json_button = gtk_button_new_with_label("Converter NF-e para JSON");
    dashboard_button = gtk_button_new_with_label("Dashboard");
    validate_button = gtk_button_new_with_label("Validar");

    text_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD);

    schema_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(schema_view), FALSE);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(schema_view), GTK_WRAP_WORD);

    hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_set_homogeneous(GTK_BOX(hbox), TRUE);

    gtk_widget_set_hexpand(text_view, TRUE);
    gtk_widget_set_vexpand(text_view, TRUE);
    gtk_box_pack_start(GTK_BOX(hbox), text_view, TRUE, TRUE, 0);

    gtk_widget_set_hexpand(schema_view, TRUE);
    gtk_widget_set_vexpand(schema_view, TRUE);
    gtk_box_pack_start(GTK_BOX(hbox), schema_view, TRUE, TRUE, 0);

    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(scrolled_window), hbox);
    gtk_widget_set_size_request(scrolled_window, 1000, 500);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    g_signal_connect(load_button, "clicked", G_CALLBACK(on_load_button_clicked), text_view);
    g_signal_connect(convert_json_button, "clicked", G_CALLBACK(on_convert_button_clicked), text_view);
    g_signal_connect(load_schema, "clicked", G_CALLBACK(on_load_schema_clicked), schema_view);
    g_signal_connect(validate_button, "clicked", G_CALLBACK(on_validate_button_clicked), schema_view);
    g_signal_connect(dashboard_button, "clicked", G_CALLBACK(on_dashboard_button_clicked), window);

    gtk_grid_attach(GTK_GRID(grid), load_button, 0, 0, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), load_schema, 2, 0, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), convert_json_button, 4, 0, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), dashboard_button, 6, 0, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), validate_button, 8, 0, 2, 1);
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
