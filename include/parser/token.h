#pragma once

#include <str.h>

typedef enum {
    STRING, INTEGER, SCI_NUM, FLOAT, BOOL_VAL, IDENTIFIER,
    SUM_OP, MUL_OP, EXP_OP, PARENTH, BOOL_OP, COND_OP, COMMA,
    BRACE, COLON, SEMICOLON
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

extern token_list_t tokenizer__appendToken(token_t tok, token_list_t list);
extern token_list_t tokenizer__listFromString(string_t str);
extern string_t tokenizer__kindToStr(token_kind_t kind);

static const struct {
    token_list_t (*appendToken)(token_t tok, token_list_t list);
    token_list_t (*listFromString)(string_t str);
    string_t (*kindToStr)(token_kind_t kind);
} tokenizer = {
    tokenizer__appendToken,
    tokenizer__listFromString,
    tokenizer__kindToStr
};
