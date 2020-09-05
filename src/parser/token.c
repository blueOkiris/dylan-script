#include <stdlib.h>
#include <str.h>
#include <lexer.h>
#include <token.h>

static token_t **tokens_g = NULL;
static int num_token_list_g = 0;

token_list_t tokenizer__append_token(token_t tok, token_list_t list) {
    token_list_t new_list = list;
    if(new_list.arr == NULL) {
        new_list.arr = malloc(sizeof(token_t));
    } else {
        new_list.arr = realloc(
            new_list.arr, (new_list.length + 1) * sizeof(token_t)
        );
    }
    new_list.arr[new_list.length] = tok;
    tokens_g[new_list.index] = new_list.arr;
    new_list.length++;
    return new_list;
}

token_list_t tokenizer__list_from_string(string_t str) {
    token_list_t new_list;
    new_list.index = num_token_list_g;
    new_list.arr = NULL;
    new_list.length = 0;

    if(num_token_list_g == 0) {
        tokens_g = malloc(sizeof(token_t *));
    } else {
        tokens_g = realloc(
            tokens_g, (num_token_list_g + 1) * sizeof(token_t *)
        );
    }
    tokens_g[new_list.index] = new_list.arr;
    num_token_list_g++;

    new_list = from_string_helper(new_list, str);

    return new_list;
}

string_t tokenizer__kind_to_str(token_kind_t kind) {
    switch(kind) {
        case STRING:
            return string.from_char_array("STRING");
        case INTEGER:
            return string.from_char_array("INTEGER");
        case FLOAT:
            return string.from_char_array("FLOAT");
        case SCI_INT:
            return string.from_char_array("SCI_INT");
        case SCI_FLOAT:
            return string.from_char_array("SCI_FLOAT");
        case BOOL_VAL:
            return string.from_char_array("BOOL_VAL");
        case IDENTIFIER:
            return string.from_char_array("IDENTIFIER");
        case MEMBER_OP:
            return string.from_char_array("MEMBER_OP");
        case SUM_OP:
            return string.from_char_array("SUM_OP");
        case MUL_OP:
            return string.from_char_array("MUL_OP");
        case EXP_OP:
            return string.from_char_array("EXP_OP");
        case BOOL_OP:
            return string.from_char_array("BOOL_OP");
        case COND_OP:
            return string.from_char_array("COND_OP");
        case ASSIGNMENT_OP:
            return string.from_char_array("ASSIGNMENT_OP");
        default:
            return string.from_char_array("UNKNOWN");
    }
}
