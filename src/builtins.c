//
// Created by hazem on 10/2/25.
//

#include "../include/builtins.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void cd(char **args) {
    if (args[1] == NULL) {
        char *home = getenv("HOME");
        if (home == NULL) {
            fprintf(stderr, "cd: HOME not set\n");
            return;
        }
        if (chdir(home) != 0) perror("cd");
    } else if (chdir(args[1]) != 0) perror("cd");
}

void pwd(char **args) {
    int logical = 1;
    for (int i = 1; args[i]; ++i) {
        if (strcmp(args[i], "-L") == 0)
            logical = 1;
        else if (strcmp(args[i], "-P") == 0)
            logical = 0;
        else {
            fprintf(stderr, "usage: pwd [-L|-P]\n");
            return;
        }
    }

    if (logical) {
        const char *pwdenv = getenv("PWD");
        if (pwdenv && pwdenv[0] == '/') {
            printf("%s\n", pwdenv);
            return;
        }
    }

    char *cwd = getcwd(NULL, 0);
    if (!cwd) {
        perror("pwd");
        return;
    }

    printf("%s\n", cwd);
    free(cwd);
}

void echo(char **args) {
    int newline = 1;
    int i = 1;

    if (args[i] && strcmp(args[i], "-n") == 0) {
        newline = 0;
        i++;
    }

    for (; args[i]; i++) {
        printf("%s", args[i]);
        if (args[i + 1]) putchar(' ');
    }

    if (newline) putchar('\n');
}

void export(char **args) {
    if (args[1] == NULL) {
        for (int i = 0; environ[i]; i++)
            printf("declare -x %s\n", environ[i]);
        return;
    }

    for (int i = 1; args[i]; i++) {
        char *arg = args[i];
        char *equal = strchr(arg, '=');

        if (equal) {
            *equal = '\0';
            char *name = arg;
            char *value = equal + 1;

            if (setenv(name, value, 1) != 0)
                perror("export");
        } else {
            printf("%s-", arg);
            char *value = getenv(arg);
            if (value == NULL) value = "";
            if (setenv(arg, value, 1) != 0)
                perror("export");
        }
    }
}

void unset(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "unset: missing argument\n");
        return;
    }

    for (int i = 1; args[i]; i++) {
        if (unsetenv(args[i]) != 0) {
            perror("unset");
        }
    }
}

int env() {
    for (char **env = environ; *env; env++)
        printf("%s\n", *env);
    return 1;
}
