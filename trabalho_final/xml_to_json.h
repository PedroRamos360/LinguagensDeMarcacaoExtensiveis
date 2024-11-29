#ifndef XML_TO_JSON_H
#define XML_TO_JSON_H

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <jansson.h>

void add_attributes_to_json(xmlNode *node, json_t *json_obj);

void traverse_node(xmlNode *node, json_t *json_obj);

char *xml_to_json(const char *xml_str);

#endif // XML_TO_JSON_H
