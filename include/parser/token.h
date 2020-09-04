#pragma once

#include <str.h>

typedef enum {
    STRING,
    INTEGER,
    SCI_NUM,
    FLOAT,
    BOOL_VAL,
    IDENTIFIER,
    SUM_OP,
    MUL_OP,
    EXP_OP
} token_kind_t;

typedef struct {
    string_t text;
    int index;
    token_kind_t kind;
} token_t;
