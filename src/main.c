#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "include/builtins.h"
#include "include/parser.h"
#include "include/executor.h"
#include "include/utils.h"

static int is_builtin_command(const char *cmd) {
    if (!cmd) return 0;
    return strcmp(cmd, "exit") == 0 ||
           strcmp(cmd, "cd") == 0 ||
           strcmp(cmd, "pwd") == 0 ||
           strcmp(cmd, "echos") == 0 ||
           strcmp(cmd, "export") == 0 ||
           strcmp(cmd, "unset") == 0 ||
           strcmp(cmd, "env") == 0;
}

static void execute_builtin(char **args) {
    if (strcmp(args[0], "exit") == 0) exit(0);
    if (strcmp(args[0], "cd") == 0) cd(args);
    else if (strcmp(args[0], "pwd") == 0) pwd(args);
    else if (strcmp(args[0], "echos") == 0) echo(args);
    else if (strcmp(args[0], "export") == 0) export(args);
    else if (strcmp(args[0], "unset") == 0) unset(args);
    else if (strcmp(args[0], "env") == 0) env();
}

static void restore_builtin_redirection(int saved_fds[2]) {
    if (saved_fds[0] != -1) {
        dup2(saved_fds[0], STDIN_FILENO);
        close(saved_fds[0]);
        saved_fds[0] = -1;
    }
    if (saved_fds[1] != -1) {
        dup2(saved_fds[1], STDOUT_FILENO);
        close(saved_fds[1]);
        saved_fds[1] = -1;
    }
}

static int apply_builtin_redirection(const redirect *redir, int saved_fds[2]) {
    saved_fds[0] = -1;
    saved_fds[1] = -1;

    if (!redir->input_file && !redir->output_file) return 0;

    if (redir->input_file) {
        int backup = dup(STDIN_FILENO);
        if (backup < 0) {
            perror("dup");
            return -1;
        }
        saved_fds[0] = backup;

        int in_fd = open(redir->input_file, O_RDONLY);
        if (in_fd < 0) {
            perror("open input");
            restore_builtin_redirection(saved_fds);
            return -1;
        }
        if (dup2(in_fd, STDIN_FILENO) < 0) {
            perror("dup2 input");
            close(in_fd);
            restore_builtin_redirection(saved_fds);
            return -1;
        }
        close(in_fd);
    }

    if (redir->output_file) {
        int backup = dup(STDOUT_FILENO);
        if (backup < 0) {
            perror("dup");
            restore_builtin_redirection(saved_fds);
            return -1;
        }
        saved_fds[1] = backup;

        const int flags = O_WRONLY | O_CREAT | (redir->append ? O_APPEND : O_TRUNC);
        int out_fd = open(redir->output_file, flags, 0644);
        if (out_fd < 0) {
            perror("open output");
            restore_builtin_redirection(saved_fds);
            return -1;
        }
        if (dup2(out_fd, STDOUT_FILENO) < 0) {
            perror("dup2 output");
            close(out_fd);
            restore_builtin_redirection(saved_fds);
            return -1;
        }
        close(out_fd);
    }

    return 0;
}


int main() {
    while (1) {
        char *input = read_input();
        if (!input) continue;

        char **args = tokenize_input(input);
        const int background = is_background(args);
        const redirect redirect_result = is_redirect(args);

        if (args[0] != NULL) {
            if (is_builtin_command(args[0])) {
                int saved_fds[2];
                if (apply_builtin_redirection(&redirect_result, saved_fds) == 0) {
                    execute_builtin(args);
                    restore_builtin_redirection(saved_fds);
                }
            } else {
                launch_process(args, background, redirect_result);
            }
        }

        free(input);
        free(args);
    }
}
