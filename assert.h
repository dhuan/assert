#ifndef ASSERT_FILE_TYPES
#define ASSERT_FILE_TYPES

#define SIZEOF_STR 1024     /* Default string size. */
#define SIZEOF_MED_STR 8192 /* Medium string size. */

struct test_case {
  char *name;
  char *exec;
  char *expect;
};

struct options {
  char *file;
};

#endif
