#ifndef COMMAND
#define COMMAND

struct command {
    char *params[17];
};

struct command constructCommand(char *input);

#endif