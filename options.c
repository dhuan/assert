#include "assert.h"
#include <getopt.h>
#include <stdio.h>

int parse_options(struct options *opts, int argc, char **argv) {
  int c;
  int digit_optind = 0;

  while (1) {
    int option_index = 0;

    static struct option long_options[] = {
        {NULL, 0, NULL, 0},
    };

    c = getopt_long(argc, argv, "f:", long_options, &option_index);

    if (c == -1) {
      break;
    }

    switch (c) {
    default:
      break;
    }
  }

  if (optind > -1) {
    opts->file = argv[optind];
  } else {
    return 1;
  }

  return 0;
}
