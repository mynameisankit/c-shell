#ifndef COMMANDS_H
#define COMMANDS_H

#include<token.h>
#include<stddef.h>

char* check_cmd(TOKEN *token_list);

void execute_command(char *program_path, TOKEN *token_list, size_t token_list_length);

#endif