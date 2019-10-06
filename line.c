#include <stdio.h>
#include <string.h>

#include "line.h"

#define DELIMS "|"

struct line constructLine(char *input) {

    // Initialize Struct
    struct line myLine;
    memset(myLine.commandStrings, 0, sizeof(myLine.commandStrings));
    memset(myLine.commandStructures, 0, sizeof(myLine.commandStructures));

    char *inCopy = strdup(input);

    // Split input and place each individual command into struct
    // Struct: [command1, command2, ... commandX, NULL]
    int currCommand = 0;
    char *ptr = strtok(inCopy, DELIMS);
    while (ptr != NULL) {
        myLine.commandStrings[currCommand] = strdup(ptr);
        myLine.commandStructures[currCommand] = constructCommand(strdup(ptr));
        ptr = strtok(NULL, DELIMS); // TODO: Problem here. On second iteration of loop, immediately becomes null
        currCommand++;
    }
    myLine.commandStrings[currCommand + 1] = NULL;

    return myLine;

}