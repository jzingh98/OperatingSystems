sshell: sshell.o command.o line.o processes.o
	gcc -Wall -Werror -o sshell sshell.o line.o command.o processes.o

sshell.o: sshell.c command.o line.o processes.o
	gcc -Wall -Werror -c -o sshell.o sshell.c

line.o: line.c line.h
	gcc -Wall -Werror -c -o line.o line.c

command.o: command.c command.h
	gcc -Wall -Werror -c -o command.o command.c

processes.o: processes.c processes.h
	gcc -Wall -Werror -c -o processes.o processes.c

clean:
	rm -f sshell sshell.o command.o line.o processes.o