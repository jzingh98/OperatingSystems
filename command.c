#include <stdio.h>
#include <string.h>

#include "command.h"

#define DELIMS " "

struct command constructCommand(char *input) {

    // Initialize Struct
    struct command cmd;
    memset(cmd.params, 0, sizeof(cmd));

    char *inCopy = strdup(input);

    // Split input and place each argument (function and params) into struct
    // Struct: [function, param1, param2, ... paramX, NULL]
    int currParam = 0;
    char *ptr = strtok(inCopy, DELIMS);
    while(ptr != NULL){
        cmd.params[currParam] = strdup(ptr);
        ptr = strtok(NULL, DELIMS);
        currParam++;
    }
    cmd.params[currParam + 1] = NULL;

    return cmd;
}