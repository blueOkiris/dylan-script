#pragma once

typedef struct {
    int index;
    char *c_str;
    int length;
} string_t;

extern string_t string__instance();
extern string_t string__from_char_array(const char *source);
extern string_t string__append_char(char c, string_t source);
extern string_t string__append_string(string_t other, string_t source);
extern string_t string__remove(int start_ind, int len, string_t source);
extern void string__free_all();

static const struct {
    string_t (*instance)();
    string_t (*from_char_array)(const char *source);
    string_t (*append_char)(char c, string_t source);
    string_t (*append_string)(string_t other, string_t source);
    string_t (*remove)(int start_ind, int len, string_t source);
    void (*free_all)();
} string = {
    string__instance,
    string__from_char_array,
    string__append_char,
    string__append_string,
    string__remove,
    string__free_all
};
