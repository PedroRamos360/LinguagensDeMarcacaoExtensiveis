#include "dashboard.h"

static void on_back_button_clicked(GtkButton *button, gpointer user_data)
{
    GtkStack *stack = GTK_STACK(user_data);
    gtk_stack_set_visible_child_name(stack, "main-view");
}

// Helper function to create and add a "General" tab
void add_general_view(GtkNotebook *notebook)
{
    GtkWidget *general_view = gtk_label_new("General View Content");
    gtk_notebook_append_page(notebook, general_view, gtk_label_new("General"));
}

// Helper function to create and add an "NFs" tab
void add_nf_view(GtkNotebook *notebook)
{
    GtkWidget *nf_view = gtk_label_new("NF View Content");
    gtk_notebook_append_page(notebook, nf_view, gtk_label_new("NFs"));
}

// Helper function to create and add a "Taxes" tab
void add_taxes_view(GtkNotebook *notebook)
{
    GtkWidget *taxes_view = gtk_label_new("Taxes View Content");
    gtk_notebook_append_page(notebook, taxes_view, gtk_label_new("Taxes"));
}

// Helper function to create and add a "Suppliers" tab
void add_suppliers_view(GtkNotebook *notebook)
{
    GtkWidget *suppliers_view = gtk_label_new("Suppliers View Content");
    gtk_notebook_append_page(notebook, suppliers_view, gtk_label_new("Suppliers"));
}

// Helper function to create and add a "Transporters" tab
void add_transporters_view(GtkNotebook *notebook)
{
    GtkWidget *transporters_view = gtk_label_new("Transporters View Content");
    gtk_notebook_append_page(notebook, transporters_view, gtk_label_new("Transporters"));
}

GtkWidget *create_dashboard_view(GtkStack *stack)
{
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    GtkWidget *back_button = gtk_button_new_with_label("Back to Main");
    gtk_box_pack_start(GTK_BOX(vbox), back_button, FALSE, FALSE, 0);
    g_signal_connect(back_button, "clicked", G_CALLBACK(on_back_button_clicked), stack);

    GtkWidget *notebook = gtk_notebook_new();

    add_general_view(GTK_NOTEBOOK(notebook));
    add_nf_view(GTK_NOTEBOOK(notebook));
    add_taxes_view(GTK_NOTEBOOK(notebook));
    add_suppliers_view(GTK_NOTEBOOK(notebook));
    add_transporters_view(GTK_NOTEBOOK(notebook));

    gtk_box_pack_start(GTK_BOX(vbox), notebook, TRUE, TRUE, 0);

    return vbox;
}
