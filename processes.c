//
// Created by Tycho Yacub on 10/15/19.
//

#include <wait.h>
#include <stdio.h>
#include <string.h>

#include "processes.h"



void printCompletedProcess(struct process_node process){
    fprintf(stderr, "+ completed '%s' ",process.input);
    for(int i = 0; i < process.numCommands; i++) {
        fprintf(stderr, "[%d]", process.statuses[i]);
    }
    fprintf(stderr, "\n");
    return;
}

void updateStatusesBackground(pid_t *processIds, int *statuses, int numCommands){
    int i = 0;
    int fill;
    int status;
    while(i < numCommands){
        pid_t child = waitpid(processIds[i], &status, WNOHANG);
        if(child == 0){
            statuses[i] = -1;
            fill = -1;
        }
        else{
            statuses[i] = WEXITSTATUS(status);
        }
        i++;
    }
    i++;
    while(i < numCommands){
        statuses[i] = fill;
    }
}
void updateStatusesForeground(pid_t *processIds, int *statuses, int numCommands) {
    int i = 0;
    int fill;
    int status;
    while(i < numCommands){
        waitpid(processIds[i], &status, 0);
        statuses[i] = WEXITSTATUS(status);
        if(statuses[i] > 0) {
            fill = 0;
            break;
        }
        i++;
    }
    i++;
    while(i < numCommands){
        statuses[i] = fill;
    }
}

void printCompletedProcessesForeground(struct process_node *node) {
        updateStatusesForeground(node->processIds, node->statuses, node->numCommands);
        printCompletedProcess(*node);
}


struct process_node *printCompletedProcessesBackground(struct process_node *first_node) {
    struct process_node *curr_node = first_node;

    while(curr_node != NULL){
        if(curr_node == curr_node->next){
            printf("error");
        }
        updateStatusesBackground(curr_node->processIds, curr_node->statuses, curr_node->numCommands);
        //if the process is done then print its exit
        if(curr_node->statuses[curr_node->numCommands-1] != -1) {
            printCompletedProcess(*curr_node);
            //If it is the only process in the list set the first node to NULL and return
            if(curr_node->prev == NULL && curr_node->next == NULL){
                first_node = NULL;
                return first_node;
            };

            //If it was the first in the list set the next to be the first;
            if(curr_node->prev == NULL){
                first_node = curr_node->next;
                curr_node->next->prev = NULL;
            }

            //If it was the last in the list then set the second to last to the last.
            else if(curr_node->next == NULL) curr_node->prev->next = NULL;

            //If in the middle join the nodes on either side
            else{
                curr_node->prev->next = curr_node->next;
                curr_node->next->prev = curr_node->prev;
            }
        }
        curr_node = curr_node->next;
    }
    return first_node;
}

struct process_node *appendProcess(struct process_node *head_node, struct process_node *new_node){
    if(head_node == NULL) {
        head_node = new_node;
        return head_node;
    }
    struct process_node *curr_node = head_node;
    while(curr_node->next != NULL) {
        curr_node = curr_node->next;
    }
    if(curr_node == new_node){
        printf("help");
    }
    curr_node->next = new_node;
    new_node->prev = curr_node;
    return head_node;
}

void constructProcess(struct process_node *newNode, int numCommands, pid_t *pidArray, char* input){
    newNode->processIds = (pid_t*)malloc(numCommands* sizeof(pid_t));
    newNode->statuses = (int*)malloc(numCommands* sizeof(int));
    newNode->input = strdup(input);
    newNode->numCommands = numCommands;
    newNode->prev = NULL;
    newNode->next = NULL;

    for(int i = 0; i < numCommands; i++) {
        newNode->processIds[i] = pidArray[i];
        newNode->statuses[i] = -1;
    }
    return;
}

