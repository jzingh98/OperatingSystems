#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>

#include "command.h"

#define MAX_INPUT_LEN 512
#define ERROR_T "1"
#define ERROR_F "0"

#define COMMANDDELIMS "|"

void startPipeline(char* input, int currCommand);
void continuePipeline(char *process1, char *process2, int* fd);


int main(int argc, char *argv[])
{
    // User Input
	char *input = (char*) malloc(MAX_INPUT_LEN*sizeof(char));
	size_t len = MAX_INPUT_LEN;

    // Initialize Input Loop
    int continueLoop = 1;

    // MAIN INPUT LOOP
	do {
        // Get User Input
        fprintf(stdout, "sshell$ ");
        getline(&input, &len, stdin);
        strtok(input, "\n");
        startPipeline(input, 0);

        // TODO STR Compare
        continueLoop = 0;

	} while (continueLoop == 1);

}




void startPipeline (char* input, int currCommand) {

    // Data Structures
    struct command cmd;

    // Input String Variables
    char* token;
    char* nextToken;
    char* rest;
    char* restCopy;

    // Chop off first command from input line
    rest = strdup(input);
    token = strtok_r(rest, COMMANDDELIMS, &rest);

    // Pipe Variables
    int fd[2];
    pipe(fd);

    // Start Recursion
    continuePipeline(token, rest, fd);

}




void continuePipeline(char *process1, char *process2, int* fd)
{

    int status;

    /* Parent */
    if (fork() != 0) {

        // Base Case: after waiting, checks process 2, null. Return
        if (strcmp(process2, "") == 0){
            // Read from Input
            char inbuf[10];
            memset(inbuf, 0, sizeof(inbuf));
            read(fd[0], inbuf, 10);

            if(inbuf[0] != 0){
                fprintf(stderr, "read something");
            }

            fprintf(stderr, "+ completed '%s' [%d]\n", process1, WEXITSTATUS(status));
            //return EXIT_SUCCESS;
        } else{



            // Read from Input
            //char inbuf[100];
            //memset(inbuf, 0, sizeof(inbuf));
            //read(fd[0], inbuf, 100);

            // Chop off first command from input line
            char* rest = strdup(process2);
            char* token = strtok_r(rest, COMMANDDELIMS, &rest);

            // Append input from pipe to token
            //strcat (token, " ");
            //strcat (token, inbuf);

            // Recursive call
            continuePipeline(token, rest, fd);
        }

    /* Child */
    } else {
        // Always executes process 1

        // Construct command
        struct command cmd;
        cmd = constructCommand(strdup(process1), 0, 0);

        // Replace Output
        close(fd[0]); /* Don't need read access to pipe */
        dup2(fd[1], STDOUT_FILENO); /* Replace stdout with the pipe */

        // Replace Input
        close(fd[1]); /* Don't need write access to pipe */
        dup2(fd[0], STDIN_FILENO); /* Replace stdin with the pipe */

        // Close connections
        close(fd[0]); /* Close now unused file descriptor */
        close(fd[1]); /* Close now unused file descriptor */

        // Execute process
        execvp(cmd.params[0], cmd.params);

        // // Add Newline to pipe
        // write(fd[1], "\n", sizeof("\n"));

    }
}




// echo toto | tr o i

