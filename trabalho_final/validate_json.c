#include <jansson.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "validate_json.h"

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

void print_failure(const char *message)
{
    printf("%s%s%s\n", ANSI_COLOR_RED, message, ANSI_COLOR_RESET);
}

char *read_file(const char *filename)
{
    FILE *file = fopen(filename, "rb");
    if (!file)
    {
        perror("Error opening file");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    if (length < 0)
    {
        perror("Error determining file size");
        fclose(file);
        return NULL;
    }
    fseek(file, 0, SEEK_SET);

    char *content = (char *)malloc(length + 1);
    if (!content)
    {
        perror("Error allocating memory");
        fclose(file);
        return NULL;
    }

    size_t read_size = fread(content, 1, length, file);
    if (read_size != (size_t)length)
    {
        fprintf(stderr, "Error: Could not read the entire file. Expected %ld bytes, got %zu bytes.\n", length, read_size);
        free(content);
        fclose(file);
        return NULL;
    }

    content[length] = '\0';
    fclose(file);
    return content;
}

int validate_json_recursive(json_t *instance, json_t *schema, const char *content)
{
    const char *key;
    json_t *value;

    json_object_foreach(schema, key, value)
    {
        if (strcmp(key, "type") == 0)
        {
            if (json_is_string(value))
            {
                const char *expected_type = json_string_value(value);
                if ((strcmp(expected_type, "object") == 0 && !json_is_object(instance)) ||
                    (strcmp(expected_type, "array") == 0 && !json_is_array(instance)) ||
                    (strcmp(expected_type, "string") == 0 && !json_is_string(instance)) ||
                    (strcmp(expected_type, "integer") == 0 && !json_is_integer(instance)) ||
                    (strcmp(expected_type, "number") == 0 && !json_is_number(instance)))
                {
                    char error_message[256];
                    snprintf(error_message, sizeof(error_message),
                             "Type mismatch: Expected %s.", expected_type);
                    print_error_with_line(error_message, 0, content);
                    return 0;
                }
            }
            else if (json_is_array(value))
            {
                size_t index;
                json_t *allowed_type;
                int valid = 0;

                json_array_foreach(value, index, allowed_type)
                {
                    const char *type_str = json_string_value(allowed_type);
                    if ((strcmp(type_str, "object") == 0 && json_is_object(instance)) ||
                        (strcmp(type_str, "array") == 0 && json_is_array(instance)) ||
                        (strcmp(type_str, "string") == 0 && json_is_string(instance)) ||
                        (strcmp(type_str, "integer") == 0 && json_is_integer(instance)) ||
                        (strcmp(type_str, "number") == 0 && json_is_number(instance)))
                    {
                        valid = 1;
                        break;
                    }
                }

                if (!valid)
                {
                    char error_message[256];
                    snprintf(error_message, sizeof(error_message),
                             "Type mismatch: Expected one of the union types.");
                    print_error_with_line(error_message, 0, content);
                    return 0;
                }
            }
        }
        else if (strcmp(key, "properties") == 0)
        {
            if (!json_is_object(instance))
            {
                print_error_with_line("Expected an object for validating properties.", 0, content);
                return 0;
            }

            json_t *properties = value;
            const char *prop_key;
            json_t *prop_schema;

            json_object_foreach(properties, prop_key, prop_schema)
            {
                json_t *prop_value = json_object_get(instance, prop_key);
                if (prop_value)
                {
                    if (!validate_json_recursive(prop_value, prop_schema, content))
                    {
                        return 0; // Propagate failure
                    }
                }
            }
        }
        else if (strcmp(key, "required") == 0)
        {
            if (!json_is_array(value))
            {
                print_error_with_line("Invalid schema: 'required' must be an array.", 0, content);
                return 0;
            }

            size_t index;
            json_t *required_key;
            json_array_foreach(value, index, required_key)
            {
                const char *required_key_str = json_string_value(required_key);
                if (!json_object_get(instance, required_key_str))
                {
                    char error_message[256];
                    snprintf(error_message, sizeof(error_message),
                             "Missing required property: %s.", required_key_str);
                    print_error_with_line(error_message, 0, content);
                    return 0;
                }
            }
        }
        else if (strcmp(key, "items") == 0)
        {
            if (!json_is_array(instance))
            {
                print_error_with_line("Expected an array for validating items.", 0, content);
                return 0;
            }

            size_t index;
            json_t *item;
            json_array_foreach(instance, index, item)
            {
                if (!validate_json_recursive(item, value, content))
                {
                    char error_message[256];
                    snprintf(error_message, sizeof(error_message),
                             "Validation failed for array item at index %zu.", index);
                    print_error_with_line(error_message, 0, content);
                    return 0;
                }
            }
        }
        else if (strcmp(key, "additionalProperties") == 0)
        {
            if (json_is_boolean(value))
            {
                // If additionalProperties is false, ensure no extra properties exist
                if (!json_boolean_value(value))
                {
                    const char *instance_key;
                    json_t *instance_value;

                    json_object_foreach(instance, instance_key, instance_value)
                    {
                        // Check if the property is defined in "properties"
                        if (!json_object_get(schema, "properties") ||
                            !json_object_get(json_object_get(schema, "properties"), instance_key))
                        {
                            char error_message[256];
                            snprintf(error_message, sizeof(error_message),
                                     "Additional property '%s' is not allowed.", instance_key);
                            print_error_with_line(error_message, 0, content);
                            return 0;
                        }
                    }
                }
            }
            else if (json_is_object(value))
            {
                // If additionalProperties is a schema, validate each extra property
                const char *instance_key;
                json_t *instance_value;

                json_object_foreach(instance, instance_key, instance_value)
                {
                    // Skip properties defined in "properties"
                    if (json_object_get(schema, "properties") &&
                        json_object_get(json_object_get(schema, "properties"), instance_key))
                    {
                        continue;
                    }

                    // Validate additional property against the additionalProperties schema
                    if (!validate_json_recursive(instance_value, value, content))
                    {
                        char error_message[256];
                        snprintf(error_message, sizeof(error_message),
                                 "Validation failed for additional property '%s'.", instance_key);
                        print_error_with_line(error_message, 0, content);
                        return 0;
                    }
                }
            }
        }
    }

    return 1;
}

int validate_json(const char *instance_str, const char *schema_str)
{
    char *content = strdup(instance_str);
    if (!content)
    {
        fprintf(stderr, "Failed to duplicate input content.\n");
        return 0;
    }

    json_error_t error;
    json_t *instance = json_loads(instance_str, 0, &error);
    if (!instance)
    {
        print_error_with_line(error.text, error.position, content);
        free(content);
        return 0;
    }

    json_t *schema = json_loads(schema_str, 0, &error);
    if (!schema)
    {
        print_error_with_line(error.text, error.position, content);
        json_decref(instance);
        free(content);
        return 0;
    }

    int result = validate_json_recursive(instance, schema, content);

    json_decref(instance);
    json_decref(schema);
    free(content);

    if (result)
    {
        print_success("JSON instance is valid against the schema.");
    }

    return result;
}
