#include "options.h"
#include "assert.h"
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFERSIZE 1
#define ASSERT_TEST_BUFFER_SIZE 999999

void read_file(FILE *file, char *dst, int *len) {
  unsigned char buffer[BUFFERSIZE];
  int buffer_size = sizeof(unsigned char) * BUFFERSIZE;

  int i = 0;
  while (fread(&buffer, buffer_size, 1, file) > 0) {
    dst[i] = buffer[0];
    i = i + 1;
  }
  dst[i] = '\0';
  *len = i;
}

int parse_options(struct options *opts, int argc, char **argv) {
  int c;
  int digit_optind = 0;

  while (1) {
    int option_index = 0;

    static struct option long_options[] = {
        {"filter", required_argument, NULL, 'f'},
        {NULL, 0, NULL, 0},
    };

    c = getopt_long(argc, argv, "f:", long_options, &option_index);

    if (c == -1) {
      break;
    }

    switch (c) {
    case 'f':
      opts->filter = optarg;
      break;
    default:
      break;
    }
  }

  if (optind > -1) {
    opts->file = argv[optind];
  } else {
    return 1;
  }

  if (opts->file == NULL) {
    return OPTIONS_ERROR_NO_FILE_GIVEN;
  }

  char *test_data = malloc(ASSERT_TEST_BUFFER_SIZE);
  int test_data_len = 0;

  if (strcmp(opts->file, "-") == 0) {
    read_file(stdin, test_data, &test_data_len);
  } else {
    FILE *file = fopen(opts->file, "rb");
    if (file == NULL) {
      return 1;
    }

    read_file(file, test_data, &test_data_len);
  }

  opts->test_data = test_data;
  opts->test_data_len = test_data_len;

  return 0;
}
