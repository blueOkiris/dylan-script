#include <stdlib.h>
#include <str.h>
#include <lexer.h>
#include <token.h>

static token_t **tokens_g = NULL;
static int num_token_list_g = 0;

token_list_t tokenizer__appendToken(token_t tok, token_list_t list) {
    token_list_t new_list = list;
    if(new_list.arr == NULL) {
        new_list.arr = malloc(sizeof(token_t));
    } else {
        new_list.arr = realloc(new_list.arr, (new_list.length + 1) * sizeof(token_t));
    }
    new_list.arr[new_list.length] = tok;
    tokens_g[new_list.index] = new_list.arr;
    new_list.length++;
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

string_t tokenizer__kindToStr(token_kind_t kind) {
    switch(kind) {
        case STRING:
            return string.fromCharArray("STRING");
        case INTEGER:
            return string.fromCharArray("INTEGER");
        case FLOAT:
            return string.fromCharArray("FLOAT");
        case SCI_INT:
            return string.fromCharArray("SCI_INT");
        case SCI_FLOAT:
            return string.fromCharArray("SCI_FLOAT");
        case BOOL_VAL:
            return string.fromCharArray("BOOL_VAL");
        default:
            return string.fromCharArray("UNKNOWN");
    }
}
