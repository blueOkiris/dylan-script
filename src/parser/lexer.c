#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <str.h>
#include <token.h>
#include <lexer.h>

char is_whitespace(char c) {
    return c == ' ' || c == '\r' || c == '\n' || c == '\t';
}

char is_letter(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

char is_digit(char c) {
    return c >= '0' && c <= '9';
}

char is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' 
        || c == '^' || c == '&' || c == '|' || c == '<' || c == '>'
        || c == '=';
}

token_list_t from_string_helper(token_list_t list, string_t str) {
    token_list_t new_list = list;

    for(int i = 0; i < str.length; i++) {
        if(is_whitespace(str.c_str[i])) {
            continue;
        } else if(str.c_str[i] == '\'') {
            new_list = lex_string(&i, str, new_list);
        } else if(str.c_str[i] == '.' || is_digit(str.c_str[i])) {
            new_list = lex_number(&i, str, new_list);
        } else if(i + 3 < str.length && 
                (strncmp("true", str.c_str + i, 4) == 0 
                || strncmp("false", str.c_str + i, 5) == 0)) {
            new_list = lex_bool_val(&i, str, new_list);
        } else if(str.c_str[i] == '_' || is_letter(str.c_str[i])) {
            new_list = lex_identifier(&i, str, new_list);
        } else if(is_operator(str.c_str[i])) {
            new_list = lex_operator(&i, str, new_list);
        } else if(str.c_str[i] == ':'
                && i + 1 < str.length && str.c_str[i + 1] == '=') {
            new_list = lex_operator(&i, str, new_list);
        }
    }

    return new_list;
}

token_list_t lex_string(int *ref_i, string_t str, token_list_t list) {
    token_list_t new_list = list;
    
    int start_ind = *ref_i;
    string_t string_literal = string.instance();
    string_literal = string.append_char('\'', string_literal);
    (*ref_i)++;
    while(*ref_i < str.length) {
        if(str.c_str[*ref_i] == '\\') {
            string_literal = string.append_char(
                str.c_str[(*ref_i)++], string_literal
            );
            if(*ref_i >= str.length) {
                printf("Parse error: Unterminated escape sequence in string!");
                exit(-1);
            }

            string_literal = string.append_char(
                str.c_str[*ref_i], string_literal
            );
        } else if(str.c_str[*ref_i] == '\'') {
            string_literal = string.append_char(
                str.c_str[(*ref_i)++], string_literal
            );
            break;
        } else {
            string_literal = string.append_char(
                str.c_str[*ref_i], string_literal
            );
        }
        (*ref_i)++;
    }
    (*ref_i)--;

    token_t new_tok = (token_t) { string_literal, start_ind, STRING };
    new_list = tokenizer.append_token(new_tok, new_list);
    return new_list;
}

token_list_t lex_number(int *ref_i, string_t str, token_list_t list) {
    token_list_t new_list = list;
    int start_ind = *ref_i;
    char is_float = 0, is_sci = 0;

    string_t intPart = string.instance();
    while(*ref_i < str.length && is_digit(str.c_str[*ref_i])) {
        intPart = string.append_char(str.c_str[*ref_i], intPart);
        (*ref_i)++;
    }

    string_t floatPart = string.instance();
    if(*ref_i < str.length && str.c_str[*ref_i] == '.') {
        is_float = 1;
        floatPart = string.append_char(str.c_str[(*ref_i)++], floatPart);

        while(*ref_i < str.length && is_digit(str.c_str[*ref_i])) {
            floatPart = string.append_char(str.c_str[*ref_i], floatPart);
            (*ref_i)++;
        }
    }

    string_t sciPart = string.instance();
    if(*ref_i < str.length && str.c_str[*ref_i] == 'e') {
        is_sci = 1;
        sciPart = string.append_char(str.c_str[(*ref_i)++], sciPart);

        while(*ref_i < str.length && is_digit(str.c_str[*ref_i])) {
            sciPart = string.append_char(str.c_str[*ref_i], sciPart);
            (*ref_i)++;
        }
    }
    
    (*ref_i)--;

    string_t number = string.instance();
    number = string.append_string(intPart, number);
    number = string.append_string(floatPart, number);
    number = string.append_string(sciPart, number);

    token_t new_tok = (token_t) {
        number, start_ind,
        is_float ? (is_sci ? SCI_FLOAT : FLOAT) : (is_sci ? SCI_INT : INTEGER)
    };
    new_list = tokenizer.append_token(new_tok, new_list);
    return new_list;
}

