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
#include<commands.h>

HISTORY_STORE *history_store;

void handler(TOKEN *token_list, size_t token_list_length);

int main(int argc, char* argv[]) {
    if(argc > 2) {
        //Print an error
        fprintf(stderr, "More number of arguments than required\n");
        exit(1);
    }

    bool is_interactive_mode = (argc == 1);

    FILE* file_ptr = stdin;
    if(!is_interactive_mode) {
        char *batch_file = strdup(argv[1]);

        if((file_ptr = fopen(batch_file, "r")) == NULL) {
            fprintf(stderr, error_msg);
            exit(1);
        }
    }

    history_store = create_history_store(); 

    char *buffer = NULL;
    TOKEN *token_list = NULL;
    size_t token_list_length, buffer_size, cmd_len;
    token_list_length = buffer_size = 0;

    while(true) {
        if(is_interactive_mode)
            printf("csh> ");

        if((cmd_len = getline(&buffer, &buffer_size, file_ptr)) == EOF)
            break;

        //Skip empty lines
        if(cmd_len == 1)
            continue;

        buffer[cmd_len - 1] = '\0';

        tokenize(strdup(buffer), &token_list, &token_list_length);
        handler(token_list, token_list_length);        
        add_history(history_store, buffer);

        free(token_list);
    }

    //Close the file
    if(!is_interactive_mode)
        fclose(file_ptr);

    free(buffer);
    free(history_store);

    return 0;
}

void handler(TOKEN *token_list, size_t token_list_length) {
    int built_in_code = get_builtin(token_list[0].val);

    if(built_in_code == -1) {
        char *program_path = check_cmd(token_list);

        if(strlen(program_path) == 0) {
            printf("Command '%s' not found\n", token_list[0].val);
            return;
        }

        execute_command(program_path, token_list, token_list_length);
    }
    else
        builtin_handler(built_in_code, token_list, token_list_length);

    return;
}