#ifndef PROJECT1_LINE_H
#define PROJECT1_LINE_H

#include "command.h"

struct line {

    char *commandStrings[10];

    struct command commandStructures[10];

    pid_t pidArray[10];

    int statusArray[10];

    int errored;

    int backgrounded;

    int numCommands;
};

struct line constructLine(char *input);
int runLine(struct line *myLine);
char* concat(const char *s1, const char *s2);

#endif

