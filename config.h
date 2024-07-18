#ifndef ASSERT_FILE_CONFIG
#define ASSERT_FILE_CONFIG
#include <stdbool.h>
#include <stdio.h>
#include "assert.h"

int parse_it(struct test_case **test_cases, FILE *file, bool debug);
#endif
