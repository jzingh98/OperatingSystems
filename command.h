#ifndef COMMAND
#define COMMAND

#include <stdlib.h>

struct command {
    char *params[17];
    char *inFile;
    char *outFile;
    int errored;
    int builtin;
};

struct command constructCommand(char *input, int first, int last);
void deconstructCommand(struct command *cmdPtr);
pid_t runCommand(struct command cmd, int* prevPipe, int* currPipe);

#endif