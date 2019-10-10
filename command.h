#ifndef COMMAND
#define COMMAND

#include <stdlib.h>

struct command {
    char *params[17];
    char *inFile;
    char *outFile;
    int errored;
};

struct command constructCommand(char *input, int first, int last);
int runCommand(struct command cmd, int isPiped);

#endif