#pragma once

typedef struct {
    int index;
    char *c_str;
    int length;
} string_t;

extern string_t string__instance();
extern string_t string__fromCharArray(const char *source);
extern string_t string__appendChar(char c, string_t source);
extern string_t string__appendString(string_t other, string_t source);

static const struct {
    string_t (*instance)();
    string_t (*fromCharArray)(const char *source);
    string_t (*appendChar)(char c, string_t source);
    string_t (*appendString)(string_t other, string_t source);
} string = {
    string__instance,
    string__fromCharArray,
    string__appendChar,
    string__appendString
};
