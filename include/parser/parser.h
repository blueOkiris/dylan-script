#pragma once

#include <token.h>

typedef struct {
    token_t root;
    void *children;
    int num_children;
    int index;
} token_tree_t;

extern token_tree_t parser__new_token_tree();
extern token_tree_t parser__append_child(token_tree_t child, token_tree_t tree);
extern token_tree_t parser__parse_program(token_list_t list);

static const struct {
    token_tree_t (*new_token_tree)();
    token_tree_t (*append_child)(token_tree_t child, token_tree_t tree);
    token_tree_t (*parse_program)(token_list_t list);
} parser = {
    parser__new_token_tree,
    parser__append_child,
    parser__parse_program
};