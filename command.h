#ifndef COMMAND
#define COMMAND

struct command {
    char *params[17];
    char *inFile;
    char *outFile;
    int errored;
};

struct command constructCommand(char *input, int first, int last);

#endif