#include "dashboard.h"
#include "xml_to_json.h"
#include "validate_json.h"
#include <locale.h>

static void on_back_button_clicked(GtkButton *button, gpointer user_data)
{
    GtkStack *stack = GTK_STACK(user_data);
    gtk_stack_set_visible_child_name(stack, "main-view");
}

void add_general_view(GtkNotebook *notebook)
{
    setlocale(LC_NUMERIC, "en_US.UTF-8");
    GtkWidget *general_view = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_notebook_append_page(notebook, general_view, gtk_label_new("General"));

    char *nfe_files[] = {
        xml_to_json(read_file("notas/NFE1.xml")),
        xml_to_json(read_file("notas/NFE2.xml")),
        xml_to_json(read_file("notas/NFE3.xml")),
        xml_to_json(read_file("notas/NFE4.xml")),
        xml_to_json(read_file("notas/NFE5.xml")),
        xml_to_json(read_file("notas/NFE6.xml"))};

    int total_nfs = 0;
    int total_products = 0;
    double total_value = 0.0;
    double total_taxes = 0.0;

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
        if (nfeProc)
        {
            json_t *NFe = json_object_get(nfeProc, "NFe");
            json_t *infNFe = json_object_get(NFe, "infNFe");
            total_nfs++;
            json_t *det = json_object_get(infNFe, "det");
            if (json_is_array(det))
            {
                total_products += json_array_size(det);
            }
            else
            {
                total_products += 1;
            }
            json_t *pag = json_object_get(infNFe, "pag");
            json_t *detPag = json_object_get(pag, "detPag");
            json_t *vPag = json_object_get(detPag, "vPag");
            json_t *vpag_text = json_object_get(vPag, "#text");
            total_value += atof(json_string_value(vpag_text));
            json_t *total = json_object_get(infNFe, "total");
            json_t *ICMSTot = json_object_get(total, "ICMSTot");
            json_t *vTotTrib = json_object_get(ICMSTot, "vTotTrib");
            json_t *vicms_text = json_object_get(vTotTrib, "#text");
            printf("vicms_text: %s\n", json_string_value(vicms_text));
            printf("vicms atof: %.2f\n", atof(json_string_value(vicms_text)));
            total_taxes += atof(json_string_value(vicms_text));
        }

        json_decref(root);
        free(nfe_files[i]);
    }

    GtkWidget *label_nfs = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label_nfs), g_strdup_printf("<span font='30'>Total NFs: %d</span>", total_nfs));
    gtk_box_pack_start(GTK_BOX(general_view), label_nfs, FALSE, FALSE, 0);

    GtkWidget *label_products = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label_products), g_strdup_printf("<span font='30'>Total Products: %d</span>", total_products));
    gtk_box_pack_start(GTK_BOX(general_view), label_products, FALSE, FALSE, 0);

    GtkWidget *label_value = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label_value), g_strdup_printf("<span font='30'>Total Value: %.2f</span>", total_value));
    gtk_box_pack_start(GTK_BOX(general_view), label_value, FALSE, FALSE, 0);

    GtkWidget *label_taxes = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label_taxes), g_strdup_printf("<span font='30'>Total Taxes: %.2f</span>", total_taxes));
    gtk_box_pack_start(GTK_BOX(general_view), label_taxes, FALSE, FALSE, 0);

    gtk_widget_show_all(general_view);
}

