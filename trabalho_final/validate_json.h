#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <json-c/json.h>
#include <json-c/json_tokener.h>
#include <json-c/linkhash.h>

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_RESET "\x1b[0m"

void print_error_with_line(const char *message, int offset, const char *content)
{
    int line_number = 1;
    for (int i = 0; i < offset; i++)
    {
        if (content[i] == '\n')
        {
            line_number++;
        }
    }
    printf("%sError on line %d: %s%s\n", ANSI_COLOR_RED, line_number, message, ANSI_COLOR_RESET);
}

void print_success(const char *message)
{
    printf("%s%s%s\n", ANSI_COLOR_GREEN, message, ANSI_COLOR_RESET);
}

char *read_file(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("Error opening file");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *content = (char *)malloc(length + 1);
    if (!content)
    {
        perror("Error allocating memory");
        fclose(file);
        return NULL;
    }

    fread(content, 1, length, file);
    content[length] = '\0';

    fclose(file);
    return content;
}

int validate_json_recursive(struct json_object *instance, struct json_object *schema, const char *content)
{
    struct lh_table *schema_table = json_object_get_object(schema);
    struct lh_entry *entry;

    lh_foreach(schema_table, entry)
    {
        const char *key = (const char *)entry->k;
        struct json_object *value = (struct json_object *)entry->v;

        if (strcmp(key, "type") == 0)
        {
            const char *expected_type = json_object_get_string(value);

            enum json_type instance_type = json_object_get_type(instance);
            if ((strcmp(expected_type, "object") == 0 && instance_type != json_type_object) ||
                (strcmp(expected_type, "array") == 0 && instance_type != json_type_array) ||
                (strcmp(expected_type, "string") == 0 && instance_type != json_type_string) ||
                (strcmp(expected_type, "number") == 0 && instance_type != json_type_double && instance_type != json_type_int))
            {
                char error_message[256];
                snprintf(error_message, sizeof(error_message),
                         "Type mismatch: Expected %s, but got %s.",
                         expected_type,
                         json_type_to_name(instance_type));
                print_error_with_line(error_message, 0, content);
                return 0;
            }
        }
        else if (strcmp(key, "properties") == 0)
        {
            if (json_object_get_type(instance) != json_type_object)
            {
                print_error_with_line("Expected an object for validating properties.", 0, content);
                return 0;
            }
            struct json_object *properties = value;
            struct lh_table *properties_table = json_object_get_object(properties);
            struct lh_entry *prop_entry;

            lh_foreach(properties_table, prop_entry)
            {
                const char *prop_key = (const char *)prop_entry->k;
                struct json_object *prop_schema = (struct json_object *)prop_entry->v;

                struct json_object *prop_value;
                if (json_object_object_get_ex(instance, prop_key, &prop_value))
                {
                    if (!validate_json_recursive(prop_value, prop_schema, content))
                    {
                        return 0;
                    }
                }
            }
        }
        // Código problemático: consertar
        // else if (strcmp(key, "items") == 0)
        // {
        //     if (json_object_get_type(instance) != json_type_array)
        //     {
        //         print_error_with_line("Expected an array for validating items.", 0, content);
        //         return 0;
        //     }
        //     int array_len = json_object_array_length(instance);
        //     for (int i = 0; i < array_len; i++)
        //     {
        //         struct json_object *item = json_object_array_get_idx(instance, i);
        //         if (!validate_json_recursive(item, value, content))
        //         {
        //             return 0;
        //         }
        //     }
        // }
        else if (strcmp(key, "required") == 0)
        {
            struct json_object *required = value;
            int len = json_object_array_length(required);
            for (int i = 0; i < len; i++)
            {
                const char *required_key = json_object_get_string(json_object_array_get_idx(required, i));
                struct json_object *required_value;
                if (!json_object_object_get_ex(instance, required_key, &required_value))
                {
                    char error_message[256];
                    snprintf(error_message, sizeof(error_message),
                             "Missing required property: %s.", required_key);
                    print_error_with_line(error_message, 0, content);
                    return 0;
                }
            }
        }
    }

    return 1;
}

int validate_json(const char *instance_str, const char *schema_str)
{
    const char *content = strdup(instance_str);
    struct json_tokener *tokener = json_tokener_new();
    if (!tokener)
    {
        print_error_with_line("Failed to create JSON tokener.", -1, content);
        return 0;
    }

    struct json_object *instance = json_tokener_parse_ex(tokener, instance_str, strlen(instance_str));
    if (json_tokener_get_error(tokener) != json_tokener_success)
    {
        print_error_with_line(json_tokener_error_desc(json_tokener_get_error(tokener)), tokener->char_offset, content);
        json_tokener_free(tokener);
        return 0;
    }

    struct json_object *schema = json_tokener_parse_ex(tokener, schema_str, strlen(schema_str));
    if (json_tokener_get_error(tokener) != json_tokener_success)
    {
        print_error_with_line(json_tokener_error_desc(json_tokener_get_error(tokener)), tokener->char_offset, content);
        json_tokener_free(tokener);
        return 0;
    }

    int result = validate_json_recursive(instance, schema, content);

    json_tokener_free(tokener);

    if (result)
    {
        print_success("JSON instance is valid against the schema.");
    }

    return result;
}
