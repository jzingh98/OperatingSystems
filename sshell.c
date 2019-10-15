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

        // Execute Line
        status = runLine(&myLine);

        int wait = 1;
        if(myLine.backgrounded == 1) wait = 0;
        struct process_node *newProcess = (struct process_node*) malloc(sizeof(struct process_node));
        constructProcess(newProcess, myLine.numCommands, myLine.pidArray, input, wait);
        if(unfinishedProcesses == newProcess){
            printf("error");
        }
        unfinishedProcesses = appendProcess(unfinishedProcesses, newProcess);
        if(unfinishedProcesses != NULL && unfinishedProcesses == unfinishedProcesses->next){
            printf("error");
        }

        //if not backgrounded wait for the current process to finish;

        unfinishedProcesses = printCompletedProcesses(unfinishedProcesses);


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

