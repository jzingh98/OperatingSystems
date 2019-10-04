#ifndef PROJECT1_LINE_H
#define PROJECT1_LINE_H

struct line {
    char *individualCommands[10];
};

struct line constructLine(char *input);

#endif
