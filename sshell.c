#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>

#include "command.h"
#include "line.h"

#define MAX_INPUT_LEN 512
#define ERROR_T "1"
#define ERROR_F "0"

#define COMMANDDELIMS "|"

void startPipeline(char* input, int currCommand);
void continuePipeline(char *process1, char *process2);


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




void startPipeline (char* input, currCommand) {

    // Data Structures
    struct command cmd;

    // Other Variables
    pid_t pid;
    int status;

    // Input String Variables
    char* token;
    char* nextToken;
    char* rest;
    char* restCopy;

    // Chop off first command from input line
    rest = strdup(input);
    token = strtok_r(rest, COMMANDDELIMS, &rest);

    // Start Recursion
    continuePipeline(token, rest);


}




void continuePipeline(char *process1, char *process2)
{
    int fd[2];
    pipe(fd); /* Create pipe */
    if (fork() != 0) {
        /* Parent */
        // Always waits for the child to finish.
        waitpid(-1, &status, 0);

        // Base Case: after waiting, checks process 2, null. Return
        if (strcmp(process2, NULL) == 0){
            fprintf(stderr, "+ completed '%s' [%d]\n", input, WEXITSTATUS(status));
            //return EXIT_SUCCESS;
        } else{
            // Recursive Case: after waiting, checks process 2. Not null. Calls itself with token and chop
            close(fd[0]); /* Don't need read access to pipe */
            dup2(fd[1], STDOUT_FILENO); /* Replace stdout with the pipe */
            close(fd[1]); /* Close now unused file descriptor */

            // Chop off first command from input line
            char* rest = strdup(process2);
            char* token = strtok_r(rest, COMMANDDELIMS, &rest);

            // Recursive call
            continuePipeline(token, rest);
        }

    } else {
        /* Child */
        // Always executes process 1.

        // Construct command
        struct command cmd;
        cmd = constructCommand(strdup(process1), 0, 0);

        // Make Pipe Connection
        close(fd[1]); /* Don't need write access to pipe */
        close(STDIN_FILENO); /* Close existing stdin */
        dup(fd[0]); /* And replace it with the pipe */
        close(fd[0]); /* Close now unused file descriptor */

        // Execute process
        execvp(cmd.params[0], cmd.params);
        perror("execvp");
        exit(1);

    }
}






