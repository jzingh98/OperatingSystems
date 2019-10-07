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

void subcommand(char* input, int currCommand);


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
        subcommand(input, 0);

        // TODO STR Compare
        continueLoop = 0;

	} while (continueLoop == 1);

}




void subcommand (char* input, currCommand) {

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

    // Peek at the next command without modifying rest
    restCopy = strdup(rest);
    nextToken = strtok_r(restCopy, COMMANDDELIMS, &rest);

    // This is the last recursive call
    if (nextToken == NULL){
        if (currCommand == 0){
            cmd = constructCommand(strdup(token), 1, 1);
        } else {
            cmd = constructCommand(strdup(token), 0, 1);
        }
    // This is not the last recursive call
    } else {
        if (currCommand == 0){
            cmd = constructCommand(strdup(token),1, 0);
        } else {
            cmd = constructCommand(strdup(token), 0, 0);
        }
    }
   // TODO: Check if errored out


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



}







//void pipeline(char *process1, char *process2)
//{
//    int fd[2];
//    pipe(fd); /* Create pipe */
//    if (fork() != 0) {
//        /* Parent */
//        close(fd[0]); /* Don't need read access to pipe */
//        dup2(fd[1], STDOUT_FILENO); /* Replace stdout with the pipe */
//        close(fd[1]); /* Close now unused file descriptor */
//        exec(process1);
//    } else {
//        /* Child */
//        close(fd[1]); /* Don't need write access to pipe */
//        close(STDIN_FILENO); /* Close existing stdin */
//        dup(fd[0]); /* And replace it with the pipe */
//        close(fd[0]); /* Close now unused file descriptor */
//        exec(process2);
//    }
//}






