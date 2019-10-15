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
    struct line myLine;

    // Other Variables
    pid_t pid;
    int status;

    // Get User Input
    fprintf(stdout, "sshell$ ");
    getline(&input, &len, stdin);
    strtok(input, "\n");

    // Construct Line
    myLine = constructLine(input);

    // Execute Line
    pid = runLine(myLine);

    if(myLine.errored == 1){
        return;
    }

    // TODO: all status gathering will be done in Line. Sshell receives and prints string
    waitpid(pid, &status, 0);


    fprintf(stderr, "+ completed '%s' [%d]\n", input, WEXITSTATUS(status));
    sleep(1);

    simpleShell();
}


/*


echo toto | tr o i


 */

