#include <libxml/parser.h>
#include <libxml/tree.h>
#include <jansson.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Utility function for adding attributes
void add_attributes_to_json(xmlNode *node, json_t *json_obj)
{
    xmlAttr *attr;
    for (attr = node->properties; attr; attr = attr->next)
    {
        char attr_name[256];
        snprintf(attr_name, sizeof(attr_name), "@%s", attr->name);
        json_object_set_new(json_obj, attr_name, json_string((const char *)attr->children->content));
    }
}

// Recursively traverse the XML node
void traverse_node(xmlNode *node, json_t *json_obj)
{
    for (xmlNode *cur_node = node; cur_node; cur_node = cur_node->next)
    {
        if (cur_node->type == XML_ELEMENT_NODE)
        {
            json_t *child_obj = json_object();

            // Add attributes to the child object
            add_attributes_to_json(cur_node, child_obj);

            // Add text content if it exists
            if (cur_node->children && cur_node->children->type == XML_TEXT_NODE)
            {
                json_object_set_new(child_obj, "#text", json_string((const char *)cur_node->children->content));
            }

            const char *name = (const char *)cur_node->name;

            // Check if this key already exists in the parent object
            json_t *existing_value = json_object_get(json_obj, name);

            if (existing_value)
            {
                if (json_is_array(existing_value))
                {
                    // If it's already an array, append the new value
                    json_array_append_new(existing_value, child_obj);
                }
                else
                {
                    // If it's not an array, convert to array
                    json_t *array = json_array();
                    json_array_append_new(array, existing_value);
                    json_array_append_new(array, child_obj);
                    json_object_set_new(json_obj, name, array);
                }
            }
            else
            {
                // If it doesn't exist, add the new key-value pair
                json_object_set_new(json_obj, name, child_obj);
            }

            // Recursively process child nodes
            traverse_node(cur_node->children, child_obj);
        }
    }
}

char *xml2json(const char *xml_str)
{
    xmlDoc *doc = xmlReadMemory(xml_str, strlen(xml_str), "noname.xml", NULL, 0);
    if (doc == NULL)
    {
        fprintf(stderr, "Failed to parse XML\n");
        return NULL;
    }

    xmlNode *root_element = xmlDocGetRootElement(doc);
    json_t *json_obj = json_object();

    traverse_node(root_element, json_obj);

    char *result = json_dumps(json_obj, JSON_INDENT(4));

    json_decref(json_obj);
    xmlFreeDoc(doc);
    xmlCleanupParser();

    return result;
}

int main()
{
    const char *xml_str = "<root><name>Example</name><address>123 Main St</address></root>";

    xmlDoc *doc = xmlReadMemory(xml_str, strlen(xml_str), "noname.xml", NULL, 0);
    if (doc == NULL)
    {
        fprintf(stderr, "Failed to parse XML\n");
        return 1;
    }

    xmlNode *root_element = xmlDocGetRootElement(doc);
    json_t *json_obj = json_object();

    traverse_node(root_element, json_obj);

    // Serialize JSON to string and print
    char *result = json_dumps(json_obj, JSON_INDENT(4));
    if (result)
    {
        printf("%s\n", result);
        free(result);
    }

    // Cleanup
    json_decref(json_obj);
    xmlFreeDoc(doc);
    xmlCleanupParser();

    return 0;
}
