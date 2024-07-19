#include "assert.h"
#include "config.h"
#include "options.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void wipe_line_break(char *str, int *len) {
  if (str[(*len) - 2] == '\n') {
    str[(*len) - 2] = '\0';
    *len = (*len) - 1;
  }
}

int exec(char *cmdBase, char **out) {
  FILE *fp;
  char path[SIZEOF_MED_STR];
  int len;
  *out = NULL;

  char *cmd = malloc(SIZEOF_MED_STR);
  if (cmd == NULL) {
    return 1;
  }

  snprintf(cmd, SIZEOF_MED_STR, "%s 2>&1", cmdBase);

  fp = popen(cmd, "r");
  if (fp == NULL) {
    return 1;
  }

  char *str_temp;
  while (fgets(path, sizeof path, fp) != NULL) {
    if (*out == NULL) {
      len = strlen(path) + 1;
      wipe_line_break(path, &len);
      *out = malloc(len);
      if (*out == NULL) {
        return 1;
      }

      snprintf(*out, len, "%s", path);

      continue;
    }

    len = sizeof(char) * (strlen(*out) + strlen(path) + 2);

    str_temp = realloc(str_temp, len);
    if (str_temp == NULL) {
      return 1;
    }

    snprintf(str_temp, len, "%s\n%s", *out, path);

    *out = realloc(*out, strlen(str_temp));
    if (*out == NULL) {
      return 1;
    }

    snprintf(*out, strlen(str_temp), "%s", str_temp);
  }

  pclose(fp);

  free(cmd);

  return 0;
}

int run_test(struct test_case *tc) {
  char *out;
  if (exec(tc->exec, &out) > 0) {
    fprintf(stderr, "Failed to run command: %s\n", tc->exec);
  }

  if (strcmp(out, tc->expect) == 0) {
    printf("✔ %s\n", tc->name);
  } else {
    printf("✗ %s\nExpected: %s\nGot:      %s\n", tc->name, tc->expect, out);

    return 1;
  }

  return 0;
}

int main(int argc, char **argv) {
  struct options *opts = malloc(sizeof(struct options));
  int options_parse_result = parse_options(opts, argc, argv);

  if (options_parse_result > 0) {
    printf("Failed to parse options!\n");

    return 2;
  }

  struct test_case *test_cases = malloc(sizeof(struct test_case) * 2);

  int test_cases_length =
      parse_it(&test_cases, opts->test_data, opts->test_data_len, false);

  for (int i = 0; i <= test_cases_length; i++) {
    if (run_test(&test_cases[i]) != 0) {
      return 1;
    }
  }

  return 0;
}
