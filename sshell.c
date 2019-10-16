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


int main(int argc, char *argv[])
{
    simpleShell();
    return 1;

}

void simpleShell() {
    // User Input
    char *input = NULL;
    size_t len = 0;

    // Data Structures
    struct line myLine;
    struct process_node *unfinishedProcesses = NULL;

    //Prompt user for input
    fprintf(stdout, "sshell$ ");

    while(getline(&input, &len, stdin)>=0){
        if (!isatty(STDIN_FILENO)) {
            printf("%s", input);
            fflush(stdout);
        }
        strtok(input, "\n");

        //TODO: check if all spaces

        // Construct Line
        myLine = constructLine(input);

        if(myLine.errored == 1) {
            fprintf(stdout, "sshell$ ");
            free(input);
            input = NULL;
            len = 0;
            continue;
        }

        // Check Built In Commands
        if(strcmp(myLine.commandStructures[0].params[0], "exit") == 0) {
            if(unfinishedProcesses == NULL) {
                fprintf(stderr, "Bye...\n");
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
        // CD
        if (strcmp(myLine.commandStructures[0].params[0], "cd") == 0){
            if (chdir(myLine.commandStructures[0].params[1]) == -1) {
                fprintf(stderr,"Error: no such directory\n");
                fprintf(stderr,"+ completed '%s' [1]\n", input);
                fprintf(stdout, "sshell$ ");
                free(input);
                input = NULL;
                len = 0;
                continue;
            }
            // Return special pid value
            fprintf(stderr,"+ completed '%s' [0]\n", input);
            fprintf(stdout, "sshell$ ");
            free(input);
            input = NULL;
            len = 0;
            continue;
        }
        // PWD
        if (strcmp(myLine.commandStructures[0].params[0], "pwd") == 0){
            char* directory = (char *)malloc(100 * sizeof(char));
            getcwd(directory, 100);
            fprintf(stdout,"%s\n", directory);
            fprintf(stderr,"+ completed '%s' [0]\n", input);
            fprintf(stdout, "sshell$ ");
            free(input);
            input = NULL;
            len = 0;
            continue;
        }

        // Execute Line
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

