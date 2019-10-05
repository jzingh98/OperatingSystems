#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>

#include "command.h"
#include "line.h"

#define MAX_INPUT_LEN 512

int main(int argc, char *argv[])
{
	char *input = (char*) malloc(MAX_INPUT_LEN*sizeof(char));
	size_t len = MAX_INPUT_LEN;
	struct command cmd;
	struct line myLine;
	pid_t pid;
	int status;

	// Get User Input
	fprintf(stdout, "sshell$ ");
	getline(&input, &len, stdin);
	//TODO: fix, weird way of removing trailing newline

	while ((strcmp("exit", input))){

        strtok(input, "\n");

        cmd = constructCommand(input);

        // TODO (in progress)
        myLine = constructLine(input);



        pid = fork();
        if(pid == 0) {
            execvp(cmd.params[0], cmd.params);
            perror("execvp");
            exit(1);
        } if(pid > 0) {
            waitpid(-1, &status, 0);
            fprintf(stderr, "+ completed '%s' [%d]\n", input, WEXITSTATUS(status));
        } else {
            perror("fork");
            exit(1);
        }

        // Get User Input
        fprintf(stdout, "sshell$ ");
        getline(&input, &len, stdin);
        //TODO: fix, weird way of removing trailing newline
	}


    return EXIT_SUCCESS;






}
