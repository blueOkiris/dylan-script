#include <stdio.h>
#include <stdlib.h>
#include <str.h>
#include <token.h>

static token_t **tokens_g = NULL;
static int num_token_list_g = 0;

token_list_t tokenizer__treeFromString(string_t str) {
    token_list_t new_list;
    new_list.index = num_token_list_g;
    new_list.arr = malloc(sizeof(token_t));
    new_list.size = 0;

    if(num_token_list_g == 0) {
        tokens_g = malloc(sizeof(token_t *));
    } else {
        tokens_g = realloc(tokens_g, (num_token_list_g + 1) * sizeof(token_t *));
    }
    tokens_g[new_list.index] = new_list.arr;
    num_token_list_g++;

    return new_list;
}
