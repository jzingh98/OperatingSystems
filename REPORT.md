# ECS 150 Project 1 Report

## Design and Structure
In designing our program, we attempted to modularize as much as 
possible. Though C does not offer classes with encapsulated functions, 
we took advantage of structs, data structures, and separated files to
achieve modularization and maximize code quality and readability.

### SShell.C

This file contains our programs main execution loop, simpleShell(), which 
iterates until the user chooses to exit. At each iteration, a line of input is 
accepted and saved as a character array. From the char array, a **line** 
struct is created (detailed below). In the event the line contains any one 
of the three built in commands, **cs**, **pwd**, or **exit**, appropriate 
system calls are made. Otherwise, the line is executed with **runLine**.

### Line.C

Line.C defines the line struct, which is itself composed of an array of 
**command** structs (as well as supporting fields used to track errors, 
background processes, etc.). There are two primary associated 
functions: **constructLine** and **runLine**. ConstructLine accepts a 
char array of user input and parses it, creating a command struct for 
each individual command separated by the "|" character. Non exec 
related errors are also checked for during line construction. RunLine is 
responsible for the execution of each command, as well as the 
construction of the pipeline. It iterates through each command in line, 
calling **runCommand** and with each call, defining and passing the 
next pipe segment in the pipeline. In the event a command is either the 
first or last in line, a modified call with one NULL pipe segment is made. 
Each call to runCommand returns the PID of the newly created process 
(details in Command.C), which is pushed to an array of PIDs. This array 
is later used by Process.C.

### Command.C

Command.C defines the command struct and manages the construction 
and execution of individual commands. **ConstructCommand** accepts 
a command consisting of a function and its arguments, and saves them 
in an array of parameters, with param[0] being the function. Supporting 
fields, like whether the command is first or last, are also initialized. 
Finally, in the event a command is either the first or last command in 
line, the constructor  checks for input and output redirection, 
respectively. 

**RunCommand** is responsible the execution of a command. Upon 
forking, appropriate pipe connections are made, connecting the output 
of the previous previous pipe to position 0 on the process file descriptor 
table, and the output of the process to the input of the next pipe. The 
parent closes unused pipe ends, and returns the PID.

### Process.C
Process.C maintains a linked list of currently running processes. It is 
created in Sshell using the PID array maintained in the line struct. This 
list of running processes is updated at ever iteration of the main 
simpleShell loop to add additional processes in execution. Normal lines 
running in the foreground never never stay in the linked list for more 
than one iteration. Background processes may be maintained over 
multiple iterations. Once the background process terminates, it is 
removed from the linked list.

## Development
In development, we referenced a tutorial by Rutgers University on how 
pipes work and how to create pipes between two or more multiple child 
processes.

https://www.cs.rutgers.edu/~pxk/416/notes/c-tutorials/pipe.html

## Testing and Debugging
To test the correctness of our project, we used the provided test script 
and example inputs and outputs on the homework prompt. We also 
relied heavily on the CLion IDE and its debugger.

