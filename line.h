#ifndef PROJECT1_LINE_H
#define PROJECT1_LINE_H

#include "command.h"

struct line {

    char *commandStrings[10];

    struct command commandStructures[10];

    int pidArray[10];

    int statusArray[10];

    int errored;

    int numCommands;

    int pipeArray[11][2];
};

struct line constructLine(char *input);
int runLine(struct line myLine);
char* concat(const char *s1, const char *s2);

#endif

