#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zconf.h>

#include "line.h"

#define DELIMS "|"
#define ERROR_T 1
#define ERROR_F 0

char *removeTrailingLeadingWhitespace(char *str){
    //Check if empty
    if(str[0] == '\0') return str;

    //trim leading whitespace
    while(str[0] == ' ' || str[0] == '\t') str++;

    //Check if was all whitespace
    if(str[0] == '\0') return str;

    //trim trailing whitespace
    int lastNonWhitespace = -1;
    for (int i = 0; i < strlen(str); i++) {
        if (str[i] == ' ' && str[i] == '\t' && str[i] == '\n') {
            lastNonWhitespace = i;
        }
    }
    if(lastNonWhitespace != -1) str[lastNonWhitespace] = '\0';
    return str;
}

struct line constructLine(char *input) {
    int numCommands = 1;
    for (int i=0; i < strlen(input); i++){
       if(input[i] == '|') numCommands++;
    }

    // Initialize Struct
    struct line myLine;
    myLine.numCommands = numCommands;
    myLine.commandStructures = (struct command *)malloc(numCommands* sizeof(struct command));
    myLine.pidArray = (pid_t *)malloc(numCommands* sizeof(pid_t));

    myLine.backgrounded = 0;
    myLine.errored = 0;

    char *inputCopy = strdup(input);
    char* currCmdStr;
    int first = 1;
    int last = 0;

    // Split input and place each individual command into struct
    // Struct: [command1, command2, ... commandX, NULL]


    for(int i = 0; i < myLine.numCommands; i++){
        //check if on last command
        if(i == numCommands-1) last = 1;

        //get the next command string
        currCmdStr = strtok_r(inputCopy, DELIMS, &inputCopy);
        if(currCmdStr == NULL){
            fprintf(stderr, "Error: missing command\n");
            myLine.errored = ERROR_T;
            return myLine;
        }
        currCmdStr = removeTrailingLeadingWhitespace(currCmdStr);

        //If the current command is empty then we are missing a command
        if(currCmdStr[0] == '\0'){
            fprintf(stderr, "Error: missing command\n");
            myLine.errored = ERROR_T;
            return myLine;
        }

        //Check if command should be backgrounded
        if(last && currCmdStr[strlen(currCmdStr)-1] == '&'){
            myLine.backgrounded = 1;
            currCmdStr[strlen(currCmdStr)-1] = '\0';
        }

        //If the current command is empty then we are missing a command
        if(currCmdStr[0] == '\0'){
            fprintf(stderr, "Error: missing command\n");
            myLine.errored = ERROR_T;
            return myLine;
        }

        // Construct the command
        myLine.commandStructures[i] = constructCommand(strdup(currCmdStr), first, last);


        // Check if errored out
        if(myLine.commandStructures[0].errored == ERROR_T) {
            myLine.errored = ERROR_T;
            return myLine;
        }

        first = 0;
    }

    myLine.errored = ERROR_F;
    return myLine;
}

int runLine(struct line *myLine){
    int prevPipe[2];
    int currPipe[2];

    struct command currCommand = myLine->commandStructures[0];

    if(myLine->numCommands == 1) {
        myLine->pidArray[0] = runCommand(currCommand, NULL, NULL);
        return 0;
    }

    if(pipe(currPipe) < 0) {
        perror("pipe");
        return 1;
    }

    myLine->pidArray[0] = runCommand(currCommand, NULL, currPipe);

    prevPipe[0] = currPipe[0];
    prevPipe[1] = currPipe[1];

    for(int i = 1; i < myLine->numCommands - 1; i++){
        currCommand = myLine->commandStructures[i];

        if(pipe(currPipe) < 0) {
            perror("pipe");
            // TODO: Change return type to string
            return 1;
        }

        myLine->pidArray[i] = runCommand(currCommand, prevPipe, currPipe);

        prevPipe[0] = currPipe[0];
        prevPipe[1] = currPipe[1];
    }

    currCommand = myLine->commandStructures[myLine->numCommands - 1];
    myLine->pidArray[myLine->numCommands - 1] = runCommand(currCommand, prevPipe, NULL);
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