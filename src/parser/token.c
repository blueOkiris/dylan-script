#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <str.h>
#include <token.h>

static token_t **tokens_g = NULL;
static int num_token_list_g = 0;

token_list_t lexString(int *ref_i, string_t str, token_list_t list) {
    token_list_t new_list = list;
    
    int start_ind = *ref_i;
    string_t string_literal = string.instance();
    string_literal = string.appendChar('\'', string_literal);
    (*ref_i)++;
    while(*ref_i < str.length) {
        if(str.c_str[*ref_i] == '\\') {
            string_literal = string.appendChar(str.c_str[(*ref_i)++], string_literal);
            if(*ref_i >= str.length) {
                printf("Parse error: Unterminated escape sequence in string literal!");
                exit(-1);
            }

            string_literal = string.appendChar(str.c_str[*ref_i], string_literal);
        } else if(str.c_str[*ref_i] == '\'') {
            string_literal = string.appendChar(str.c_str[(*ref_i)++], string_literal);
            break;
        } else {
            string_literal = string.appendChar(str.c_str[*ref_i], string_literal);
        }
        (*ref_i)++;
    }

    token_t new_tok = (token_t) { string_literal, start_ind, STRING };
    new_list = tokenizer__appendToken(new_tok, new_list);

    return new_list;
}

token_list_t fromStringHelper(token_list_t list, string_t str) {
    token_list_t new_list = list;

    for(int i = 0; i < str.length; i++) {
        if(str.c_str[i] == '\'') {
            new_list = lexString(&i, str, list);
        }
    }

    return new_list;
}

token_list_t tokenizer__appendToken(token_t tok, token_list_t list) {
    token_t *new_tok_arr = malloc((list.length + 1) * sizeof(token_t));
    token_list_t new_list;

    memcpy(new_tok_arr, list.arr, list.length);
    new_tok_arr[list.length] = tok;
    
    new_list.arr = new_tok_arr;
    tokens_g[list.index] = new_tok_arr;
    if(list.arr != NULL) {
        free(list.arr);
    }

    new_list.length = list.length + 1;
    new_list.index = list.index;
    return new_list;
}

token_list_t tokenizer__listFromString(string_t str) {
    token_list_t new_list;
    new_list.index = num_token_list_g;
    new_list.arr = NULL;
    new_list.length = 0;

    if(num_token_list_g == 0) {
        tokens_g = malloc(sizeof(token_t *));
    } else {
        tokens_g = realloc(tokens_g, (num_token_list_g + 1) * sizeof(token_t *));
    }
    tokens_g[new_list.index] = new_list.arr;
    num_token_list_g++;

    new_list = fromStringHelper(new_list, str);

    return new_list;
}
