#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <str.h>
#include <token.h>
#include <parser.h>

static token_tree_t **token_trees_g = NULL;
static int num_token_trees_g = 0;

token_tree_t parser__new_token_tree() {
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

    token_t root = (token_t) { string.from_char_array("program"), 0, PROGRAM };
    token_tree_t new_tree;
    new_tree.index = num_token_trees_g - 1;
    new_tree.children = NULL;
    new_tree.num_children = 0;
    new_tree.root = root;
    return new_tree;
}

token_tree_t parser__append_child(token_tree_t child, token_tree_t tree) {
    token_tree_t new_tree = tree;

    token_tree_t *old_children = (token_tree_t *) (new_tree.children);
    token_tree_t *new_children = malloc(
        sizeof(token_tree_t) * (new_tree.num_children + 1)
    );
    memcpy(
        new_children, old_children,
        sizeof(token_tree_t) * new_tree.num_children
    );
    new_children[new_tree.num_children] = child;
    free(old_children);

    new_tree.children = new_children;
    token_trees_g[new_tree.index] = new_children;
    new_tree.num_children++;
    return new_tree;
}

// <import> ::= 'import' <ident>
token_tree_t parse_import(int *ref_i, token_list_t list, token_tree_t t) {
    token_tree_t import_tree = parser__new_token_tree();
    token_t import_token = list.arr[*ref_i];
    import_token.kind = IMPORT;
    import_tree.root = import_token;

    (*ref_i)++;
    if(*ref_i >= list.length) {
        printf("Parser error: Unexpected EOF in import.\n");
        exit(-1);
    } else if(list.arr[*ref_i].kind != IDENTIFIER) {
        printf(
            "Parser error: Expected identifier, received '%s' : %s.\n",
            list.arr[*ref_i].text.c_str,
            tokenizer.kind_to_str(list.arr[*ref_i].kind).c_str
        );
        exit(-1);
    }
    token_tree_t ident_tree = parser__new_token_tree();
    ident_tree.root = list.arr[*ref_i];
    import_tree = parser__append_child(ident_tree, import_tree);

    return import_tree;
}

// <type-name> ::= [ 'ref' ] { '*' } < ident>
token_tree_t parse_type_name(int *ref_i, token_list_t list, token_tree_t t) {
    if(*ref_i >= list.length) {
        printf("Parser error: Unexpected EOF in type name.\n");
        exit(-1);
    }
    token_tree_t type_tree = parser__new_token_tree();
    type_tree.root.index = list.arr[*ref_i].index;
    type_tree.root.kind = TYPE_NAME;
    type_tree.root.text = string.from_char_array("type-name");
    
    if(*ref_i >= list.length) {
        printf("Parser error: Unexpected EOF in type name.\n");
        parser__print_tree(t, 0);
        printf("ERROR! See above\n");
        exit(-1);
    }
    if(list.arr[*ref_i].kind == IDENTIFIER
            && strcmp(list.arr[*ref_i].text.c_str, "ref") == 0) {
        token_tree_t ref_tree = parser__new_token_tree();
        ref_tree.root = list.arr[*ref_i];
        type_tree = parser__append_child(ref_tree, type_tree);
        (*ref_i)++;
        if(*ref_i >= list.length) {
            printf("Parser error: Unexpected EOF in type name after ref.\n");
            parser__print_tree(t, 0);
            printf("ERROR! See above\n");
            exit(-1);
        }
    }

    while(list.arr[*ref_i].kind == MUL_OP
            && strcmp(list.arr[*ref_i].text.c_str, "*") == 0) {
        token_tree_t ptr_tree = parser__new_token_tree();
        ptr_tree.root = list.arr[*ref_i];
        type_tree = parser__append_child(ptr_tree, type_tree);
        (*ref_i)++;
        if(*ref_i >= list.length) {
            printf("Parser error: Unexpected EOF in type name after '*'.\n");
            parser__print_tree(t, 0);
            printf("ERROR! See above\n");
            exit(-1);
        }
    }

    if(list.arr[*ref_i].kind != IDENTIFIER) {
        printf(
            "Parser error: Expected identifier, received '%s' : %s.\n",
            list.arr[*ref_i].text.c_str,
            tokenizer.kind_to_str(list.arr[*ref_i].kind).c_str
        );
        parser__print_tree(t, 0);
        printf("ERROR! See above\n");
        exit(-1);
    }
    token_tree_t ident_tree = parser__new_token_tree();
    ident_tree.root = list.arr[*ref_i];
    type_tree = parser__append_child(ident_tree, type_tree);

    return type_tree;
}