token_list_t lex_bool_val(int *ref_i, string_t str, token_list_t list) {
    token_list_t new_list = list;
    int start_ind = *ref_i;

    if(str.c_str[*ref_i] == 't') {
        *ref_i += 4;

        token_t new_tok = (token_t) {
            string.from_char_array("true"), start_ind, BOOL_VAL
        };
        new_list = tokenizer.append_token(new_tok, new_list);
    } else {
        *ref_i += 5;

        token_t new_tok = (token_t) {
            string.from_char_array("false"), start_ind, BOOL_VAL
        };
        new_list = tokenizer.append_token(new_tok, new_list);
    }
    return new_list;
}

token_list_t lex_identifier(int *ref_i, string_t str, token_list_t list) {
    token_list_t new_list = list;
    int start_ind = *ref_i;

    string_t ident = string.instance();
    ident = string.append_char(str.c_str[*ref_i], ident);
    (*ref_i)++;
    while(str.c_str[*ref_i] == '_' 
            || is_letter(str.c_str[*ref_i]) || is_digit(str.c_str[*ref_i])) {
        ident = string.append_char(str.c_str[*ref_i], ident);
        (*ref_i)++;
    }

    token_t new_tok = (token_t) { ident, start_ind, IDENTIFIER };
    new_list = tokenizer.append_token(new_tok, new_list);

    if(str.c_str[*ref_i] == '.') {
        token_t tok2 = (token_t) {
            string.from_char_array("."), (*ref_i), MEMBER_OP
        };
        new_list = tokenizer.append_token(tok2, new_list);
        (*ref_i)++;
    }
    (*ref_i)--;

    return new_list;
}

token_list_t lex_operator(int *ref_i, string_t str, token_list_t list) {
    token_list_t new_list = list;
    int start_ind = *ref_i;

    token_t new_token;
    char op[3];
    switch(str.c_str[*ref_i]) {
        case '+':
        case '-':
            op[0] = str.c_str[*ref_i];
            op[1] = '\0';
            new_token = (token_t) {
                string.from_char_array(op), start_ind, SUM_OP
            };
            new_list = tokenizer.append_token(new_token, new_list);
            break;

        case '*':
        case '%':
            op[0] = str.c_str[*ref_i];
            op[1] = '\0';
            new_token = (token_t) {
                string.from_char_array(op), start_ind, MUL_OP
            };
            new_list = tokenizer.append_token(new_token, new_list);
            break;
        
        case '&':
        case '|':
            op[0] = str.c_str[*ref_i];
            op[1] = '\0';
            new_token = (token_t) {
                string.from_char_array(op), start_ind, BOOL_OP
            };
            new_list = tokenizer.append_token(new_token, new_list);
            break;
        
        case '=':
            new_token = (token_t) {
                string.from_char_array("="), start_ind, COND_OP
            };
            new_list = tokenizer.append_token(new_token, new_list);
            break;
        
        case '>':
        case '<':
            if((*ref_i) + 1 < str.length && str.c_str[(*ref_i) + 1] == '=') {
                op[0] = str.c_str[(*ref_i)++];
                op[1] = str.c_str[*ref_i];
                op[2] = '\0';
            } else {
                op[0] = str.c_str[*ref_i];
                op[1] = '\0';
            }
            new_token = (token_t) {
                string.from_char_array(op), start_ind, COND_OP
            };
            new_list = tokenizer.append_token(new_token, new_list);
            break;
        
        case ':':
            if((*ref_i) + 1 < str.length && str.c_str[(*ref_i) + 1] == '=') {
                op[0] = str.c_str[(*ref_i)++];
                op[1] = str.c_str[*ref_i];
                op[2] = '\0';
            } else {
                op[0] = str.c_str[*ref_i];
                op[1] = '\0';
            }
            new_token = (token_t) {
                string.from_char_array(op), start_ind, ASSIGNMENT_OP
            };
            new_list = tokenizer.append_token(new_token, new_list);
            break;
        
        case '/':
            if((*ref_i) + 1 < str.length && str.c_str[(*ref_i) + 1] == '=') {
                op[0] = str.c_str[(*ref_i)++];
                op[1] = str.c_str[*ref_i];
                op[2] = '\0';

                new_token = (token_t) {
                    string.from_char_array(op), start_ind, COND_OP
                };
            } else {
                op[0] = str.c_str[*ref_i];
                op[1] = '\0';

                new_token = (token_t) {
                    string.from_char_array(op), start_ind, MUL_OP
                };
            }
            new_list = tokenizer.append_token(new_token, new_list);
            break;
    }

    return new_list;
}
