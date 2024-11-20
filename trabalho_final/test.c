#include <jansson.h>
#include <stdio.h>

int main()
{
    // Create a JSON object
    json_t *json_obj = json_object();

    // Add a key-value pair
    json_object_set_new(json_obj, "name", json_string("John Doe"));

    // Retrieve the value of the "name" property
    json_t *name_value = json_object_get(json_obj, "name");
    if (json_is_string(name_value))
    {
        printf("Original 'name': %s\n", json_string_value(name_value));
    }

    // Replace "name" with an array ["John", "Doe"]
    json_t *name_array = json_array();
    json_array_append_new(name_array, json_string("John"));
    json_array_append_new(name_array, json_string("Doe"));
    json_object_set(json_obj, "name", name_array);

    // Serialize the modified JSON object to a string
    char *json_str = json_dumps(json_obj, JSON_INDENT(4));
    if (json_str)
    {
        printf("Modified JSON:\n%s\n", json_str);
        free(json_str);
    }

    // Free resources
    json_decref(json_obj);

    return 0;
}
