#include <stdio.h>
#include <str.h>

int main(int argc, char **args) {
    string_t str = string.fromCharArray("Hello, world!");
    printf("%s\n", str.c_str);

    return 0;
}
