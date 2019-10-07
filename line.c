#include <stdio.h>
#include <string.h>

#include "line.h"

#define DELIMS "|"
#define ERROR_T "1"
#define ERROR_F "0"

struct line constructLine(char *input) {

    // Initialize Struct
    struct line myLine;
    memset(myLine.commandStrings, 0, sizeof(myLine.commandStrings));
    memset(myLine.commandStructures, 0, sizeof(myLine.commandStructures));

    char *inCopy = strdup(input);
    char* token;
    char* nextToken;
    char* rest = inCopy;

    // Split input and place each individual command into struct
    // Struct: [command1, command2, ... commandX, NULL]
    int currCommand = 0;
    token = strtok_r(rest, DELIMS, &rest);
    nextToken = strtok_r(rest, DELIMS, &rest);
    while (token != NULL) {
        // Create new command with current token
        myLine.commandStrings[currCommand] = strdup(token);
        // Determine type of command
        if(currCommand == 0 && nextToken == NULL) {
            myLine.commandStructures[currCommand] = constructOnlyCommand(strdup(token));
        }
        else if(currCommand == 0) {
            myLine.commandStructures[currCommand] = constructFirstCommand(strdup(token));
        }
        else if(nextToken == NULL) {
            myLine.commandStructures[currCommand] = constructLastCommand(strdup(token));
        }
        else {
            myLine.commandStructures[currCommand] = constructInnerCommand(strdup(token));
        }
        //Check if errored out
        if(myLine.commandStructures[currCommand].errored == ERROR_T) {
            myLine.errored = ERROR_T;
            return myLine;
        }
        // Update token
        token = nextToken;
        nextToken = strtok_r(rest, DELIMS, &rest);
        currCommand++;
    }
    myLine.commandStrings[currCommand + 1] = NULL;

    myLine.errored = ERROR_F;
    return myLine;

}

