#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "command.h"
#include "line.h"

#define MAX_INPUT_LEN 512
#define ERROR_T "1"
#define ERROR_F "0"

void simpleShell();

int main(int argc, char *argv[])
{
    simpleShell();
    return EXIT_SUCCESS;
}

void simpleShell() {
    // User Input
    char *input = (char*) malloc(MAX_INPUT_LEN*sizeof(char));
    size_t len = MAX_INPUT_LEN;

    // Data Structures
    struct command cmd;

    // Other Variables
    pid_t pid;
    int status;

    // Get User Input
    fprintf(stdout, "sshell$ ");
    getline(&input, &len, stdin);
    strtok(input, "\n");

    cmd = constructCommand(input, 0, 1);

    //check for error and finish current loop if errored
    int parsingError = cmd.errored;
    if(parsingError == 1) {
        fprintf(stderr, "+ completed '%s' [1]\n", input);
        simpleShell();
    }

    // Start Process
    pid = fork();
    if(pid == 0) {
        // Child Process
        execvp(cmd.params[0], cmd.params);
        perror("execvp");
        exit(1);
    } if(pid > 0) {
        // Parent Process
        waitpid(-1, &status, 0);
        fprintf(stderr, "+ completed '%s' [%d]\n", input, cmd.errored);
        simpleShell();
    } else {
        perror("fork");
        simpleShell();
    }
}

