#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stddef.h>
#include<ctype.h>

#include<token.h>
#include<built-in.h>
#include<history.h>

extern HISTORY_STORE *history_store;

const char* built_ins[] = {
    "cd",
    "help",
    "exit",
    "pwd",
    "history",
    "path"
};

int get_builtin(char *cmd) {
    int code = -1;

    for(size_t i = 0; i < BUILT_INS; i++) {
        if(strcmp(cmd, built_ins[i]) == 0) {
            code = i;
            break;
        }
    }

    return code;
}

void builtin_handler(int code, TOKEN *token_list, size_t token_list_length) {
    switch(code) {
        case 1 :
            help(token_list, token_list_length);
            break;
        case 2:
            exit_shell();
            break;
        case 4:
            history_cmd(token_list, token_list_length);
            break;
    }
    
    return;
}

void help(TOKEN *token_list, size_t token_list_length) {
    //TODO: Print help text

    return;
}

void history_cmd(TOKEN *token_list, size_t token_list_length) {
    size_t start = 0;

    if(token_list_length == 2) {
        start = atoi(token_list[1].val);

        if(start == 0 && (strlen(token_list[0].val) == 0 || !isdigit(token_list[0].val[0]))) {
            fprintf(stdout, "A numeric argument greater than 0 is required\n");
            return;
        }

        start = ((long long int)history_store->curr - (long long int)start >= 0) ? history_store->curr - start : 0;
    }

    for(size_t i = start; i < history_store->curr; i++)
        fprintf(stdout, "%lu %s\n", i + 1, (history_store->store)[i]);

    return;
}

void exit_shell(void) {
    exit(0);
}