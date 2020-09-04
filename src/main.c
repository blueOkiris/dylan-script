#include <stdio.h>
#include <str.h>
#include <token.h>

int main(int argc, char **args) {
    if(argc < 2) {
        printf("No input file given\n");
    } else {
        FILE *source_file = fopen(args[1], "r");
        fseek(source_file, 0, SEEK_END);
        size_t source_len = ftell(source_file);
        rewind(source_file);
        char source_arr[source_len + 1];
        fgets(source_arr, source_len + 1, source_file);

        token_list_t tokens = tokenizer.listFromString(string.fromCharArray(source_arr));
        for(int i = 0; i < tokens.length; i++) {
            printf(
                "Token { index = %d, string = %s, type = %s }\n", 
                tokens.arr[i].index, tokens.arr[i].text.c_str,
                tokenizer.kindToStr(tokens.arr[i].kind).c_str
            );
        }
    }

    return 0;
}
