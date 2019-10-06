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

struct command constructInnerCommand(char *input) {
    struct command cmd = initCommand();

    char *inCopy = strdup(input);

    // Split input and place each argument (function and params) into struct
    // Struct: [function, param1, param2, ... paramX, NULL]
    int currParam = 0;
    char *ptr = strtok(inCopy, DELIMS);
    while(ptr != NULL){
        if(strcmp(ptr, IN_REDIRECT) == 0) {
            fprintf(stderr, "Error: mislocated input redirection");
            cmd.errored = ERROR_T;
            return cmd;
        }
        if(strcmp(ptr, OUT_REDIRECT) == 0) {
            fprintf(stderr, "Error: mislocated output redirection");
            cmd.errored = ERROR_T;
            return cmd;
        }
        else{
            cmd.params[currParam] = strdup(ptr);
            ptr = strtok(NULL, DELIMS);
            currParam++;
        }
    }
    cmd.params[currParam + 1] = NULL;

    cmd.errored = ERROR_F;
    return cmd;
}

struct command constructFirstCommand(char *input){
    struct command cmd = initCommand();

    char *inCopy = strdup(input);

    // Split input and place each argument (function and params) into struct
    // Struct: [function, param1, param2, ... paramX, NULL]
    int currParam = 0;
    char *ptr = strtok(inCopy, DELIMS);
    while(ptr != NULL){
        if(strcmp(ptr, IN_REDIRECT) == 0) {
            ptr = strtok(NULL, DELIMS);
            cmd.inFile = strdup(ptr);
        }
        if(strcmp(ptr, OUT_REDIRECT) == 0) {
            fprintf(stderr, "Error: mislocated output redirection");
            cmd.errored = ERROR_T;
            return cmd;
        }
        else {
            cmd.params[currParam] = strdup(ptr);
            ptr = strtok(NULL, DELIMS);
            currParam++;
        }
    }
    cmd.params[currParam + 1] = NULL;

    cmd.errored = ERROR_F;
    return cmd;
}

struct command constructLastCommand(char *input) {
    struct command cmd = initCommand();

    char *inCopy = strdup(input);

    // Split input and place each argument (function and params) into struct
    // Struct: [function, param1, param2, ... paramX, NULL]
    int currParam = 0;
    char *ptr = strtok(inCopy, DELIMS);
    while(ptr != NULL){
        if(strcmp(ptr, IN_REDIRECT) == 0) {
            fprintf(stderr, "Error: mislocated input redirection");
            cmd.errored = ERROR_T;
            return cmd;
        }
        if(strcmp(ptr, OUT_REDIRECT) == 0) {
            ptr = strtok(NULL, DELIMS);
            cmd.outFile = strdup(ptr);
        }
        else{
            cmd.params[currParam] = strdup(ptr);
            ptr = strtok(NULL, DELIMS);
            currParam++;
        }
    }
    cmd.params[currParam + 1] = NULL;

    cmd.errored = ERROR_F;
    return cmd;
}

struct command constructOnlyCommand(char *input) {
    struct command cmd = initCommand();

    char *inCopy = strdup(input);

    // Split input and place each argument (function and params) into struct
    // Struct: [function, param1, param2, ... paramX, NULL]
    int currParam = 0;
    char *ptr = strtok(inCopy, DELIMS);
    while(ptr != NULL) {
        if (strcmp(ptr, IN_REDIRECT) == 0) {
            ptr = strtok(NULL, DELIMS);
            cmd.inFile = strdup(ptr);
        }
        if (strcmp(ptr, OUT_REDIRECT) == 0) {
            ptr = strtok(NULL, DELIMS);
            cmd.outFile = strdup(ptr);
        } else {
            cmd.params[currParam] = strdup(ptr);
            ptr = strtok(NULL, DELIMS);
            currParam++;
        }
    }
    cmd.params[currParam + 1] = NULL;

    cmd.errored = ERROR_F;
    return cmd;
}