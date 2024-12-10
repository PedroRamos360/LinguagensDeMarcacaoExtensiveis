#include <gtk/gtk.h>
#include "xml_to_json.h"
#include "validate_json.h"

static void on_back_button_clicked(GtkButton *button, gpointer user_data)
{
    GtkStack *stack = GTK_STACK(user_data);
    gtk_stack_set_visible_child_name(stack, "main-view");
}

void display_nf(const char *nfe_json)
{
    json_error_t error;
    json_t *root = json_loads(nfe_json, 0, &error);
    if (!root)
    {
        fprintf(stderr, "Error parsing JSON: %s\n", error.text);
        return;
    }

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "NFE Details");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 600);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE); // Disable resizing

    GtkWidget *scrollable_view = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollable_view), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(window), scrollable_view);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(scrollable_view), vbox);

    GtkWidget *text_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(text_view), FALSE);
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    gtk_text_buffer_set_text(buffer, nfe_json, -1);
    gtk_box_pack_start(GTK_BOX(vbox), text_view, TRUE, TRUE, 0);

    gtk_widget_show_all(window);
    json_decref(root);
}

void on_nf_clicked(GtkButton *button, gpointer user_data)
{
    const char *nfe_json = (const char *)user_data;
    display_nf(nfe_json);
}

void set_tab1_content(GtkWidget *tab_view)
{
    GtkWidget *scrollable_view = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollable_view), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start(GTK_BOX(tab_view), scrollable_view, TRUE, TRUE, 0);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(scrollable_view), vbox);

    char *nfe_files[] = {
        xml_to_json(read_file("notas/NFE1.xml")),
        xml_to_json(read_file("notas/NFE2.xml")),
        xml_to_json(read_file("notas/NFE3.xml")),
        xml_to_json(read_file("notas/NFE4.xml")),
        xml_to_json(read_file("notas/NFE5.xml")),
        xml_to_json(read_file("notas/NFE6.xml"))};

    for (int i = 0; i < 6; i++)
    {
        if (!nfe_files[i])
        {
            fprintf(stderr, "Error converting XML to JSON for NFE%d\n", i + 1);
            continue;
        }

        GtkWidget *button = gtk_button_new_with_label(g_strdup_printf("Abrir NFE %d", i + 1));
        g_signal_connect(button, "clicked", G_CALLBACK(on_nf_clicked), g_strdup(nfe_files[i]));
        gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);
    }
}

void display_product_data(const char *product_json)
{
    json_error_t error;
    json_t *root = json_loads(product_json, 0, &error);
    if (!root)
    {
        fprintf(stderr, "Error parsing JSON: %s\n", error.text);
        return;
    }

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Product Details");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 600);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE); // Disable resizing

    GtkWidget *scrollable_view = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollable_view), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(window), scrollable_view);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(scrollable_view), vbox);

    GtkWidget *text_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(text_view), FALSE);
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    gtk_text_buffer_set_text(buffer, product_json, -1);
    gtk_box_pack_start(GTK_BOX(vbox), text_view, TRUE, TRUE, 0);

    gtk_widget_show_all(window);
    json_decref(root);
}

void on_product_button_clicked(GtkButton *button, gpointer user_data)
{
    const char *product_json = (const char *)user_data;
    display_product_data(product_json);
}

void set_tab2_content(GtkWidget *tab_view)
{
    GtkWidget *scrollable_view = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollable_view), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start(GTK_BOX(tab_view), scrollable_view, TRUE, TRUE, 0);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(scrollable_view), vbox);

    char *nfe_files[] = {
        xml_to_json(read_file("notas/NFE1.xml")),
        xml_to_json(read_file("notas/NFE2.xml")),
        xml_to_json(read_file("notas/NFE3.xml")),
        xml_to_json(read_file("notas/NFE4.xml")),
        xml_to_json(read_file("notas/NFE5.xml")),
        xml_to_json(read_file("notas/NFE6.xml"))};

    for (int i = 0; i < 6; i++)
    {
        if (!nfe_files[i])
        {
            fprintf(stderr, "Error converting XML to JSON for NFE%d\n", i + 1);
            continue;
        }

        json_error_t error;
        json_t *root = json_loads(nfe_files[i], 0, &error);
        if (!root)
        {
            fprintf(stderr, "Error parsing JSON for NFE%d: %s\n", i + 1, error.text);
            free(nfe_files[i]);
            continue;
        }

        json_t *nfeProc = json_object_get(root, "nfeProc");
        json_t *NFe = json_object_get(nfeProc, "NFe");
        json_t *infNFe = json_object_get(NFe, "infNFe");
        json_t *det = json_object_get(infNFe, "det");

        json_t *product_data = json_array();
        if (json_is_array(det))
        {
            size_t index;
            json_t *item;
            json_array_foreach(det, index, item)
            {
                json_t *prod = json_object_get(item, "prod");
                json_array_append(product_data, prod);
            }
        }
        else
        {
            json_t *prod = json_object_get(det, "prod");
            json_array_append(product_data, prod);
        }

        char *product_json = json_dumps(product_data, JSON_INDENT(4));
        json_decref(product_data);

        GtkWidget *button = gtk_button_new_with_label(g_strdup_printf("Abrir produtos para NFE %d", i + 1));
        g_signal_connect(button, "clicked", G_CALLBACK(on_product_button_clicked), g_strdup(product_json));
        gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);

        json_decref(root);
        free(nfe_files[i]);
    }
}

