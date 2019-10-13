#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zconf.h>

#include "line.h"

#define DELIMS "|"
#define ERROR_T 1
#define ERROR_F 0

struct line constructLine(char *input) {

    // Initialize Struct
    struct line myLine;
    memset(myLine.commandStrings, 0, sizeof(myLine.commandStrings));
    memset(myLine.commandStructures, 0, sizeof(myLine.commandStructures));
    memset(myLine.pipeArray, 0, sizeof(myLine.pipeArray));

    char *inCopy = strdup(input);
    char* token;
    char* nextToken;
    char* rest = inCopy;
    int first;
    int last;

    // Split input and place each individual command into struct
    // Struct: [command1, command2, ... commandX, NULL]
    int currCommand = 0;
    token = strtok_r(rest, DELIMS, &rest);
    nextToken = strtok_r(rest, DELIMS, &rest);
    while (token != NULL) {
        // Create new command with current token
        myLine.commandStrings[currCommand] = strdup(token);

        // Check if first or last command in the line
        if(currCommand == 0) first = 1;
        else first = 0;
        if(nextToken == NULL) last = 1;
        else last = 0;

        // Construct the command
        myLine.commandStructures[currCommand] = constructCommand(strdup(token), first, last);

        // Check if errored out
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

void runLine(struct line myLine){

    // Iterate through each command in line
    struct command* currentCommand = &(myLine.commandStructures[0]);
    struct command* endPtr = &(myLine.commandStructures[0]) + 10;
    int prevOutIndex = 0;
    int prevInIndex = 1;
    int nextOutIndex = 2;
    int nextInIndex = 3;
    while ( currentCommand < endPtr && currentCommand->params[0] != NULL){

        runCommand(*currentCommand, &myLine.pipeArray[prevOutIndex], &myLine.pipeArray[prevInIndex],
                   &myLine.pipeArray[nextOutIndex], &myLine.pipeArray[nextInIndex]);

        // Increment
        currentCommand++;
        prevOutIndex ++;
        prevInIndex++;
        nextOutIndex++;
        nextInIndex++;
    }

    // Test: try reading from last pipe
    char inbuf[10];
    memset(inbuf, 0, sizeof(inbuf));
    read(*(myLine.pipeArray + 3), inbuf, 10);

    char* test = inbuf;
    
}