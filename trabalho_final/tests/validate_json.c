#include "../validate_json.h"

int main()
{
    print_success("\n========== SHOULD SUCCEED TESTS ==========\n");
    for (int i = 1; i <= 4; i++)
    {
        printf("Validating JSON instance %d...\n", i);
        char schema_filename[50];
        char instance_filename[50];
        snprintf(schema_filename, sizeof(schema_filename), "tests/ss%d.json", i);
        snprintf(instance_filename, sizeof(instance_filename), "tests/si%d.json", i);

        char *schema = read_file(schema_filename);
        char *instance = read_file(instance_filename);

        if (!instance || !schema)
        {
            return 1;
        }

        int result = validate_json(instance, schema);
        if (!result)
        {
            printf("Schema: %s\n", schema);
            printf("Instance: %s\n", instance);
        }
    }

    char *main_instance = read_file("input.json");
    char *main_schema = read_file("schema.json");
    int result = validate_json(main_instance, main_schema);
    if (!result)
    {
        printf("Schema: %s\n", main_schema);
        printf("Instance: %s\n", main_instance);
    }

    print_failure("\n========== SHOULD FAIL TESTS ==========\n");
    for (int i = 1; i <= 4; i++)
    {
        printf("Validating JSON instance %d...\n", i);
        char schema_filename[50];
        char instance_filename[50];
        snprintf(schema_filename, sizeof(schema_filename), "tests/fs%d.json", i);
        snprintf(instance_filename, sizeof(instance_filename), "tests/fi%d.json", i);

        char *schema = read_file(schema_filename);
        char *instance = read_file(instance_filename);

        if (!instance || !schema)
        {
            return 1;
        }

        int result = validate_json(instance, schema);
        if (result)
        {
            printf("Schema: %s\n", schema);
            printf("Instance: %s\n", instance);
        }
    }

    return 0;
}
