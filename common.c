#include "common.h"
#include <regex.h>
#include <stdbool.h>
#include <string.h>

void wipe_line_break(char *str, int *len) {
  if (str[(*len) - 2] == '\n') {
    str[(*len) - 2] = '\0';
    *len = (*len) - 1;
  }
}

bool regex_match(char *pattern, char *subject) {
  regex_t re;
  int result = regcomp(&re, pattern, 0);

  if (result != 0) {
    return false;
  }

  result = regexec(&re, subject, 0, NULL, 0);

  return result == 0;
}
