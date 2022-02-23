#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<token.h>
#include<constants.h>

TOKEN* new_token(void) {
    TOKEN *tok = (TOKEN *)malloc(sizeof(TOKEN));

    if(tok != NULL) {
        tok->is_background = false;
        tok->is_option = false;
        tok->val = "";
    }

    return tok;
}

//Tokenize the string
void tokenize(char *str, TOKEN **token_list, size_t* token_list_length) {
    int n = strlen(str);

    //Count the number of tokens
    *token_list_length = 1;
    for(int i = 0; i < n; i++) {
        if(str[i] == ' ')
            (*token_list_length)++;
    }

    *token_list = (TOKEN *)malloc(sizeof(TOKEN)*(*token_list_length));
    if(token_list == NULL) {
        fprintf(stderr, error_msg);
        exit(1);
    }
    
    char *buffer = NULL;

    (*token_list_length) = 0;
    while((buffer = strsep(&str, " \n")) != NULL) {
        if(strlen(buffer) == 0)
            continue;

        TOKEN *curr = NULL;

        if(strcmp(buffer, "&") == 0) 
            (*token_list)[0].is_background = true;
        else {
            curr = new_token();

            if(buffer[0] == '-') {
                curr->is_option = true;
                buffer = strcpy(buffer, buffer + 1);
            }
    
            curr->val = strdup(buffer);
        }

        (*token_list)[(*token_list_length)++] = *curr;
    }
    
    return;
}