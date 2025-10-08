//
// Created by hazem on 10/2/25.
//

#include "../include/parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **tokenize_input(char *line) {
    int buf_size = TOK_BUF_SIZE;
    int position = 0;
    char **tokens = malloc(buf_size * sizeof(char *));

    if (!tokens) {
        fprintf(stderr, "allocation error\n");
        exit(EXIT_FAILURE);
    }

    char *token = strtok(line, TOK_DELIM);
    while (token != NULL) {
        tokens[position++] = token;

        if (position >= buf_size) {
            buf_size += TOK_BUF_SIZE;
            char **tmp = realloc(tokens, buf_size * sizeof(char *));
            if (!tmp) {
                free(tokens);
                fprintf(stderr, "allocation error\n");
                exit(EXIT_FAILURE);
            }
            tokens = tmp;
        }

        token = strtok(NULL, TOK_DELIM);
    }
    tokens[position] = NULL;
    return tokens;
}
