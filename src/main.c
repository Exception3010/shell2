#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/builtins.h"
#include "include/parser.h"
#include "include/executor.h"


int main() {
    while (1) {
        char *input = read_input();
        if (!input) continue;

        char **args = tokenize_input(input);
        const int background = is_background(args);
        const redirect redirect_result = is_redirect(args);

        if (args[0] != NULL) {
            if (strcmp(args[0], "exit") == 0) exit(0);
            if (strcmp(args[0], "cd") == 0) cd(args);
            else if (strcmp(args[0], "pwd") == 0) pwd(args);
            else if (strcmp(args[0], "echos") == 0) echo(args);
            else if (strcmp(args[0], "export") == 0) export(args);
            else if (strcmp(args[0], "unset") == 0) unset(args);
            else if (strcmp(args[0], "env") == 0) env();
            else launch_process(args, background, redirect_result);
        }

        free(input);
        free(args);
    }
}
