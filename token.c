#include<token.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//Tokenize the string
void tokenizer(char *str, TOKEN **token_list, size_t* token_list_length) {
    int n = strlen(str);

    //Count the number of tokens
    *token_list_length = 1;
    for(int i = 0; i < n; i++) {
        if(str[i] == ' ')
            (*token_list_length)++;
    }

    *token_list = (TOKEN *)malloc(sizeof(TOKEN)*(*token_list_length));
    char *buffer = NULL;

    (*token_list_length) = 0;
    while((buffer = strsep(&str, " \n")) != NULL) {
        if(strlen(buffer) == 0)
            continue;
        
        TOKEN *curr = (TOKEN *)malloc(sizeof(TOKEN));

        if(buffer[0] == '-') {
            curr->is_option = true;
            buffer = strcpy(buffer, buffer + 1);
        }
 
        curr->val = strdup(buffer);
        (*token_list)[(*token_list_length)++] = *curr;
    }
    
    free(buffer);
    return;
}