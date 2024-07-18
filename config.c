#include "assert.h"
#include "yaml.h"
#include <stdbool.h>
#include <stdio.h>

void debug_it(yaml_token_t *token) {
  yaml_token_type_t token_type = token->type;
  char *str = "";

  if (token_type == YAML_NO_TOKEN) {
    str = "YAML_NO_TOKEN";
  }

  if (token_type == YAML_STREAM_START_TOKEN) {
    str = "YAML_STREAM_START_TOKEN";
  }

  if (token_type == YAML_STREAM_END_TOKEN) {
    str = "YAML_STREAM_END_TOKEN";
  }

  if (token_type == YAML_VERSION_DIRECTIVE_TOKEN) {
    str = "YAML_VERSION_DIRECTIVE_TOKEN";
  }

  if (token_type == YAML_TAG_DIRECTIVE_TOKEN) {
    str = "YAML_TAG_DIRECTIVE_TOKEN";
  }

  if (token_type == YAML_DOCUMENT_START_TOKEN) {
    str = "YAML_DOCUMENT_START_TOKEN";
  }

  if (token_type == YAML_DOCUMENT_END_TOKEN) {
    str = "YAML_DOCUMENT_END_TOKEN";
  }

  if (token_type == YAML_BLOCK_SEQUENCE_START_TOKEN) {
    str = "YAML_BLOCK_SEQUENCE_START_TOKEN";
  }

  if (token_type == YAML_BLOCK_MAPPING_START_TOKEN) {
    str = "YAML_BLOCK_MAPPING_START_TOKEN";
  }

  if (token_type == YAML_BLOCK_END_TOKEN) {
    str = "YAML_BLOCK_END_TOKEN";
  }

  if (token_type == YAML_FLOW_SEQUENCE_START_TOKEN) {
    str = "YAML_FLOW_SEQUENCE_START_TOKEN";
  }

  if (token_type == YAML_FLOW_SEQUENCE_END_TOKEN) {
    str = "YAML_FLOW_SEQUENCE_END_TOKEN";
  }

  if (token_type == YAML_FLOW_MAPPING_START_TOKEN) {
    str = "YAML_FLOW_MAPPING_START_TOKEN";
  }

  if (token_type == YAML_FLOW_MAPPING_END_TOKEN) {
    str = "YAML_FLOW_MAPPING_END_TOKEN";
  }

  if (token_type == YAML_BLOCK_ENTRY_TOKEN) {
    str = "YAML_BLOCK_ENTRY_TOKEN";
  }

  if (token_type == YAML_FLOW_ENTRY_TOKEN) {
    str = "YAML_FLOW_ENTRY_TOKEN";
  }

  if (token_type == YAML_KEY_TOKEN) {
    str = "YAML_KEY_TOKEN";
  }

  if (token_type == YAML_VALUE_TOKEN) {
    str = "YAML_VALUE_TOKEN";
  }

  if (token_type == YAML_ALIAS_TOKEN) {
    str = "YAML_ALIAS_TOKEN";
  }

  if (token_type == YAML_ANCHOR_TOKEN) {
    str = "YAML_ANCHOR_TOKEN";
  }

  if (token_type == YAML_TAG_TOKEN) {
    str = "YAML_TAG_TOKEN";
  }

  if (token_type == YAML_SCALAR_TOKEN) {
    str = "YAML_SCALAR_TOKEN";
  }

  fprintf(stderr, "TOKEN TYPE: %s\n", str);

  if (token_type == YAML_SCALAR_TOKEN && token->data.scalar.value != NULL) {
    fprintf(stderr, "VALUE: %s\n", token->data.scalar.value);
  }
}

bool is_beginning_object_inside_array(yaml_token_t *previous_token,
                                      yaml_token_t *token) {
  return previous_token->type == YAML_BLOCK_ENTRY_TOKEN &&
         token->type == YAML_BLOCK_MAPPING_START_TOKEN;
}

int parse_it(struct test_case **test_cases, FILE *file, bool debug) {
  yaml_parser_t parser;
  yaml_event_t event;
  yaml_token_t token;

  int test_cases_length = -1;
  bool is_inside_tests = false;
  bool is_value = false;
  char *key;
  yaml_token_t previous_token;

  yaml_parser_initialize(&parser);

  yaml_parser_set_input_file(&parser, file);

  do {
    previous_token = token;

    yaml_parser_scan(&parser, &token);

    if (debug) {
      debug_it(&token);
    }

    if (previous_token.type == YAML_KEY_TOKEN &&
        token.type == YAML_SCALAR_TOKEN) {
      key = (char *)token.data.scalar.value;
    }

    if (previous_token.type == YAML_KEY_TOKEN &&
        token.type == YAML_SCALAR_TOKEN && strcmp(key, "tests") == 0) {
      is_inside_tests = true;

      continue;
    }

    if (previous_token.type == YAML_VALUE_TOKEN &&
        token.type == YAML_SCALAR_TOKEN) {
      is_value = true;
    } else {
      is_value = false;
    }

    if (is_beginning_object_inside_array(&previous_token, &token)) {
      if (test_cases_length > -1) {
        *test_cases = realloc(*test_cases, sizeof(struct test_case) *
                                               (test_cases_length + 2));

        if (!*test_cases) {
          perror("Failed!");
        }
      }

      test_cases_length = test_cases_length + 1;

      continue;
    }

    if (is_value && strcmp(key, "name") == 0) {
      (*test_cases)[test_cases_length].name = (char *)token.data.scalar.value;

      continue;
    }

    if (is_value && strcmp(key, "expect") == 0) {
      (*test_cases)[test_cases_length].expect = (char *)token.data.scalar.value;

      continue;
    }

    if (is_value && strcmp(key, "exec") == 0) {
      (*test_cases)[test_cases_length].exec = (char *)token.data.scalar.value;
    }
  } while (token.type != YAML_STREAM_END_TOKEN);

  return test_cases_length;
}