// <declaration> ::= <ident> ':' <type-name> [ ':=' <expr> ]
token_tree_t parse_declaration(int *ref_i, token_list_t list, token_tree_t t) {
    if(*ref_i >= list.length) {
        printf("Parser error: Unexpected EOF in variable declaration.\n");
        parser__print_tree(t, 0);
        printf("ERROR! See above\n");
        exit(-1);
    } else if(list.arr[*ref_i].kind != IDENTIFIER) {
        printf(
            "Parser error: Expected identifier, received '%s' : %s.\n",
            list.arr[*ref_i].text.c_str,
            tokenizer.kind_to_str(list.arr[*ref_i].kind).c_str
        );
        parser__print_tree(t, 0);
        printf("ERROR! See above\n");
        exit(-1);
    }

    token_tree_t dec_tree = parser__new_token_tree();
    dec_tree.root.text = string.from_char_array("declaration");
    dec_tree.root.kind = DECLARATION;
    dec_tree.root.index = list.arr[*ref_i].index;
    
    token_tree_t ident_tree = parser__new_token_tree();
    ident_tree.root = list.arr[*ref_i];
    dec_tree = parser__append_child(ident_tree, dec_tree);

    (*ref_i)++;
    if(*ref_i >= list.length) {
        printf("Parser error: Unexpected EOF in variable declaration.\n");
        parser__print_tree(t, 0);
        printf("ERROR! See above\n");
        exit(-1);
    } else if(list.arr[*ref_i].kind != COLON) {
        printf(
            "Parser error: Expected colon, received '%s' : %s.\n",
            list.arr[*ref_i].text.c_str,
            tokenizer.kind_to_str(list.arr[*ref_i].kind).c_str
        );
        parser__print_tree(t, 0);
        printf("ERROR! See above\n");
        exit(-1);
    }
    
    token_tree_t colon_tree = parser__new_token_tree();
    colon_tree.root = list.arr[*ref_i];
    dec_tree = parser__append_child(colon_tree, dec_tree);
    (*ref_i)++;

    token_tree_t type_tree = parse_type_name(ref_i, list, t);
    dec_tree = parser__append_child(type_tree, dec_tree);

    return dec_tree;
}

// <struct-body> ::= '{' { ( <declaration | <assignment> ) ';' } '}'
token_tree_t parse_struct_body(int *ref_i, token_list_t list, token_tree_t t) {
    if(list.arr[*ref_i].kind != BRACE && list.arr[*ref_i].kind != '{') {
        printf("Parser error: Expected '{' after struct declaration\n");
        parser__print_tree(t, 0);
        printf("ERROR! See above\n");
        exit(-1);
    }
    token_tree_t struct_body = parser__new_token_tree();
    struct_body.root.text = string.from_char_array("struct-body");
    struct_body.root.kind = STRUCT_BODY;
    struct_body.root.index = list.arr[*ref_i].index;

    token_tree_t lbrack_tree = parser__new_token_tree();
    lbrack_tree.root = list.arr[*ref_i];
    struct_body = parser__append_child(lbrack_tree, struct_body);
    (*ref_i)++;
    if(*ref_i >= list.length) {
        printf("Parser error: Unexpected EOF in struct declaration.\n");
        parser__print_tree(t, 0);
        printf("ERROR! See above\n");
        exit(-1);
    }

    while(list.arr[*ref_i].kind != BRACE) {
        token_tree_t dec_tree = parse_declaration(ref_i, list, t);
        struct_body = parser__append_child(dec_tree, struct_body);

        (*ref_i)++;
        if(*ref_i >= list.length) {
            printf("Parser error: Unexpected EOF after dec in struct.\n");
            parser__print_tree(t, 0);
            printf("ERROR! See above\n");
            exit(-1);
        } else if(list.arr[*ref_i].kind != SEMICOLON) {
            printf(
                "Parser error: Expected semicolon, received '%s' : %s.\n",
                list.arr[*ref_i].text.c_str,
                tokenizer.kind_to_str(list.arr[*ref_i].kind).c_str
            );
            parser__print_tree(t, 0);
            printf("ERROR! See above\n");
            exit(-1);
        }
        token_tree_t semi_colon_tree = parser__new_token_tree();
        semi_colon_tree.root = list.arr[*ref_i];
        struct_body = parser__append_child(semi_colon_tree, struct_body);

        (*ref_i)++;
        if(*ref_i >= list.length) {
            printf("Parser error: Unexpected EOF in struct declaration.\n");
            parser__print_tree(t, 0);
            printf("ERROR! See above\n");
            exit(-1);
        }
    }

    token_tree_t rbrack_tree = parser__new_token_tree();
    rbrack_tree.root = list.arr[*ref_i];
    struct_body = parser__append_child(rbrack_tree, struct_body);

    return struct_body;
}

