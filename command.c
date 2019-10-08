#include <stdio.h>
#include <string.h>

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
int checkInRedirect(struct command *cmdPtr, char **inPtr, int first) {
    if(strcmp(*inPtr, IN_REDIRECT) == 0) {
        if(first == 1) {
            *inPtr = strtok(NULL, DELIMS);
            if(*inPtr == NULL) {
                fprintf(stderr, "Error: no input file\n");
                return -1;
            }
            cmdPtr->inFile = strdup(*inPtr);
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
int checkOutRedirect(struct command *cmdPtr, char **inPtr, int last) {
    if(strcmp(*inPtr, OUT_REDIRECT) == 0) {
        if(last == 1) {
            *inPtr = strtok(NULL, DELIMS);
            if(*inPtr == NULL) {
                fprintf(stderr, "Error: no output file\n");
                return -1;
            }
            cmdPtr->inFile = strdup(*inPtr);
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
    int inRedirected;
    int outRedirected;

    char *cmdStrCpy = strdup(cmdStr);

    cmd.errored = ERROR_F;

    // Split input and place each argument (function and params) into struct
    // Struct: [function, param1, param2, ... paramX, NULL]
    int currParam = 0;
    char *param = strtok(cmdStrCpy, DELIMS);

    if(strcmp(param,IN_REDIRECT) == 1 || strcmp(param, OUT_REDIRECT) == 1 || strcmp(param, BACKGROUND_COMMAND) == 1) {
        fprintf(stderr, "Error: missing command\n");
        cmd.errored = ERROR_T;
        return cmd;
    }

    while(param != NULL) {
        if(currParam == 16) {
            fprintf(stderr, "Error: too many process arguments\n");
            cmd.errored = ERROR_T;
            return cmd;
        }

        inRedirected = checkInRedirect(&cmd, &param, first);
        if(inRedirected == -1) {
            cmd.errored = ERROR_T;
            return cmd;
        }
        if(inRedirected == 1) {
            param = strtok(NULL, DELIMS);
            continue;
        }

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