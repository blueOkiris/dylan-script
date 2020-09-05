#pragma once

#include <str.h>

typedef enum {
    STRING, INTEGER, SCI_INT, SCI_FLOAT, FLOAT, BOOL_VAL, IDENTIFIER,
    SUM_OP, MUL_OP, EXP_OP, BOOL_OP, COND_OP, ASSIGNMENT_OP, MEMBER_OP, RET_OP,
    COMMA, BRACE, PARENTH, COLON, SEMICOLON,
    
    PROGRAM, IMPORT, STRUCT
} token_kind_t;

typedef struct {
    string_t text;
    int index;
    token_kind_t kind;
} token_t;

typedef struct {
    int index;
    token_t *arr;
    int length;
} token_list_t;

extern token_list_t tokenizer__append_token(token_t tok, token_list_t list);
extern token_list_t tokenizer__list_from_string(string_t str);
extern string_t tokenizer__kind_to_str(token_kind_t kind);

static const struct {
    token_list_t (*append_token)(token_t tok, token_list_t list);
    token_list_t (*list_from_string)(string_t str);
    string_t (*kind_to_str)(token_kind_t kind);
} tokenizer = {
    tokenizer__append_token,
    tokenizer__list_from_string,
    tokenizer__kind_to_str
};
