#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "command.h"

#define DELIMS " "
#define IN_REDIRECT "<"
#define OUT_REDIRECT ">"
#define BACKGROUND_COMMAND "&"
#define ERROR_T 1
#define ERROR_F 0

struct command initCommand() {
    struct command cmd;
    memset(cmd.params, 0, sizeof(cmd.params));
    cmd.inFile = NULL;
    cmd.outFile = NULL;

    return cmd;
}

//1. "Error: missing command"
//2. "Error: mislocated output/input redirection"
//3. "Error: no output/input file"
//4. "Error: mislocated background sign"
/*
 * checks if there is an input redirection.
 * returns 1 if there was, 0 if not, and -1 if there was an error.
 */
int checkInRedirect(struct command *cmdPtr, char **inputPtr, int first) {
    if(strcmp(*inputPtr, IN_REDIRECT) == 0) {
        if(first == 1) {
            *inputPtr = strtok(NULL, DELIMS);
            if(*inputPtr == NULL) {
                fprintf(stderr, "Error: no input file\n");
                return -1;
            }
            cmdPtr->inFile = strdup(*inputPtr);
            return 1;
        }
        else {
            fprintf(stderr, "Error: mislocated input redirection\n");
            return -1;
        }
    }
    return 0;
}

/*
 * checks if there is an input redirection.
 * returns 1 if there was, 0 if not, and -1 if there was an error.
 */
int checkOutRedirect(struct command *cmdPtr, char **inputPtr, int last) {
    if(strcmp(*inputPtr, OUT_REDIRECT) == 0) {
        if(last == 1) {
            *inputPtr = strtok(NULL, DELIMS);
            if(*inputPtr == NULL) {
                fprintf(stderr, "Error: no output file\n");
                return -1;
            }
            cmdPtr->outFile = strdup(*inputPtr);
            return 1;
        }
        else {
            fprintf(stderr, "Error: mislocated output redirection\n");
            return -1;
        }
    }
    return 0;
}

struct command constructCommand(char *cmdStr, int first, int last) {

    struct command cmd = initCommand();
    char *cmdStrCpy = strdup(cmdStr);
    char *param = strtok(cmdStrCpy, DELIMS);
    int currParam = 0;
    int inRedirected;
    int outRedirected;
    cmd.errored = ERROR_F;
    cmd.first = first;
    cmd.last = last;

    // Command-string level error
    if(strcmp(param,IN_REDIRECT) == 1 || strcmp(param, OUT_REDIRECT) == 1 || strcmp(param, BACKGROUND_COMMAND) == 1) {
        fprintf(stderr, "Error: missing command\n");
        cmd.errored = ERROR_T;
        return cmd;
    }

    // Split input and place each argument (function and params) into struct
    // Struct: [function, param1, param2, ... paramX, NULL]
    while(param != NULL){

        // Command-string level errors
        if(strcmp(param, BACKGROUND_COMMAND) == 1) {
            fprintf(stderr, "Error: mislocated background sign\n");
            cmd.errored = ERROR_T;
            return cmd;
        }
        if(currParam == 16) {
            fprintf(stderr, "Error: too many process arguments\n");
            cmd.errored = ERROR_T;
            return cmd;
        }

        // Input Redirection
        inRedirected = checkInRedirect(&cmd, &param, first);
        if(inRedirected == -1) {
            cmd.errored = ERROR_T;
            return cmd;
        }
        if(inRedirected == 1) {
            param = strtok(NULL, DELIMS);
            continue;
        }

        // Output Redirection
        outRedirected = checkOutRedirect(&cmd, &param, last);
        if(outRedirected == -1) {
            cmd.errored = ERROR_T;
            return cmd;
        }
        if(outRedirected == 1) {
            param = strtok(NULL, DELIMS);
            continue;
        }

        cmd.params[currParam] = strdup(param);
        param = strtok(NULL, DELIMS);
        currParam++;
    }

    return cmd;
}

// Returns the PID of the child process.
// Returns -1 if there was an error.
int runCommand(struct command cmd, int* prevPipe, int* currPipe){

    // Fork
    pid_t pid;
    pid = fork();

    // Child Process
    if(pid == 0) {

        // Setup IO redirects/Piping
        if(cmd.inFile != NULL) {
            int inFile = open(cmd.inFile, O_RDONLY);
            dup2(inFile, STDIN_FILENO);
            close(inFile);
        }
        if(cmd.outFile != NULL) {
            int outFile = open(cmd.outFile, O_WRONLY | O_CREAT | O_TRUNC, 0666);
            dup2(outFile, STDOUT_FILENO);
            close(outFile);
        }

        // Connect Pipes
        if(cmd.first == 0){
            // If not the first command
            dup2(prevPipe[0], STDIN_FILENO);
            close(prevPipe[1]);
        }
        if(cmd.last == 0){
            // If not the last command
            dup2(currPipe[1], STDOUT_FILENO);
            close(prevPipe[0]);
        }


        // Run child command
        execvp(cmd.params[0], cmd.params);
        perror("execvp");
        exit(1);



    } if(pid > 0) {
        // Parent

    } else {
        // Error
        perror("fork");
        return -1;
    }
}