void display_nfe_data(const char *nfe_json)
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

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    json_t *nfeProc = json_object_get(root, "nfeProc");
    json_t *NFe = json_object_get(nfeProc, "NFe");
    json_t *infNFe = json_object_get(NFe, "infNFe");
    json_t *emit = json_object_get(infNFe, "emit");
    const char *store_name = "N/A";
    if (emit)
    {
        json_t *xNome = json_object_get(emit, "xNome");
        json_t *xnome_text = json_object_get(xNome, "#text");
        if (xnome_text && json_is_string(xnome_text))
        {
            store_name = json_string_value(xnome_text);
        }
    }
    GtkWidget *label_store_title = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label_store_title), "<b>Store:</b>");
    gtk_box_pack_start(GTK_BOX(vbox), label_store_title, FALSE, FALSE, 0);

    GtkWidget *label_store = gtk_label_new(NULL);
    gtk_label_set_text(GTK_LABEL(label_store), store_name);
    gtk_box_pack_start(GTK_BOX(vbox), label_store, FALSE, FALSE, 0);

    GtkWidget *label_products_title = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label_products_title), "<b>Products:</b>");
    gtk_box_pack_start(GTK_BOX(vbox), label_products_title, FALSE, FALSE, 0);

    json_t *det = json_object_get(infNFe, "det");
    if (json_is_array(det))
    {
        size_t index;
        json_t *item;
        json_array_foreach(det, index, item)
        {
            json_t *prod = json_object_get(item, "prod");

            const char *product_name = "N/A";
            double product_value = 0.0;

            json_t *xProd = json_object_get(prod, "xProd");
            json_t *xprod_text = json_object_get(xProd, "#text");
            if (xprod_text && json_is_string(xprod_text))
            {
                product_name = json_string_value(xprod_text);
            }

            json_t *vProd = json_object_get(prod, "vProd");
            json_t *vprod_text = json_object_get(vProd, "#text");
            if (vprod_text && json_is_string(vprod_text))
            {
                product_value = atof(json_string_value(vprod_text));
            }

            GtkWidget *product_label = gtk_label_new(NULL);
            gtk_label_set_text(GTK_LABEL(product_label),
                               g_strdup_printf("- %s: %.2f", product_name, product_value));
            gtk_box_pack_start(GTK_BOX(vbox), product_label, FALSE, FALSE, 0);
        }
    }
    else
    {
        json_t *prod = json_object_get(det, "prod");
        const char *product_name = "N/A";
        double product_value = 0.0;

        json_t *xProd = json_object_get(prod, "xProd");
        json_t *xprod_text = json_object_get(xProd, "#text");
        if (xprod_text && json_is_string(xprod_text))
        {
            product_name = json_string_value(xprod_text);
        }

        json_t *vProd = json_object_get(prod, "vProd");
        json_t *vprod_text = json_object_get(vProd, "#text");
        if (vprod_text && json_is_string(vprod_text))
        {
            product_value = atof(json_string_value(vprod_text));
        }

        GtkWidget *product_label = gtk_label_new(NULL);
        gtk_label_set_text(GTK_LABEL(product_label),
                           g_strdup_printf("- %s: %.2f", product_name, product_value));
        gtk_box_pack_start(GTK_BOX(vbox), product_label, FALSE, FALSE, 0);
    }

    double total_value = 0.0;
    json_t *pag = json_object_get(infNFe, "pag");

    json_t *detPag = json_object_get(pag, "detPag");

    json_t *vPag = json_object_get(detPag, "vPag");
    json_t *vpag_text = json_object_get(vPag, "#text");
    if (vpag_text && json_is_string(vpag_text))
    {
        total_value = atof(json_string_value(vpag_text));
    }

    GtkWidget *label_total_title = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label_total_title), "<b>Total Value:</b>");
    gtk_box_pack_start(GTK_BOX(vbox), label_total_title, FALSE, FALSE, 0);

    GtkWidget *label_total = gtk_label_new(NULL);
    gtk_label_set_text(GTK_LABEL(label_total), g_strdup_printf("%.2f", total_value));
    gtk_box_pack_start(GTK_BOX(vbox), label_total, FALSE, FALSE, 0);

    json_decref(root);
    gtk_widget_show_all(window);
}

void on_nfe_button_clicked(GtkButton *button, gpointer user_data)
{
    const char *nfe_json = (const char *)user_data;
    display_nfe_data(nfe_json);
}

void add_nf_view(GtkNotebook *notebook)
{
    GtkWidget *nf_view = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_notebook_append_page(notebook, nf_view, gtk_label_new("NFs"));

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

        GtkWidget *button = gtk_button_new_with_label(g_strdup_printf("Open NFE %d", i + 1));
        g_signal_connect(button, "clicked", G_CALLBACK(on_nfe_button_clicked), nfe_files[i]);
        gtk_box_pack_start(GTK_BOX(nf_view), button, FALSE, FALSE, 0);
    }

    gtk_widget_show_all(nf_view);
}

typedef struct
{
    double icms;
    double pis;
    double cofins;
    double ipi;
    double outros;
} NFeTaxes;

