cmake_minimum_required(VERSION 4.0)
project(shell C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(shell
        src/main.c
        src/parser.c
        include/parser.h
        src/executor.c
        include/executor.h
        src/builtins.c
        include/builtins.h
        src/jobs.c
        include/jobs.h
        src/utils.c
        include/utils.h)
