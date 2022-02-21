#ifndef TOKENIZER_H
#define TOKENIZER_H

#include<stddef.h>
#include<stdbool.h>

typedef struct TOKEN {
    char *val;
    bool is_option;
} TOKEN;

void tokenizer(char *str, TOKEN **token_list, size_t *token_list_length);

#endif