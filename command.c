#include <stdio.h>
#include <string.h>

#include "command.h"

#define DELIMS " "

struct command constructCommand(char *input) {
    struct command cmd;

    char *inCopy = strdup(input);

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