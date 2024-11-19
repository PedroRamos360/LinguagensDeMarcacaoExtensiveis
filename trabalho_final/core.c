#include <stdio.h>
#include <stdlib.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <jansson.h>

void xml_to_json(xmlNode *node, json_t *json_obj)
{
    char *last_element_name;
    for (xmlNode *current = node; current; current = current->next)
    {
        if (current->type == XML_ELEMENT_NODE)
        {
            last_element_name = (char *)current->name;
            json_t *child_obj = json_object();

            // Add attributes of the current node to child_obj
            for (xmlAttr *attr = current->properties; attr; attr = attr->next)
            {
                xmlChar *value = xmlNodeListGetString(current->doc, attr->children, 1);
                json_object_set_new(child_obj, (const char *)attr->name, json_string((const char *)value));
                xmlFree(value);
            }

            // Check if this element should be an array or an object
            json_t *existing = json_object_get(json_obj, (const char *)current->name);
            printf("Element: %s\n", current->name);
            if (existing && json_is_array(existing))
            {
                // Add new child to the existing array
                json_array_append_new(existing, child_obj);
            }
            else if (existing)
            {
                // Convert the single object to an array and add new child
                json_t *array = json_array();
                json_array_append_new(array, existing);
                json_array_append_new(array, child_obj);
                json_object_set_new(json_obj, (const char *)current->name, array);
            }
            else
            {
                // Initialize a new array for this element and add the first child
                json_t *array = json_array();
                json_array_append_new(array, child_obj);
                json_object_set_new(json_obj, (const char *)current->name, array);
            }

            // Handle nested elements
            if (current->children)
            {
                xml_to_json(current->children, child_obj);
            }
        }
        else if (current->type == XML_TEXT_NODE && current->content)
        {
            printf("Content: %s\n", current->content);
            if (json_object_get(json_obj, last_element_name))
            {
                json_object_set_new(json_obj, last_element_name, json_string(current->content));
            }
        }
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
