//
// Created by hazem on 10/2/25.
//

#ifndef SHELL_BUILTINS_H
#define SHELL_BUILTINS_H

#endif //SHELL_BUILTINS_H

extern char **environ;

void cd(char **args);

void pwd(char **args);

void echo(char **args);

void export(char **args);

void unset(char **args);

int env();