void display_all_products(const char *all_products_json)
{
    json_error_t error;
    json_t *root = json_loads(all_products_json, 0, &error);
    if (!root)
    {
        fprintf(stderr, "Error parsing JSON: %s\n", error.text);
        return;
    }

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "All Products");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 600);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE); // Disable resizing

    GtkWidget *scrollable_view = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollable_view), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(window), scrollable_view);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(scrollable_view), vbox);

    GtkWidget *text_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(text_view), FALSE);
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    gtk_text_buffer_set_text(buffer, all_products_json, -1);
    gtk_box_pack_start(GTK_BOX(vbox), text_view, TRUE, TRUE, 0);

    gtk_widget_show_all(window);
    json_decref(root);
}

void on_all_products_button_clicked(GtkButton *button, gpointer user_data)
{
    const char *all_products_json = (const char *)user_data;
    display_all_products(all_products_json);
}

void set_tab3_content(GtkWidget *tab_view)
{
    GtkWidget *scrollable_view = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollable_view), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start(GTK_BOX(tab_view), scrollable_view, TRUE, TRUE, 0);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(scrollable_view), vbox);

    json_t *all_products = json_array();

    char *nfe_files[] = {
        xml_to_json(read_file("notas/NFE1.xml")),
        xml_to_json(read_file("notas/NFE2.xml")),
        xml_to_json(read_file("notas/NFE3.xml")),
        xml_to_json(read_file("notas/NFE4.xml")),
        xml_to_json(read_file("notas/NFE5.xml")),
        xml_to_json(read_file("notas/NFE6.xml"))};

    for (int i = 0; i < 6; i++)
    {
        if (!nfe_files[i])
        {
            fprintf(stderr, "Error converting XML to JSON for NFE%d\n", i + 1);
            continue;
        }

        json_error_t error;
        json_t *root = json_loads(nfe_files[i], 0, &error);
        if (!root)
        {
            fprintf(stderr, "Error parsing JSON for NFE%d: %s\n", i + 1, error.text);
            free(nfe_files[i]);
            continue;
        }

        json_t *nfeProc = json_object_get(root, "nfeProc");
        json_t *NFe = json_object_get(nfeProc, "NFe");
        json_t *infNFe = json_object_get(NFe, "infNFe");
        json_t *det = json_object_get(infNFe, "det");

        if (json_is_array(det))
        {
            size_t index;
            json_t *item;
            json_array_foreach(det, index, item)
            {
                json_t *prod = json_object_get(item, "prod");
                json_array_append(all_products, prod);
            }
        }
        else
        {
            json_t *prod = json_object_get(det, "prod");
            json_array_append(all_products, prod);
        }

        json_decref(root);
        free(nfe_files[i]);
    }

    char *all_products_json = json_dumps(all_products, JSON_INDENT(4));
    json_decref(all_products);

    GtkWidget *button = gtk_button_new_with_label("Abrir todos produtos");
    g_signal_connect(button, "clicked", G_CALLBACK(on_all_products_button_clicked), g_strdup(all_products_json));
    gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);
}

int compare_products(const void *a, const void *b)
{
    json_t *prod_a = *(json_t **)a;
    json_t *prod_b = *(json_t **)b;

    json_t *xProd_a = json_object_get(prod_a, "xProd");
    json_t *xProd_b = json_object_get(prod_b, "xProd");

    const char *name_a = json_string_value(json_object_get(xProd_a, "#text"));
    const char *name_b = json_string_value(json_object_get(xProd_b, "#text"));

    return g_strcmp0(name_a, name_b);
}

