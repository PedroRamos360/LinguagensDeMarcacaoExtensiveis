#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <gtk/gtk.h>

static void on_back_button_clicked(GtkButton *button, gpointer user_data)
{
    GtkWidget *window = GTK_WIDGET(user_data);

    GtkWidget *main_view = GTK_WIDGET(g_object_get_data(G_OBJECT(window), "main-view"));

    gtk_container_remove(GTK_CONTAINER(window), gtk_bin_get_child(GTK_BIN(window)));
    gtk_container_add(GTK_CONTAINER(window), main_view);

    gtk_widget_show_all(window);
}

GtkWidget *create_dashboard_view(GtkWidget *window)
{
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

    GtkWidget *back_button = gtk_button_new_with_label("Back");
    gtk_box_pack_start(GTK_BOX(vbox), back_button, FALSE, FALSE, 0);
    g_signal_connect(back_button, "clicked", G_CALLBACK(on_back_button_clicked), window);

    GtkWidget *notebook = gtk_notebook_new();

    GtkWidget *general_view = gtk_label_new("General View Content");
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), general_view, gtk_label_new("General"));

    GtkWidget *nf_view = gtk_label_new("NF View Content");
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), nf_view, gtk_label_new("NFs"));

    GtkWidget *taxes_view = gtk_label_new("Taxes View Content");
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), taxes_view, gtk_label_new("Taxes"));

    GtkWidget *suppliers_view = gtk_label_new("Suppliers View Content");
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), suppliers_view, gtk_label_new("Suppliers"));

    GtkWidget *transporters_view = gtk_label_new("Transporters View Content");
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), transporters_view, gtk_label_new("Transporters"));

    gtk_box_pack_start(GTK_BOX(vbox), notebook, TRUE, TRUE, 0);

    return vbox;
}

#endif // DASHBOARD_H
