sshell: sshell.o command.o
	gcc -Wall -Werror -o sshell sshell.o command.o
sshell.o: sshell.c
	gcc -Wall -Werror -c -o sshell.o sshell.c

command.o: command.c command.h
	gcc -Wall -Werror -c -o command.o command.c

clean:
	rm -f sshell sshell.o command.o