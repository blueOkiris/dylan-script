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
token_tree_t parse_import(int *ref_i, token_list_t list) {
    (*ref_i)++;
    if(*ref_i >= list.length) {
        printf("Parser error: Unexpected EOF in import.\n");
        exit(-1);
    } else if(list.arr[*ref_i].kind != IDENTIFIER) {
        printf(
            "Parser error: Expected identifier, recieved %s : %s\n.",
            list.arr[*ref_i].text.c_str,
            tokenizer.kind_to_str(list.arr[*ref_i].kind).c_str
        );
        exit(-1);
    }

    token_tree_t import_tree = parser__new_token_tree();
    token_t import_token = list.arr[(*ref_i) - 1];
    import_token.kind = IMPORT;
    import_tree.root = import_token;
    token_tree_t ident_tree = parser__new_token_tree();
    token_t ident_token = list.arr[*ref_i];
    ident_tree.root = ident_token;
    import_tree = parser__append_child(ident_tree, import_tree);
    return import_tree;
}

// <program> ::= { ( <import> | <struct> | <func-def> ) }
token_tree_t parser__parse_program(token_list_t list) {
    token_tree_t new_tree = parser__new_token_tree();

    for(int i = 0; i < list.length; i++) {
        // All start with idents
        /*if(list.arr[i].kind != IDENTIFIER
                || (strcmp(list.arr[i].text.c_str, "struct") != 0
                    && strcmp(list.arr[i].text.c_str, "fn") != 0
                    && strcmp(list.arr[i].text.c_str, "import") != 0)) {
            printf(
                "Parser error: %s : %s not 'struct,' 'fn,' or 'import'.\n",
                list.arr[i].text.c_str,
                tokenizer.kind_to_str(list.arr[i].kind).c_str
            );
            exit(-1);
        } else*/ if(strcmp(list.arr[i].text.c_str, "import") == 0) {
            new_tree = parser__append_child(parse_import(&i, list), new_tree);
        } else if(strcmp(list.arr[i].text.c_str, "fn") == 0) {
            //new_tree = parser__append_child(parse_function(&i, list), new_tree);
        } else if(strcmp(list.arr[i].text.c_str, "struct") == 0) {
            //new_tree = parser__append_child(parse_struct(&i, list), new_tree);
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
        for(int j = 0; j < tab_ind; j++) {
            printf("|--");
        }
        token_tree_t child;
        child = ((token_tree_t *) tree.children)[i];
        parser__print_tree(child, tab_ind + 1);
    }
}
