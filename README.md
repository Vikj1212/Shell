## Description
Implemented a Shell-like environment in c using the system calls fork() and exec().
Use Makefile to test functionality against test scripts.

#### Features of this program: 
* Program prints out a prompt of msh> when it is ready to accept input. It reads a line of input and, if the command given is a supported shell command, it shall execute the command and display the output of the command.
* A 'history' command is implemented that displays the previous 15 commands.
* The history command supports a -p parameter. This will cause the history command to list the associated PID with the command.
* To exit program, enter command 'quit'.
