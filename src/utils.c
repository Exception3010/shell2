//
// Created by hazem on 10/2/25.
//

#include "../include/utils.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *read_input(void) {
    char cwd[1024];
    if (!getcwd(cwd, 1024)) return NULL;

    printf("%s# ", cwd);

    char *buffer = NULL;
    size_t buf_size = 0;
    ssize_t n_read = 0;

    n_read = getline(&buffer, &buf_size, stdin);

    if (n_read == -1) {
        free(buffer);
        return NULL;
    }

    if (strlen(buffer) == 1) {
        free(buffer);
        return NULL;
    }

    buffer[strcspn(buffer, "\n")] = '\0';

    return buffer;
}

int is_background(char **args) {
    int background = 0;
    int n = 0;
    while (args[n]) n++;
    if (n > 0 && strcmp(args[n - 1], "&") == 0) {
        background = 1;
        args[n - 1] = NULL;
    }
    return background;
}

redirect is_redirect(char **args) {
    redirect redirect_instance = {0, NULL, NULL, 0};
    for (int i = 0; args[i]; i++) {
        if (strcmp(args[i], "<") == 0 && args[i+1]) {
            redirect_instance.input_file = args[i+1];
            args[i] = NULL; // terminate command before '<'
            break;
        }
        if (strcmp(args[i], ">") == 0 && args[i+1]) {
            redirect_instance.output_file = args[i+1];
            redirect_instance.append = 0;
            args[i] = NULL;
            break;
        }
        if (strcmp(args[i], ">>") == 0 && args[i+1]) {
            redirect_instance.output_file = args[i+1];
            redirect_instance.append = 1;
            args[i] = NULL;
            break;
        }
    }
    return redirect_instance;
}

void handle_redirection(const char *input_file, const char *output_file, const int append) {
    if (input_file) {
        int in = open(input_file, O_RDONLY);
        if (in < 0) { perror("open input"); exit(1); }
        dup2(in, STDIN_FILENO);
        close(in);
    }

    if (output_file) {
        const int flags = O_WRONLY | O_CREAT | (append ? O_APPEND : O_TRUNC);
        const int out = open(output_file, flags, 0644);
        if (out < 0) { perror("open output"); exit(1); }
        dup2(out, STDOUT_FILENO);
        close(out);
    }
}