void add_taxes_view(GtkNotebook *notebook)
{
    setlocale(LC_NUMERIC, "en_US.UTF-8");
    GtkWidget *scrollable_view = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollable_view), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    GtkWidget *general_view = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(scrollable_view), general_view);
    gtk_notebook_append_page(notebook, scrollable_view, gtk_label_new("Taxes"));

    char *nfe_files[] = {
        xml_to_json(read_file("notas/NFE1.xml")),
        xml_to_json(read_file("notas/NFE2.xml")),
        xml_to_json(read_file("notas/NFE3.xml")),
        xml_to_json(read_file("notas/NFE4.xml")),
        xml_to_json(read_file("notas/NFE5.xml")),
        xml_to_json(read_file("notas/NFE6.xml"))};

    double total_taxes = 0.0;
    NFeTaxes nfes[6] = {0};

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
        json_t *total = json_object_get(infNFe, "total");
        json_t *ICMSTot = json_object_get(total, "ICMSTot");

        json_t *vTotTrib = json_object_get(ICMSTot, "vTotTrib");
        json_t *vtot_text = json_object_get(vTotTrib, "#text");
        double this_taxes = atof(json_string_value(vtot_text));
        total_taxes += this_taxes;

        json_t *vICMS = json_object_get(ICMSTot, "vICMS");
        json_t *vicms_text = json_object_get(vICMS, "#text");
        nfes[i].icms = atof(json_string_value(vicms_text));
        json_t *vPIS = json_object_get(ICMSTot, "vPIS");
        json_t *vpis_text = json_object_get(vPIS, "#text");
        nfes[i].pis = atof(json_string_value(vpis_text));
        json_t *vCOFINS = json_object_get(ICMSTot, "vCOFINS");
        json_t *vcofins_text = json_object_get(vCOFINS, "#text");
        nfes[i].cofins = atof(json_string_value(vcofins_text));
        json_t *vIPI = json_object_get(ICMSTot, "vIPI");
        json_t *vipi_text = json_object_get(vIPI, "#text");
        nfes[i].ipi = atof(json_string_value(vipi_text));

        nfes[i].outros = this_taxes - nfes[i].icms - nfes[i].pis - nfes[i].cofins - nfes[i].ipi;

        json_decref(root);
        free(nfe_files[i]);
    }

    GtkWidget *label_taxes = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label_taxes), g_strdup_printf("<span font='30'>Total Taxes: %.2f</span>", total_taxes));
    gtk_box_pack_start(GTK_BOX(general_view), label_taxes, FALSE, FALSE, 0);

    for (int i = 0; i < 6; i++)
    {
        GtkWidget *label_nfe = gtk_label_new(NULL);
        gtk_label_set_markup(GTK_LABEL(label_nfe), g_strdup_printf("<span font='25'><b>NFE%d</b></span>", i + 1));
        gtk_box_pack_start(GTK_BOX(general_view), label_nfe, FALSE, FALSE, 0);

        GtkWidget *label_icms = gtk_label_new(NULL);
        gtk_label_set_markup(GTK_LABEL(label_icms), g_strdup_printf("<span font='20'>ICMS: %.2f</span>", nfes[i].icms));
        gtk_box_pack_start(GTK_BOX(general_view), label_icms, FALSE, FALSE, 0);

        GtkWidget *label_pis = gtk_label_new(NULL);
        gtk_label_set_markup(GTK_LABEL(label_pis), g_strdup_printf("<span font='20'>PIS: %.2f</span>", nfes[i].pis));
        gtk_box_pack_start(GTK_BOX(general_view), label_pis, FALSE, FALSE, 0);

        GtkWidget *label_cofins = gtk_label_new(NULL);
        gtk_label_set_markup(GTK_LABEL(label_cofins), g_strdup_printf("<span font='20'>COFINS: %.2f</span>", nfes[i].cofins));
        gtk_box_pack_start(GTK_BOX(general_view), label_cofins, FALSE, FALSE, 0);

        GtkWidget *label_ipi = gtk_label_new(NULL);
        gtk_label_set_markup(GTK_LABEL(label_ipi), g_strdup_printf("<span font='20'>IPI: %.2f</span>", nfes[i].ipi));
        gtk_box_pack_start(GTK_BOX(general_view), label_ipi, FALSE, FALSE, 0);

        GtkWidget *label_outros = gtk_label_new(NULL);
        gtk_label_set_markup(GTK_LABEL(label_outros), g_strdup_printf("<span font='20'>Outros: %.2f</span>", nfes[i].outros));
        gtk_box_pack_start(GTK_BOX(general_view), label_outros, FALSE, FALSE, 0);
    }

    gtk_widget_show_all(general_view);
}