void display_sorted_products(const char *sorted_products_json)
{
    json_error_t error;
    json_t *root = json_loads(sorted_products_json, 0, &error);
    if (!root)
    {
        fprintf(stderr, "Error parsing JSON: %s\n", error.text);
        return;
    }

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Sorted Products");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 600);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

    GtkWidget *scrollable_view = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollable_view), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(window), scrollable_view);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(scrollable_view), vbox);

    GtkWidget *text_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(text_view), FALSE);
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    gtk_text_buffer_set_text(buffer, sorted_products_json, -1);
    gtk_box_pack_start(GTK_BOX(vbox), text_view, TRUE, TRUE, 0);

    gtk_widget_show_all(window);
    json_decref(root);
}

void on_sorted_products_button_clicked(GtkButton *button, gpointer user_data)
{
    const char *sorted_products_json = (const char *)user_data;
    display_sorted_products(sorted_products_json);
}

void set_tab4_content(GtkWidget *tab_view)
{
    GtkWidget *scrollable_view = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollable_view), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start(GTK_BOX(tab_view), scrollable_view, TRUE, TRUE, 0);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(scrollable_view), vbox);

    char *nfe_files[] = {
        xml_to_json(read_file("notas/NFE1.xml")),
        xml_to_json(read_file("notas/NFE2.xml")),
        xml_to_json(read_file("notas/NFE3.xml")),
        xml_to_json(read_file("notas/NFE4.xml")),
        xml_to_json(read_file("notas/NFE5.xml")),
        xml_to_json(read_file("notas/NFE6.xml"))};

    for (int i = 0; i < 6; i++)
    {
        if (!nfe_files[i])
        {
            fprintf(stderr, "Error converting XML to JSON for NFE%d\n", i + 1);
            continue;
        }

        json_error_t error;
        json_t *root = json_loads(nfe_files[i], 0, &error);
        if (!root)
        {
            fprintf(stderr, "Error parsing JSON for NFE%d: %s\n", i + 1, error.text);
            free(nfe_files[i]);
            continue;
        }

        json_t *nfeProc = json_object_get(root, "nfeProc");
        json_t *NFe = json_object_get(nfeProc, "NFe");
        json_t *infNFe = json_object_get(NFe, "infNFe");
        json_t *det = json_object_get(infNFe, "det");

        json_t *product_data = json_array();
        if (json_is_array(det))
        {
            size_t index;
            json_t *item;
            json_array_foreach(det, index, item)
            {
                json_t *prod = json_object_get(item, "prod");
                json_array_append(product_data, prod);
            }
        }
        else
        {
            json_t *prod = json_object_get(det, "prod");
            json_array_append(product_data, prod);
        }

        size_t product_count = json_array_size(product_data);
        json_t **products = malloc(product_count * sizeof(json_t *));
        for (size_t j = 0; j < product_count; j++)
        {
            products[j] = json_array_get(product_data, j);
        }

        qsort(products, product_count, sizeof(json_t *), compare_products);

        json_t *sorted_product_data = json_array();
        for (size_t j = 0; j < product_count; j++)
        {
            json_array_append(sorted_product_data, products[j]);
        }

        free(products);

        char *sorted_products_json = json_dumps(sorted_product_data, JSON_INDENT(4));
        json_decref(sorted_product_data);

        GtkWidget *button = gtk_button_new_with_label(g_strdup_printf("Abrir produtos ordenados por NFE %d", i + 1));
        g_signal_connect(button, "clicked", G_CALLBACK(on_sorted_products_button_clicked), g_strdup(sorted_products_json));
        gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);

        json_decref(root);
        free(nfe_files[i]);
    }
}

int compare_products_by_price(const void *a, const void *b)
{
    json_t *prod_a = *(json_t **)a;
    json_t *prod_b = *(json_t **)b;

    json_t *vProd_a = json_object_get(prod_a, "vProd");
    json_t *vProd_b = json_object_get(prod_b, "vProd");

    double price_a = atof(json_string_value(json_object_get(vProd_a, "#text")));
    double price_b = atof(json_string_value(json_object_get(vProd_b, "#text")));

    return (price_a > price_b) - (price_a < price_b);
}

