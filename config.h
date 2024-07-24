#ifndef ASSERT_FILE_CONFIG
#define ASSERT_FILE_CONFIG
#include "assert.h"
#include <stdbool.h>
#include <stdio.h>

int parse_test_data(struct test_case **test_cases, char *test_data, int test_data_len, bool debug);
#endif
