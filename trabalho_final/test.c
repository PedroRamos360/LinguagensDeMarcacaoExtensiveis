#include <jansson.h>
#include <stdio.h>

int main()
{
    // Create a JSON object
    json_t *root = json_object();
    json_object_set_new(root, "name", json_string("John Doe"));
    json_object_set_new(root, "age", json_integer(30));

    // Create an array
    json_t *hobbies = json_array();
    json_array_append_new(hobbies, json_string("Reading"));
    json_array_append_new(hobbies, json_string("Traveling"));
    json_object_set_new(root, "hobbies", hobbies);

    json_t *address = json_array();
    json_object_set_new(root, "address", address);

    if (json_object_get(root, "address"))
    {
        json_object_set_new(root, "address", json_string("123 Main Street"));
    }

    // Serialize and print JSON
    char *json_str = json_dumps(root, JSON_INDENT(4));
    printf("%s\n", json_str);
    free(json_str);

    // Cleanup
    json_decref(root);
    return 0;
}