void display_final_xml(const char *final_xml_json)
{
    json_error_t error;
    json_t *root = json_loads(final_xml_json, 0, &error);
    if (!root)
    {
        fprintf(stderr, "Error parsing JSON: %s\n", error.text);
        return;
    }

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Final XML");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 600);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE); // Disable resizing

    GtkWidget *scrollable_view = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollable_view), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(window), scrollable_view);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(scrollable_view), vbox);

    GtkWidget *text_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(text_view), FALSE);
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    gtk_text_buffer_set_text(buffer, final_xml_json, -1);
    gtk_box_pack_start(GTK_BOX(vbox), text_view, TRUE, TRUE, 0);

    gtk_widget_show_all(window);
    json_decref(root);
}

void on_final_xml_button_clicked(GtkButton *button, gpointer user_data)
{
    const char *final_xml_json = (const char *)user_data;
    display_final_xml(final_xml_json);
}

void set_tab5_content(GtkWidget *tab_view)
{
    GtkWidget *scrollable_view = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollable_view), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start(GTK_BOX(tab_view), scrollable_view, TRUE, TRUE, 0);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(scrollable_view), vbox);

    char *nfe_files[] = {
        xml_to_json(read_file("notas/NFE1.xml")),
        xml_to_json(read_file("notas/NFE2.xml")),
        xml_to_json(read_file("notas/NFE3.xml")),
        xml_to_json(read_file("notas/NFE4.xml")),
        xml_to_json(read_file("notas/NFE5.xml")),
        xml_to_json(read_file("notas/NFE6.xml"))};

    json_t *all_products = json_array();

    for (int i = 0; i < 6; i++)
    {
        if (!nfe_files[i])
        {
            fprintf(stderr, "Error converting XML to JSON for NFE%d\n", i + 1);
            continue;
        }

        json_error_t error;
        json_t *root = json_loads(nfe_files[i], 0, &error);
        if (!root)
        {
            fprintf(stderr, "Error parsing JSON for NFE%d: %s\n", i + 1, error.text);
            free(nfe_files[i]);
            continue;
        }

        json_t *nfeProc = json_object_get(root, "nfeProc");
        json_t *NFe = json_object_get(nfeProc, "NFe");
        json_t *infNFe = json_object_get(NFe, "infNFe");
        json_t *det = json_object_get(infNFe, "det");

        if (json_is_array(det))
        {
            size_t index;
            json_t *item;
            json_array_foreach(det, index, item)
            {
                json_t *prod = json_object_get(item, "prod");
                json_array_append(all_products, prod);
            }
        }
        else
        {
            json_t *prod = json_object_get(det, "prod");
            json_array_append(all_products, prod);
        }

        json_decref(root);
        free(nfe_files[i]);
    }

    size_t product_count = json_array_size(all_products);
    json_t **products = malloc(product_count * sizeof(json_t *));
    for (size_t j = 0; j < product_count; j++)
    {
        products[j] = json_array_get(all_products, j);
    }

    qsort(products, product_count, sizeof(json_t *), compare_products_by_price);

    json_t *sorted_product_data = json_array();
    for (size_t j = 0; j < product_count; j++)
    {
        json_array_append(sorted_product_data, products[j]);
    }

    free(products);

    char *sorted_products_json = json_dumps(sorted_product_data, JSON_INDENT(4));
    json_decref(sorted_product_data);

    GtkWidget *button = gtk_button_new_with_label("Abrir produtos ordenados por preço");
    g_signal_connect(button, "clicked", G_CALLBACK(on_sorted_products_button_clicked), g_strdup(sorted_products_json));
    gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);
}

void add_tab_views(GtkNotebook *notebook)
{
    GtkWidget *tab1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_notebook_append_page(notebook, tab1, gtk_label_new("XML para JSON"));
    set_tab1_content(tab1);

    GtkWidget *tab2 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_notebook_append_page(notebook, tab2, gtk_label_new("Produtos"));
    set_tab2_content(tab2);

    GtkWidget *tab3 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_notebook_append_page(notebook, tab3, gtk_label_new("Todos produtos"));
    set_tab3_content(tab3);

    GtkWidget *tab4 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_notebook_append_page(notebook, tab4, gtk_label_new("Produtos ordenados alfabeticamente"));
    set_tab4_content(tab4);

    GtkWidget *tab5 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_notebook_append_page(notebook, tab5, gtk_label_new("Todos produtos ordenados por preço"));
    set_tab5_content(tab5);
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
