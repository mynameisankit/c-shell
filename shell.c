#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<stdbool.h>
#include<sys/wait.h>
#include<token.h>

int main(int argc, char* argv[]) {
    if(argc > 2) {
        //Print an error
        fprintf(stderr, "More number of arguments than required\n");
        exit(1);
    }
    
    char *buffer = NULL;
    size_t line_length = 0;

    TOKEN *token_list = NULL;
    size_t token_list_length = 0, cmd_len;
    int tokenizer_status;

    //Interactive Mode
    if(argc == 1) {
        bool flag = false;
        
        while(!flag) {
            fprintf(stdout, "csh> ");
            getline(&buffer, &line_length, stdin);

            if((cmd_len = strlen(buffer)) == 1)
                continue;

            buffer[cmd_len - 1] = '\0';

            tokenizer_status = tokenize(buffer, &token_list, &token_list_length);

            free(token_list);
        }
    }
    //Batch Mode
    else {
        char *batch_file = strdup(argv[1]);
        FILE* file_ptr = NULL;

        if((file_ptr = fopen(batch_file, "r")) == NULL) {
            fprintf(stderr, "Cannot open file\n");
            exit(1);
        }

        while(getline(&buffer, &line_length, file_ptr) != EOF) {
            //Skip empty lines
            if((cmd_len = strlen(buffer)) == 1)
                continue;

            buffer[cmd_len - 1] = '\0';

            tokenizer_status = tokenize(strdup(buffer), &token_list, &token_list_length);

            if(tokenizer_status == false) {
                fprintf(stderr, "The command \"%s\" is wrong\n", buffer);
                exit(1);
            }

            free(token_list);
        }

        //Close the file
        fclose(file_ptr);
    }

    free(buffer);

    return 0;
}