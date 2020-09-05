#pragma once

#include <token.h>

typedef struct {
    token_t root;
    struct token_tree_t *children;
    int num_children;
    int index;
} token_tree_t;

extern token_tree_t parser__ast_from_tokens(token_list_t list);

static const struct {
    token_tree_t (*ast_from_tokens)(token_list_t list);
} parser = {
    parser__ast_from_tokens
};