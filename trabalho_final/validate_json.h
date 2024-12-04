#ifndef JSON_VALIDATOR_H
#define JSON_VALIDATOR_H

#include <jansson.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_RESET "\x1b[0m"

void print_success(const char *message);
void print_failure(const char *message);
char *read_file(const char *filename);
int validate_json(const char *instance_str, const char *schema_str);

#endif // JSON_VALIDATOR_H
