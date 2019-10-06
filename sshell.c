#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "command.h"
#include "line.h"

#define MAX_INPUT_LEN 512
#define ERROR_T "1"
#define ERROR_F "0"

int main(int argc, char *argv[])
{
    // User Input
	char *input = (char*) malloc(MAX_INPUT_LEN*sizeof(char));
	size_t len = MAX_INPUT_LEN;

	// Data Structures
	struct command cmd;
	struct line myLine;

	// Other Variables
	pid_t pid;
	int status;

    // Initialize Input Loop
    int continueLoop = 1;

    // MAIN INPUT LOOP
	do {

        // Get User Input
        fprintf(stdout, "sshell$ ");
        getline(&input, &len, stdin);
        strtok(input, "\n");
        //cmd = constructCommand(input);
        myLine = constructLine(input);
        //check if line construction errored out
        if(myLine.errored == 0) {
            fprintf(stderr, "+ completed '%s' [%d]\n", input, 1);
            continue;
        }
        cmd = myLine.commandStructures[0]; // Does not yet work for index > 0

        // Start Process
        pid = fork();
        if(pid == 0) {
            // Child Process
            execvp(cmd.params[0], cmd.params);
            perror("execvp");
            exit(1);
        } if(pid > 0) {
            // Parent Process
            waitpid(-1, &status, 0);
            fprintf(stderr, "+ completed '%s' [%d]\n", input, WEXITSTATUS(status));
            // Get User Input
            fprintf(stdout, "sshell$ ");
            getline(&input, &len, stdin);
            //TODO: fix, weird way of removing trailing newline
            //return EXIT_SUCCESS;
        } else {
            perror("fork");
            exit(1);
        }

        // TODO: STR Compare
        continueLoop = 0;


	} while (continueLoop == 1);





}


// (strcmp("exit", input))


