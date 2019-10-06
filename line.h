#ifndef PROJECT1_LINE_
#define PROJECT1_LINE_H

#include "command.h"

struct line {

    char *commandStrings[10];

    struct command commandStructures[10];

};

struct line constructLine(char *input);

#endif
