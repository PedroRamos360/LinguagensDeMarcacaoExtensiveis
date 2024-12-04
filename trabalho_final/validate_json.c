#include <jansson.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "validate_json.h"

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_RESET "\x1b[0m"

void print_error(const char *message, const char *key, json_t *value)
{
    char *json_str = json_dumps(value, JSON_INDENT(2));
    printf("%sError: %s%s\n", ANSI_COLOR_RED, message, ANSI_COLOR_RESET);
    printf("Key: %s\n", key);
    printf("Value: %s\n", json_str);
    free(json_str); // Make sure to free the memory allocated by json_dump
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

int validate_json_recursive(json_t *instance, json_t *schema)
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
                    print_error(error_message, key, value);
                    return 0;
                }
            }
        }
        else if (strcmp(key, "properties") == 0)
        {
            if (!json_is_object(instance))
            {
                print_error("Expected an object for validating properties.", key, value);
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
                    if (!validate_json_recursive(prop_value, prop_schema))
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
                print_error("Invalid schema: 'required' must be an array.", key, value);
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
                    print_error(error_message, key, value);
                    return 0;
                }
            }
        }
        else if (strcmp(key, "items") == 0)
        {
            if (!json_is_array(instance))
            {
                print_error("Expected an array for validating items.", key, value);
                return 0;
            }

            size_t index;
            json_t *item;
            json_array_foreach(instance, index, item)
            {
                if (!validate_json_recursive(item, value))
                {
                    char error_message[256];
                    snprintf(error_message, sizeof(error_message),
                             "Validation failed for array item at index %zu.", index);
                    print_error(error_message, key, value);
                    return 0;
                }
            }
        }
        // Handle other schema properties (like 'additionalProperties', etc.)
        else if (strcmp(key, "additionalProperties") == 0)
        {
            // Handle logic if needed (depends on your schema)
        }
    }

    return 1;
}

int validate_json(const char *instance_str, const char *schema_str)
{
    return 1;
    json_error_t error;
    json_t *instance = json_loads(instance_str, 0, &error);
    if (!instance)
    {
        print_error(error.text, "", NULL);
        return 0;
    }

    json_t *schema = json_loads(schema_str, 0, &error);
    if (!schema)
    {
        print_error(error.text, "", NULL);
        json_decref(instance);
        return 0;
    }

    int result = validate_json_recursive(instance, schema);

    json_decref(instance);
    json_decref(schema);

    if (result)
    {
        print_success("JSON instance is valid against the schema.");
    }

    return result;
}
