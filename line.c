#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zconf.h>
#include <wait.h>

#include "line.h"

#define DELIMS "|"
#define ERROR_T 1
#define ERROR_F 0

int lastNonWhite(char *str) {
    int lastNonWhitespace = -1;
    for (int i = 0; i < strlen(str); i++) {
        if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n') {
            lastNonWhitespace = i;
        }
    }
    return lastNonWhitespace;
}

struct line constructLine(char *input) {

    // Initialize Struct
    struct line myLine;
    memset(myLine.commandStrings, 0, sizeof(myLine.commandStrings));
    memset(myLine.commandStructures, 0, sizeof(myLine.commandStructures));
    memset(myLine.pidArray, 0, sizeof(myLine.pidArray));
    memset(myLine.statusArray, 0, sizeof(myLine.statusArray));

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

        //Check to see if the command should be backgrounded
        if(last == 1) {
            int posLastNonWhite = lastNonWhite(token);
            if(posLastNonWhite != -1 && token[posLastNonWhite] == '&'){
                myLine.backgrounded = 1;
                token[posLastNonWhite] = '\0';
            }
        }

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
    myLine.numCommands = currCommand;

    myLine.commandStrings[currCommand + 1] = NULL;

    myLine.errored = ERROR_F;
    return myLine;

}

int runLine(struct line myLine){
    int prevPipe[2];
    int currPipe[2];

    // Keeps track of pid and status arrays
    int currentCommandIndex = 0;

    struct command currCommand = myLine.commandStructures[0];

    if(myLine.numCommands == 1) {
        // TODO: Change return type to string
        myLine.pidArray[currentCommandIndex] = runCommand(currCommand, NULL, NULL);
        return 0;
    }

    if(pipe(currPipe) < 0) {
        perror("pipe");
        // TODO: Change return type to string
        return -1;
    }

    myLine.pidArray[currentCommandIndex] = runCommand(currCommand, NULL, currPipe);
    currentCommandIndex++;

    prevPipe[0] = currPipe[0];
    prevPipe[1] = currPipe[1];

    for(int i = 1; i < myLine.numCommands - 1; i++){
        currCommand = myLine.commandStructures[i];

        if(pipe(currPipe) < 0) {
            perror("pipe");
            // TODO: Change return type to string
            return -1;
        }

        myLine.pidArray[currentCommandIndex] = runCommand(currCommand, prevPipe, currPipe);
        currentCommandIndex++;

        prevPipe[0] = currPipe[0];
        prevPipe[1] = currPipe[1];
    }

    currCommand = myLine.commandStructures[myLine.numCommands - 1];

    if(pipe(currPipe) < 0) {
        perror("pipe");
        // TODO: Change return type to string
        return -1;
    }

    myLine.pidArray[currentCommandIndex] = runCommand(currCommand, prevPipe, NULL);

    // TODO: Change return type to string
    return 0;

}

char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}