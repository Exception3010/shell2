//
// Created by hazem on 10/2/25.
//

#include "../include/executor.h"

int launch_process(char **args, const int background, const redirect redirect) {
    const pid_t pid = fork();
    int status;

    if (pid == 0) {
        if (redirect.input_file) {
            int in = open(redirect.input_file, O_RDONLY);
            if (in < 0) {
                perror("open input");
                exit(1);
            }
            dup2(in, STDIN_FILENO);
            close(in);
        }

        if (redirect.output_file) {
            int flags = O_WRONLY | O_CREAT | (redirect.append ? O_APPEND : O_TRUNC);
            int out = open(redirect.output_file, flags, 0644);
            if (out < 0) {
                perror("open output");
                exit(1);
            }
            dup2(out, STDOUT_FILENO);
            close(out);
        }

        execvp(args[0], args);
        perror("execvp");
        exit(1);
    } else if (pid < 0) {
        perror("fork");
    } else if (!background) {
        waitpid(pid, &status, WUNTRACED);
    } else {
        printf("[background pid %d]\n", pid);
    }
    return 1;
}
