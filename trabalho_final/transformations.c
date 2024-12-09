#include <gtk/gtk.h>

static void on_back_button_clicked(GtkButton *button, gpointer user_data)
{
    GtkStack *stack = GTK_STACK(user_data);
    gtk_stack_set_visible_child_name(stack, "main-view");
}

GtkWidget *create_transformations_view(GtkStack *stack)
{
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    GtkWidget *back_button = gtk_button_new_with_label("Back to Main");
    gtk_box_pack_start(GTK_BOX(vbox), back_button, FALSE, FALSE, 0);
    g_signal_connect(back_button, "clicked", G_CALLBACK(on_back_button_clicked), stack);

    GtkWidget *label = gtk_label_new("Transformations View Content");
    gtk_box_pack_start(GTK_BOX(vbox), label, TRUE, TRUE, 0);

    return vbox;
}
