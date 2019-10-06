#ifndef COMMAND
#define COMMAND

struct command {
    char *params[17];
    char *inFile;
    char *outFile;
    int initialized;
};

struct command constructInnerCommand(char *input);
struct command constructFirstCommand(char *input);
struct command constructLastCommand(char *input);
struct command constructOnlyCommand(char *input);

#endif