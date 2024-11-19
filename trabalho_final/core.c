#include <stdio.h>
#include <stdlib.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <jansson.h>

void xml_to_json(xmlNode *node, json_t *json_obj)
{
    for (xmlNode *current = node; current; current = current->next)
    {
        printf("Node: %s\n", current->name);
        json_object_set_new(json_obj, current->name, json_string(current->children->content));
    }
}

int main(int argc, char **argv)
{
    xmlDoc *doc = xmlReadFile("input.xml", NULL, 0);
    if (doc == NULL)
    {
        fprintf(stderr, "Failed to parse XML\n");
        return 1;
    }
    xmlNode *root_element = xmlDocGetRootElement(doc);
    printf("Root Node: %s\n", root_element->name);
    json_t *json_obj = json_object();
    xml_to_json(root_element, json_obj);
    printf("JSON:\n%s\n", json_dumps(json_obj, JSON_INDENT(4)));
}
