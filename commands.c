#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<stddef.h>
#include<fcntl.h> //For X_ constants
#include<sys/wait.h>

#include<token.h>
#include<constants.h>

const char *PATH_FILE = "PATH";

char* check_cmd(TOKEN *token_list) {
    FILE* file_ptr = NULL;

    if((file_ptr = fopen(PATH_FILE, "r")) == NULL) {
        fprintf(stderr, error_msg);
        exit(1);
    }

    char *buffer = NULL;
    size_t line_length = 0, cmd_len;
    
    char *cmd = token_list[0].val;
    while((cmd_len = getline(&buffer, &line_length, file_ptr)) != EOF) {
        //Skip empty lines
        if(cmd_len == 1)
            continue;
        
        buffer[cmd_len - 1] = '\0';

        char *program_path = buffer;
        strcat(program_path, "/");
        strcat(program_path, cmd);

        if(access(program_path, X_OK) == 0) 
            return program_path;
    }

    return "";
}

void execute_command(char *program_path, TOKEN *token_list, size_t token_list_length) {        
    pid_t pid = fork();

    if(pid < 0) {
        fprintf(stderr, error_msg);
        return;
    } 
    //Child Process
    else if(pid == 0) {
        char **arguments = (char **)malloc(sizeof(char *)*(token_list_length + 1));
        
        for(int i = 0; i < token_list_length; i++)
            arguments[i] = strdup(token_list[i].val);
        
        arguments[token_list_length] = NULL;

        //TODO: Add error handling
        execv(program_path, arguments);

        free(arguments);
        exit(0);
    }
    //Parent Process
    else {
        int status;

        waitpid(pid, &status, 0);

        //TODO: Add Error Handling
    }
    
    return;
}