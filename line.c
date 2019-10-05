#include <stdio.h>
#include <string.h>

#include "line.h"

#define DELIMS "|"

struct line constructLine(char *input) {
    struct line myLine;

    char *inCopy = strdup(input);

    // Split input and place each individual command into struct
    // Struct: [command1, command2, ... commandX, NULL]
    int currCommand = 0;
    char *ptr = strtok(inCopy, DELIMS);
    while (ptr != NULL) {
        myLine.individualCommands[currCommand] = strdup(ptr);
        ptr = strtok(NULL, DELIMS);
        currCommand++;
    }
    myLine.individualCommands[currCommand + 1] = NULL;

    return myLine;

}