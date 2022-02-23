#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<stdbool.h>
#include<sys/wait.h>

//Custom Headers
#include<constants.h>
#include<token.h>
#include<history.h>
#include<built-in.h>

HISTORY_STORE *history_store;

void handler(TOKEN *token_list, size_t token_list_length);

int main(int argc, char* argv[]) {
    if(argc > 2) {
        //Print an error
        fprintf(stderr, "More number of arguments than required\n");
        exit(1);
    }
    
    char *buffer = NULL;
    size_t line_length = 0;

    TOKEN *token_list = NULL;
    history_store = create_history_store();
    size_t token_list_length = 0, cmd_len;

    //Interactive Mode
    if(argc == 1) {
        bool flag = false;

        while(!flag) {
            fprintf(stdout, "csh> ");
            getline(&buffer, &line_length, stdin);

            if((cmd_len = strlen(buffer)) == 1)
                continue;

            buffer[cmd_len - 1] = '\0';

            tokenize(strdup(buffer), &token_list, &token_list_length);
            handler(token_list, token_list_length);
            add_history(history_store, buffer);

            free(token_list);
        }
    }
    //Batch Mode
    else {
        char *batch_file = strdup(argv[1]);
        FILE* file_ptr = NULL;

        if((file_ptr = fopen(batch_file, "r")) == NULL) {
            fprintf(stderr, error_msg);
            exit(1);
        }

        while(getline(&buffer, &line_length, file_ptr) != EOF) {
            //Skip empty lines
            if((cmd_len = strlen(buffer)) == 1)
                continue;

            buffer[cmd_len - 1] = '\0';

            tokenize(strdup(buffer), &token_list, &token_list_length);
            handler(token_list, token_list_length);
            add_history(history_store, buffer);

            free(token_list);
        }

        //Close the file
        fclose(file_ptr);
    }

    free(buffer);
    free(history_store);

    return 0;
}

void handler(TOKEN *token_list, size_t token_list_length) {
    int built_in_code = get_builtin(token_list[0].val);

    if(built_in_code == -1) {}
    else
        builtin_handler(built_in_code, token_list, token_list_length);

    return;
}