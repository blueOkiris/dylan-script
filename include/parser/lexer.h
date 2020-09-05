#pragma once

#include <str.h>
#include <token.h>

token_list_t lexBoolVal(int *ref_i, string_t str, token_list_t list);
token_list_t lexNumber(int *ref_i, string_t str, token_list_t list);
token_list_t lexString(int *ref_i, string_t str, token_list_t list);
token_list_t fromStringHelper(token_list_t list, string_t str);
