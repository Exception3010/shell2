//
// Created by hazem on 10/2/25.
//

#ifndef SHELL_UTILS_H
#define SHELL_UTILS_H

#endif //SHELL_UTILS_H

#include <fcntl.h>

char *read_input(void);

int is_background(char **args);

typedef  struct {
    int redirect;
    char *input_file;
    char *output_file;
    int append;
} redirect;

redirect is_redirect(char **args);

void handle_redirection(const char *input_file, const char *output_file, const int append);