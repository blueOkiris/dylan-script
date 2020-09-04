#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <str.h>
#include <token.h>

static token_t **tokens_g = NULL;
static int num_token_list_g = 0;

token_list_t lexNumber(int *ref_i, string_t str, token_list_t list) {
    token_list_t new_list = list;
    int start_ind = *ref_i;
    char is_float = 0, is_sci = 0;

    string_t intPart = string.instance();
    while(*ref_i < str.length && str.c_str[*ref_i] >= '0' && str.c_str[*ref_i] <= '9') {
        intPart = string.appendChar(str.c_str[*ref_i], intPart);
        (*ref_i)++;
    }

    string_t floatPart = string.instance();
    if(*ref_i < str.length && str.c_str[*ref_i] == '.') {
        is_float = 1;
        floatPart = string.appendChar(str.c_str[(*ref_i)++], floatPart);

        while(*ref_i < str.length && str.c_str[*ref_i] >= '0' && str.c_str[*ref_i] <= '9') {
            floatPart = string.appendChar(str.c_str[*ref_i], floatPart);
            (*ref_i)++;
        }
    }

    string_t sciPart = string.instance();
    if(*ref_i < str.length && str.c_str[*ref_i] == 'e') {
        is_sci = 1;
        sciPart = string.appendChar(str.c_str[(*ref_i)++], sciPart);

        while(*ref_i < str.length && str.c_str[*ref_i] >= '0' && str.c_str[*ref_i] <= '9') {
            sciPart = string.appendChar(str.c_str[*ref_i], sciPart);
            (*ref_i)++;
        }
    }
    
    (*ref_i)--;

    string_t number = string.instance();
    number = string.appendString(intPart, number);
    number = string.appendString(floatPart, number);
    number = string.appendString(sciPart, number);

    token_t new_tok = (token_t) {
        number, start_ind,
        is_float ? (is_sci ? SCI_FLOAT : FLOAT) : (is_sci ? SCI_INT : INTEGER)
    };
    new_list = tokenizer__appendToken(new_tok, new_list);
    return new_list;
}

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
    (*ref_i)--;

    token_t new_tok = (token_t) { string_literal, start_ind, STRING };
    new_list = tokenizer__appendToken(new_tok, new_list);
    return new_list;
}

token_list_t fromStringHelper(token_list_t list, string_t str) {
    token_list_t new_list = list;

    for(int i = 0; i < str.length; i++) {
        if(str.c_str[i] == ' ' || str.c_str[i] == '\r'
                || str.c_str[i] == '\n' || str.c_str[i] == '\t') {
            i++;
        } if(str.c_str[i] == '\'') {
            new_list = lexString(&i, str, new_list);
        } else if(str.c_str[i] == '.' || (str.c_str[i] >= '0' && str.c_str[i] <= '9')) {
            new_list = lexNumber(&i, str, new_list);
        }
    }

    return new_list;
}

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
        default:
            return string.fromCharArray("UNKNOWN");
    }
}
