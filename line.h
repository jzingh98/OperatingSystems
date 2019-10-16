#ifndef PROJECT1_LINE_H
#define PROJECT1_LINE_H

#include "command.h"

struct line {

    struct command *commandStructures;

    pid_t *pidArray;

    int *statusArray;

    int errored;

    int backgrounded;

    int numCommands;
};

struct line constructLine(char *input);
int runLine(struct line *myLine);
char* concat(const char *s1, const char *s2);

#endif

