#ifndef PROJECT1_LINE_H
#define PROJECT1_LINE_H

#include "command.h"

struct line {

    char *commandStrings[10];

    struct command commandStructures[10];

    int errored;
};

struct line constructLine(char *input);

#endif
