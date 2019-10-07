#include <stdio.h>
#include <string.h>

#include "command.h"

#define DELIMS " "
#define IN_REDIRECT "<"
#define OUT_REDIRECT ">"
#define ERROR_T "1"
#define ERROR_F "0"

struct command initCommand() {
    struct command cmd;
    memset(cmd.params, 0, sizeof(cmd.params));
    cmd.inFile = NULL;
    cmd.outFile = NULL;

    return cmd;
}

//checks if the current character is an IO redirect and then will print error and set errored == 1 if there shouldn't
// be a redirect or reads in the next token and sets the inFile/outFile to that token.
// Returns 1 if there was a redirect and 0 if there wasn't
int checkIORedirect(struct command *cmdPtr, char **inPtr, int first, int last) {
    if(strcmp(*inPtr, IN_REDIRECT) == 0) {
        if(first == 1) {
            *inPtr = strtok(NULL, DELIMS);
            cmdPtr->inFile = strdup(*inPtr);
        }
        else {
            fprintf(stderr, "Error: mislocated input redirection");
            cmdPtr->errored = ERROR_T;
        }
        return 1;
    }
    if(strcmp(*inPtr, OUT_REDIRECT) == 0) {
        if(last == 1) {
            *inPtr = strtok(NULL, DELIMS);
            cmdPtr->outFile = strdup(*inPtr);
        }
        else {
            fprintf(stderr, "Error: mislocated output redirection");
            cmdPtr->errored = ERROR_T;
        }
        return 1;
    }
    return 0;
}

struct command constructCommand(char *input, int first, int last) {
    struct command cmd = initCommand();
    int redirected;

    char *inCopy = strdup(input);

    cmd.errored = ERROR_F;

    // Split input and place each argument (function and params) into struct
    // Struct: [function, param1, param2, ... paramX, NULL]
    int currParam = 0;
    char *input = strtok(inCopy, DELIMS);
    while(input != NULL) {
//        if(currParam == 16) {
//            cmd.errored = 1;
//            fprintf(stderr, )
//        }
        redirected = checkIORedirect(&cmd, &input, first, last);
        if(cmd.errored == 1) {
            return cmd;
        }
        else if(redirected == 1) {
            continue;
        }
        else {
            cmd.params[currParam] = strdup(input);
            input = strtok(NULL, DELIMS);
            currParam++;
        }
    }
}