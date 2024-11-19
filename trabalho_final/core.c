#include <libxml/parser.h>
#include <libxml/tree.h>
#include <jansson.h>

// Recursive function to convert XML node to JSON
static json_t *xml_to_json(xmlNode *node)
{
    json_t *json_obj = json_object();

    for (xmlNode *cur_node = node; cur_node; cur_node = cur_node->next)
    {
        if (cur_node->type == XML_ELEMENT_NODE)
        {
            json_t *child_obj;

            // Handle element attributes
            if (cur_node->properties)
            {
                json_t *attr_obj = json_object();
                for (xmlAttr *attr = cur_node->properties; attr; attr = attr->next)
                {
                    json_object_set_new(attr_obj, (const char *)attr->name,
                                        json_string((const char *)xmlNodeGetContent(attr->children)));
                }
                json_object_set_new(json_obj, "_attributes", attr_obj);
            }

            // Recurse for child elements
            if (cur_node->children)
            {
                child_obj = xml_to_json(cur_node->children);
            }
            else
            {
                child_obj = json_string((const char *)xmlNodeGetContent(cur_node));
            }

            // Append the child JSON object
            json_object_set_new(json_obj, (const char *)cur_node->name, child_obj);
        }
    }

    return json_obj;
}

int convert_xml_to_json(const char *xml_file, const char *json_file)
{
    xmlDocPtr doc = xmlParseFile(xml_file);
    if (doc == NULL)
    {
        fprintf(stderr, "Failed to parse XML file.\n");
        return 1;
    }

    xmlNode *root = xmlDocGetRootElement(doc);
    if (root == NULL)
    {
        fprintf(stderr, "Empty XML document.\n");
        xmlFreeDoc(doc);
        return 1;
    }

    // Convert XML to JSON
    json_t *json_root = xml_to_json(root);

    // Write JSON to file
    if (json_dump_file(json_root, json_file, JSON_INDENT(4)) != 0)
    {
        fprintf(stderr, "Failed to write JSON file.\n");
        json_decref(json_root);
        xmlFreeDoc(doc);
        return 1;
    }

    json_decref(json_root);
    xmlFreeDoc(doc);
    return 0;
}

int main()
{
    const char *xml_file = "example.xml";
    const char *json_file = "output.json";

    if (convert_xml_to_json(xml_file, json_file) == 0)
    {
        printf("XML successfully converted to JSON.\n");
    }
    else
    {
        printf("Failed to convert XML to JSON.\n");
    }

    return 0;
}
