#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "processes.h"
#include "line.h"

#define COMMANDDELIMS "|"
#define MAX_INPUT_LEN 512
#define ERROR_T "1"
#define ERROR_F "0"

void simpleShell();


int main(int argc, char *argv[])
{
    simpleShell();
    printf("done");
    return 1;

}

void simpleShell() {
    // User Input
    char *input = NULL;
    size_t len = 0;

    // Data Structures
    struct line myLine;
    struct process_node *unfinishedProcesses = NULL;

    // Other Variables
    int status;

    //Prompt user for input
    fprintf(stdout, "sshell$ ");

    while(getline(&input, &len, stdin)>=0){
        strtok(input, "\n");
        if(strcmp(input, "exit") == 0) {
            if(unfinishedProcesses == NULL) {
                fprintf(stderr, "Bye...");
                return;
            }
            else{
                fprintf(stderr,"Error: active jobs still running\n");
                fprintf(stderr,"+ completed '%s' [1]\n", input);
                fprintf(stdout, "sshell$ ");
                free(input);
                input = NULL;
                len = 0;
                continue;
            }
        }

        // Construct Line
        myLine = constructLine(input);
        if(myLine.errored == 1) {
            fprintf(stdout, "sshell$ ");
            free(input);
            input = NULL;
            len = 0;
            continue;
        }

        // Execute Line
        status = runLine(&myLine);

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
            unfinishedProcesses = printCompletedProcessesBackground(unfinishedProcesses);
            printCompletedProcessesForeground(newProcess);
        }

        //Print the next shell prompt
        fprintf(stdout, "sshell$ ");
        free(input);
        input = NULL;
        len = 0;
    }
    return;
}


/*


echo toto | tr o i


 */

