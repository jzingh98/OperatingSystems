#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>

#include "command.h"
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
    char *input = (char*) malloc(MAX_INPUT_LEN*sizeof(char));
    size_t len = MAX_INPUT_LEN;

    // Data Structures
    struct command cmd;
    struct line myLine;

    // Other Variables
    pid_t pid;

    // Get User Input
    fprintf(stdout, "sshell$ ");
    getline(&input, &len, stdin);
    strtok(input, "\n");

    // Construct Line
    myLine = constructLine(input);
    cmd = myLine.commandStructures[1];

    // Execute Line
    runLine(myLine);


}


// echo toto | tr o i


