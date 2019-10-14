#ifndef PROJECT1_LINE_H
#define PROJECT1_LINE_H

#include "command.h"

struct line {

    char *commandStrings[10];

    struct command commandStructures[10];

    int errored;

    int pipeArray[11][2];
};

struct line constructLine(char *input);
void runLine(struct line myLine);

#endif

