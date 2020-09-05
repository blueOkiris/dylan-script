#include <stdio.h>
#include <token.h>
#include <parser.h>

static token_tree_t **token_trees_g = NULL;
static int num_token_trees_g = 0;

extern token_tree_t parser__ast_from_tokens(token_list_t list) {
    token_tree_t root_tree;
    root_tree.children = NULL;
    root_tree.num_children = 0;
    root_tree.index = num_token_trees_g;

    num_token_trees_g++;
    return root_tree;
}