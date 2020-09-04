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
        fgets(source_arr, source_len, source_file);

        token_list_t tokens = tokenizer.treeFromString(string.fromCharArray(source_arr));
    }

    return 0;
}
