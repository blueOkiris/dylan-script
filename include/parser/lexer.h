#pragma once

#include <str.h>
#include <token.h>

char is_whitespace(char c);
char is_letter(char c);
char is_digit(char c);
char is_operator(char c);

token_list_t from_string_helper(token_list_t list, string_t str);
token_list_t lex_string(int *ref_i, string_t str, token_list_t list);
token_list_t lex_number(int *ref_i, string_t str, token_list_t list);
token_list_t lex_bool_val(int *ref_i, string_t str, token_list_t list);
token_list_t lex_identifier(int *ref_i, string_t str, token_list_t list);
token_list_t lex_operator(int *ref_i, string_t str, token_list_t list);
