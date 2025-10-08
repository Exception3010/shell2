//
// Created by hazem on 10/2/25.
//

#ifndef SHELL_EXECUTOR_H
#define SHELL_EXECUTOR_H

#endif //SHELL_EXECUTOR_H

#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "include/utils.h"



int launch_process(char **args, int background, redirect redirect);
