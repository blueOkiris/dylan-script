#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <str.h>
#include <token.h>
#include <lexer.h>

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
        } else if(i + 3 < str.length && 
                (strncmp("true", str.c_str + i, 4) == 0 || strncmp("false", str.c_str + i, 5) == 0)) {
            new_list = lexBoolVal(&i, str, new_list);
        }
    }

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
    new_list = tokenizer.appendToken(new_tok, new_list);
    return new_list;
}

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
    new_list = tokenizer.appendToken(new_tok, new_list);
    return new_list;
}

token_list_t lexBoolVal(int *ref_i, string_t str, token_list_t list) {
    token_list_t new_list = list;
    int start_ind = *ref_i;

    if(str.c_str[*ref_i] == 't') {
        *ref_i += 3;

        token_t new_tok = (token_t) { string.fromCharArray("true"), start_ind, BOOL_VAL };
        new_list = tokenizer.appendToken(new_tok, new_list);
    } else {
        *ref_i += 4;

        token_t new_tok = (token_t) { string.fromCharArray("false"), start_ind, BOOL_VAL };
        new_list = tokenizer.appendToken(new_tok, new_list);
    }
    return new_list;
}
