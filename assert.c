#include "assert.h"
#include "common.h"
#include "config.h"
#include "options.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

  char *str_temp = malloc(1);
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

    snprintf(*out, strlen(str_temp) + 1, "%s", str_temp);
  }

  pclose(fp);

  free(cmd);
  free(str_temp);

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

void print_usage(FILE *file) {
  fprintf(file,
          "USAGE:\n\n"
          "$ assert test_data.yml\n\n"
          "Or:"
          "\n"
          "\n"
          "$ assert - < test_data.yml"
          "\n"
          "\n"
          "OPTIONS:\n"
          "\n"
          "-f, --filter\n"
          "Run only tests that you want, based on the test name. Regex "
          "is supported.\n"
          "\n"
          "\n"
          "TEST DATA EXAMPLE:"
          "\n"
          "\n"
          "tests:\n"
          "- name: Test printf\n"
          "  exec: printf \"Hello, %s!\" \"world\"\n"
          "  expect: Hello, world!\n"
          "\n",
          "%s");
}

void handle_options_parse_error(int err) {
  if (err == OPTIONS_ERROR_NO_FILE_GIVEN) {
    print_usage(stderr);

    return;
  }

  fprintf(stderr, "Failed to parse options!\n");
}

int main(int argc, char **argv) {
  struct options *opts = malloc(sizeof(struct options));
  int options_parse_result = parse_options(opts, argc, argv);

  if (options_parse_result > 0) {
    handle_options_parse_error(options_parse_result);

    return 2;
  }

  struct test_case *test_cases = malloc(sizeof(struct test_case) * 2);

  int test_cases_length =
      parse_test_data(&test_cases, opts->test_data, opts->test_data_len, false);

  bool has_filter = !(opts->filter == NULL);

  if (test_cases_length < 1) {
    fprintf(stderr, "No tests found! Check the manual to see how to properly "
                    "define your test data.\n");

    return 1;
  }

  for (int i = 0; i < test_cases_length; i++) {
    if (has_filter && !regex_match(opts->filter, test_cases[i].name)) {
      continue;
    }

    if (run_test(&test_cases[i]) != 0) {
      return 1;
    }
  }

  return 0;
}
