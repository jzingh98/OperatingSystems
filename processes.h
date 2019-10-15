//
// Created by Tycho Yacub on 10/15/19.
//

#ifndef PROJECT1_PROCESSES_H
#define PROJECT1_PROCESSES_H

#include <stdlib.h>

struct process_node{
    pid_t *processIds;
    int *statuses;
    char *input;
    int numCommands;
    int wait;
    struct process_node* prev;
    struct process_node* next;
};

struct process_node *printCompletedProcesses(struct process_node *first_node);
struct process_node *appendProcess(struct process_node *head_node, struct process_node *new_node);
void constructProcess(struct process_node *newNode, int numCommands, pid_t *pidArray, char* input, int wait);

#endif //PROJECT1_PROCESSES_H
