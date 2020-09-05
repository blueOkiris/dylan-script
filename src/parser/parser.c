#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <str.h>
#include <token.h>
#include <parser.h>

static token_tree_t **token_trees_g = NULL;
static int num_token_trees_g = 0;

extern token_tree_t parser__new_token_tree() {
    if(num_token_trees_g == 0) {
        token_trees_g = malloc(sizeof(token_tree_t *));
    } else {
        token_tree_t **new_tree_list = malloc(
            sizeof(token_tree_t *) * (num_token_trees_g + 1)
        );
        
        token_tree_t **old_trees = token_trees_g;
        memcpy(new_tree_list, token_trees_g, num_token_trees_g);
        token_trees_g = new_tree_list;
        free(old_trees);
    }
    num_token_trees_g++;

    token_t root = (token_t) { string.from_char_array(""), 0, PROGRAM };
    token_tree_t new_tree;
    new_tree.index = num_token_trees_g - 1;
    new_tree.children = NULL;
    new_tree.root = root;
    return new_tree;
}

token_tree_t parser__append_child(token_tree_t child, token_tree_t tree) {
    token_tree_t new_tree = tree;
    if(new_tree.num_children == 0) {
        new_tree.children = malloc(sizeof(token_tree_t));
    } else {
        new_tree.children = realloc(
            new_tree.children,
            sizeof(token_tree_t) * (new_tree.num_children + 1)
        );
    }
    memcpy(
        new_tree.children + new_tree.num_children, &child,
        sizeof(token_tree_t)
    );
    token_trees_g[new_tree.index] = tree.children;
    new_tree.num_children++;
    return new_tree;
}

// <program> ::= { ( <import> ';' | <struct> | <func-def> ) }
extern token_tree_t parser__parse_program(token_list_t list) {
    token_tree_t new_tree = parser__new_token_tree();

    for(int i = 0; i < list.length; i++) {
        // All start with idents
        if(list.arr[i].kind != IDENTIFIER) {
            printf(
                "Parser error: %s : %s not 'struct,' 'fn,' or 'import'.\n",
                list.arr[i].text.c_str,
                tokenizer.kind_to_str(list.arr[i].kind).c_str
            );
            exit(-1);
        }
    }

    return new_tree;
}