#ifndef ASSERT_FILE_COMMON
#define ASSERT_FILE_COMMON
#include <stdbool.h>

void wipe_line_break(char *str, int *len);

bool regex_match(char *pattern, char *subject);
#endif
