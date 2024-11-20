#include <stdio.h>
#include "jsonschema-c/instance_validator.h"

void validate_json_against_schema(const char *json_file, const char *schema_file)
{
    struct json_object *schema = json_object_from_file(schema_file);
    struct json_object *instance = json_object_from_file(json_file);

    if (!schema)
    {
        fprintf(stderr, "Error: Could not load schema from file '%s'.\n", schema_file);
        return;
    }

    if (!instance)
    {
        fprintf(stderr, "Error: Could not load JSON from file '%s'.\n", json_file);
        json_object_put(schema); // Free schema object
        return;
    }

    int result = json_validate_instance(instance, schema);
    if (result)
    {
        printf("Validation succeeded. The JSON is valid.\n");
    }
    else
    {
        printf("Validation failed. The JSON is not valid.\n");
    }

    // Cleanup
    json_object_put(schema);
    json_object_put(instance);
}

int main()
{

    const char *schema_file = "schema.json";
    const char *json_file = "input.json";

    validate_json_against_schema(json_file, schema_file);

    return 0;
}