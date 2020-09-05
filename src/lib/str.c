#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <str.h>

static char **strings_g = NULL;
static int num_strings_g = 0;

string_t string__instance() {
    if(num_strings_g == 0) {
        strings_g = malloc(sizeof(char **));
    } else {
        strings_g = realloc(strings_g, (num_strings_g + 1) * sizeof(char *));
    }

    strings_g[num_strings_g] = malloc(1);
    strings_g[num_strings_g][0] = '\0';
    num_strings_g++;

    return (string_t) { num_strings_g - 1, strings_g[num_strings_g - 1], 0 };
}

string_t string__from_char_array(const char *source) {
    string_t new_str = string__instance();
    
    size_t len = strlen(source);
    for(size_t i = 0; i < len; i++) {
        new_str = string__append_char(source[i], new_str);
    }

    return new_str;
}

string_t string__append_char(char c, string_t source) {
    char *new_src = malloc(source.length + 2);
    string_t new_str;

    memcpy(new_src, source.c_str, source.length);
    new_src[source.length] = c;
    new_src[source.length + 1] = '\0';

    new_str.c_str = new_src;
    strings_g[source.index] = new_src;
    free(source.c_str);

    new_str.length = source.length + 1;
    new_str.index = source.index;
    return new_str;
}

string_t string__append_string(string_t other, string_t source) {
    char *new_src = malloc(source.length + other.length + 1);
    string_t new_str;

    memcpy(new_src, source.c_str, source.length);
    memcpy(new_src + source.length, other.c_str, other.length);
    new_src[source.length + other.length] = '\0';

    new_str.c_str = new_src;
    strings_g[source.index] = new_src;
    free(source.c_str);

    new_str.length = source.length + other.length;
    new_str.index = source.index;
    return new_str;
}

string_t string__remove(int start_ind, int len, string_t source) {
    char *new_src = malloc(source.length - len + 1);
    string_t new_str;

    memcpy(new_src, source.c_str + start_ind, source.length - len);
    new_src[source.length - len] = '\0';

    new_str.c_str = new_src;
    strings_g[source.index] = new_src;
    free(source.c_str);

    new_str.length = source.length - len;
    new_str.index = source.index;
    return new_str;
}
