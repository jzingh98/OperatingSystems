cmake_minimum_required(VERSION 3.15)
project(Project1 C)

set(CMAKE_C_STANDARD 99)

add_executable(Project1 sshell.c command.h command.c )