void add_suppliers_view(GtkNotebook *notebook)
{
    GtkWidget *suppliers_view = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_notebook_append_page(notebook, suppliers_view, gtk_label_new("Suppliers"));

    char *nfe_files[] = {
        xml_to_json(read_file("notas/NFE1.xml")),
        xml_to_json(read_file("notas/NFE2.xml")),
        xml_to_json(read_file("notas/NFE3.xml")),
        xml_to_json(read_file("notas/NFE4.xml")),
        xml_to_json(read_file("notas/NFE5.xml")),
        xml_to_json(read_file("notas/NFE6.xml"))};

    GHashTable *suppliers = g_hash_table_new(g_str_hash, g_str_equal);

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
        json_t *emit = json_object_get(infNFe, "emit");
        json_t *xNome = json_object_get(emit, "xNome");
        json_t *xnome_text = json_object_get(xNome, "#text");
        const char *supplier_name = json_string_value(xnome_text);

        GList *nfe_list = g_hash_table_lookup(suppliers, supplier_name);
        nfe_list = g_list_append(nfe_list, nfe_files[i]);
        g_hash_table_insert(suppliers, g_strdup(supplier_name), nfe_list);

        json_decref(root);
    }

    GList *keys = g_hash_table_get_keys(suppliers);
    keys = g_list_sort(keys, (GCompareFunc)g_strcmp0);

    for (GList *iter = keys; iter != NULL; iter = iter->next)
    {
        const char *supplier_name = iter->data;
        GtkWidget *label_supplier = gtk_label_new(NULL);
        gtk_label_set_markup(GTK_LABEL(label_supplier), g_strdup_printf("<span font='20'><b>%s</b></span>", supplier_name));
        gtk_box_pack_start(GTK_BOX(suppliers_view), label_supplier, FALSE, FALSE, 0);

        GList *nfe_list = g_hash_table_lookup(suppliers, supplier_name);
        for (GList *nfe_iter = nfe_list; nfe_iter != NULL; nfe_iter = nfe_iter->next)
        {
            char *nfe_json = nfe_iter->data;
            GtkWidget *button = gtk_button_new_with_label("Open NFE");
            g_signal_connect(button, "clicked", G_CALLBACK(on_nfe_button_clicked), nfe_json);
            gtk_box_pack_start(GTK_BOX(suppliers_view), button, FALSE, FALSE, 0);
        }
    }

    g_list_free(keys);
    g_hash_table_destroy(suppliers);

    gtk_widget_show_all(suppliers_view);
}

void add_transporters_view(GtkNotebook *notebook)
{
    GtkWidget *transporters_view = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_notebook_append_page(notebook, transporters_view, gtk_label_new("Transporters"));

    char *nfe_files[] = {
        xml_to_json(read_file("notas/NFE1.xml")),
        xml_to_json(read_file("notas/NFE2.xml")),
        xml_to_json(read_file("notas/NFE3.xml")),
        xml_to_json(read_file("notas/NFE4.xml")),
        xml_to_json(read_file("notas/NFE5.xml")),
        xml_to_json(read_file("notas/NFE6.xml"))};

    GHashTable *transporters = g_hash_table_new(g_str_hash, g_str_equal);

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
        json_t *transp = json_object_get(infNFe, "transp");
        json_t *transporta = json_object_get(transp, "transporta");
        if (transporta)
        {
            json_t *xNome = json_object_get(transporta, "xNome");
            if (xNome)
            {
                json_t *xnome_text = json_object_get(xNome, "#text");
                if (xnome_text && json_is_string(xnome_text))
                {
                    const char *transporter_name = json_string_value(xnome_text);

                    GList *nfe_list = g_hash_table_lookup(transporters, transporter_name);
                    nfe_list = g_list_append(nfe_list, nfe_files[i]);
                    g_hash_table_insert(transporters, g_strdup(transporter_name), nfe_list);
                }
            }
        }

        json_decref(root);
    }

    GList *keys = g_hash_table_get_keys(transporters);
    keys = g_list_sort(keys, (GCompareFunc)g_strcmp0);

    for (GList *iter = keys; iter != NULL; iter = iter->next)
    {
        const char *transporter_name = iter->data;
        GtkWidget *label_transporter = gtk_label_new(NULL);
        gtk_label_set_markup(GTK_LABEL(label_transporter), g_strdup_printf("<span font='20'><b>%s</b></span>", transporter_name));
        gtk_box_pack_start(GTK_BOX(transporters_view), label_transporter, FALSE, FALSE, 0);

        GList *nfe_list = g_hash_table_lookup(transporters, transporter_name);
        for (GList *nfe_iter = nfe_list; nfe_iter != NULL; nfe_iter = nfe_iter->next)
        {
            char *nfe_json = nfe_iter->data;
            GtkWidget *button = gtk_button_new_with_label("Open NFE");
            g_signal_connect(button, "clicked", G_CALLBACK(on_nfe_button_clicked), nfe_json);
            gtk_box_pack_start(GTK_BOX(transporters_view), button, FALSE, FALSE, 0);
        }
    }

    g_list_free(keys);
    g_hash_table_destroy(transporters);

    gtk_widget_show_all(transporters_view);
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
