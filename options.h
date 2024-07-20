#ifndef ASSERT_FILE_OPTIONS
#define ASSERT_FILE_OPTIONS
#include "assert.h"
#include <stdbool.h>
#include <stdio.h>

typedef enum parse_options_error {
  OPTIONS_ERROR_NONE,
  OPTIONS_ERROR_NO_FILE_GIVEN,
} yaml_scalar_style_t;

int parse_options(struct options *opts, int argc, char **argv);
#endif