// <struct> ::= 'struct' <ident> <scope-dec>
token_tree_t parse_struct(int *ref_i, token_list_t list, token_tree_t t) {
    token_tree_t struct_tree = parser__new_token_tree();
    token_t struct_token = list.arr[*ref_i];
    struct_token.kind = STRUCT;
    struct_tree.root = struct_token;

    (*ref_i)++;
    if(*ref_i >= list.length) {
        printf("Parser error: Unexpected EOF in struct declaration.\n");
        parser__print_tree(t, 0);
        printf("ERROR! See above\n");
        exit(-1);
    } else if(list.arr[*ref_i].kind != IDENTIFIER) {
        printf(
            "Parser error: Expected identifier, received '%s' : %s.\n",
            list.arr[*ref_i].text.c_str,
            tokenizer.kind_to_str(list.arr[*ref_i].kind).c_str
        );
        parser__print_tree(t, 0);
        printf("ERROR! See above\n");
        exit(-1);
    }
    token_tree_t ident_tree = parser__new_token_tree();
    token_t ident_token = list.arr[*ref_i];
    ident_tree.root = ident_token;
    struct_tree = parser__append_child(ident_tree, struct_tree);

    (*ref_i)++;
    if(*ref_i >= list.length) {
        printf("Parser error: Unexpected EOF in struct declaration.\n");
        parser__print_tree(t, 0);
        printf("ERROR! See above\n");
        exit(-1);
    }

    token_tree_t scop_dec = parse_struct_body(ref_i, list, t);
    struct_tree = parser__append_child(scop_dec, struct_tree);

    return struct_tree;
}

// <func-def> ::= 'fn' <ident> '(' [ <def-arg-body> ] ')' 
//                                                  '->' <type-name> <scope-dec>
token_tree_t parse_function(int *ref_i, token_list_t list, token_tree_t t) {
    token_tree_t function_tree = parser__new_token_tree();
    function_tree.root = list.arr[*ref_i];
    function_tree.root.kind = FUNCTION;
    
    return function_tree;
}

// <program> ::= { ( <import> | <struct> | <func-def> ) }
token_tree_t parser__parse_program(token_list_t list) {
    token_tree_t new_tree = parser__new_token_tree();

    for(int i = 0; i < list.length; i++) {
        // All start with idents
        if(list.arr[i].kind != IDENTIFIER
                || (strcmp(list.arr[i].text.c_str, "struct") != 0
                    && strcmp(list.arr[i].text.c_str, "fn") != 0
                    && strcmp(list.arr[i].text.c_str, "import") != 0)) {
            printf(
                "Parser error: '%s' : %s not 'struct,' 'fn,' or 'import'.\n",
                list.arr[i].text.c_str,
                tokenizer.kind_to_str(list.arr[i].kind).c_str
            );
            parser__print_tree(new_tree, 0);
            printf("ERROR! See above\n");
            exit(-1);
        } else if(strcmp(list.arr[i].text.c_str, "import") == 0) {
            new_tree = parser__append_child(
                parse_import(&i, list, new_tree), new_tree
            );
        } else if(strcmp(list.arr[i].text.c_str, "fn") == 0) {
            new_tree = parser__append_child(
                parse_function(&i, list, new_tree), new_tree
            );
        } else if(strcmp(list.arr[i].text.c_str, "struct") == 0) {
            token_tree_t struct_tree = parse_struct(&i, list, new_tree);
            new_tree = parser__append_child(struct_tree, new_tree);
        }
    }

    return new_tree;
}

void parser__print_tree(token_tree_t tree, int tab_ind) {
    for(int i = 0; i < tab_ind; i++) {
        printf("|--");
    }
    printf(
        "%s : %s\n", 
        tree.root.text.c_str,
        tokenizer.kind_to_str(tree.root.kind).c_str
    );

    for(int i = 0; i < tree.num_children; i++) {
        parser__print_tree(((token_tree_t *) tree.children)[i], tab_ind + 1);
    }
}
