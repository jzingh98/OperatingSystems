#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "processes.h"
#include "line.h"

#define COMMANDDELIMS "|"
#define MAX_INPUT_LEN 512
#define ERROR_T "1"
#define ERROR_F "0"

void simpleShell();



int runCdCmd(struct line *myLine);

void runPwdCmd();

int main(int argc, char *argv[])
{
    simpleShell();
    return 0;

}

void simpleShell() {
    // User Input
    char *input = NULL;
    size_t len = 0;

    // Data Structures
    struct line myLine;
    struct process_node *unfinishedProcesses = NULL;

    while(1){
        //Prints the next shell prompt get the input and removes trialing and leading whitespace.
        fprintf(stdout, "sshell$ ");
        getline(&input, &len, stdin);
        if (!isatty(STDIN_FILENO)) {
            printf("%s", input);
            fflush(stdout);
        }
        input = removeTrailingLeadingWhitespace(input);

        if(input[0] == '\0'){
            unfinishedProcesses = printCompletedProcessesBackground(unfinishedProcesses);
            continue;
        }

        // Construct Line
        myLine = constructLine(input);

        if(myLine.errored == 1) {
            unfinishedProcesses = printCompletedProcessesBackground(unfinishedProcesses);
            continue;
        }

        //Check input against builtin commands
        char *firstParam = myLine.commandStructures[0].params[0];
        //Check builtin command which will change the values of finish and cont if those should happen.
        if (strcmp(firstParam, "exit") == 0){
            if(unfinishedProcesses == NULL) {
                fprintf(stderr, "Bye...\n");
                return;
            }
            else{
                fprintf(stderr,"Error: active jobs still running\n");
                fprintf(stderr,"+ completed '%s' [1]\n", input);
                continue;
            }
        }
        if(strcmp(firstParam, "cd") == 0){
            int cdOut = runCdCmd(&myLine);
            unfinishedProcesses = printCompletedProcessesBackground(unfinishedProcesses);
            fprintf(stderr,"+ completed '%s' [%d]\n", input, cdOut);
            continue;
        }
        if(strcmp(firstParam, "pwd") == 0){
            runPwdCmd(&input);
            unfinishedProcesses = printCompletedProcessesBackground(unfinishedProcesses);
            fprintf(stderr,"+ completed '%s' [0]\n", input);
            continue;
        }



        // If its not a builtin command Execute Line
        if(runLine(&myLine) == 1) exit(1);

        //Create new processes node struct
        struct process_node *newProcess = (struct process_node*) malloc(sizeof(struct process_node));
        constructProcess(newProcess, myLine.numCommands, myLine.pidArray, input);

        if(myLine.backgrounded == 1){
            //If its to be done in the background add it to the stack
            unfinishedProcesses = appendProcess(unfinishedProcesses, newProcess);
            //print all completed background commands
            unfinishedProcesses = printCompletedProcessesBackground(unfinishedProcesses);
        }
        else{
            //print all completed background commands
            unfinishedProcesses = printAllCompletedProcesses(newProcess, unfinishedProcesses);
            free(newProcess);
        }

        deconstructLine(&myLine);
    }
    return;
}


void runPwdCmd() {
    char* directory = (char *)malloc(100 * sizeof(char));
    getcwd(directory, 100);
    fprintf(stdout,"%s\n", directory);
}

int runCdCmd(struct line *myLine) {
    if (chdir((*myLine).commandStructures[0].params[1]) == -1) {
        fprintf(stderr, "Error: no such directory\n");
        return 1;
    }
    // Return special pid value
    return 0;
}

