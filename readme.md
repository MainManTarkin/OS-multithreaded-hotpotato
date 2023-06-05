# OS Semaphore and Condition Flags Program

## Project Description

The purpose behind this project was to understand how a multithreaded environment with shared resources works. The program runs a simulation of the party game hot potato. A thread will try to aquire the potato (the shared resource) be aquiring the mutex and setting the semaphore (which is esstianly the potato). If a thread fails to set the semaphore, becuase it is already set, it then will wait for the condition flag to be set releasing the other threads to try to set the semaphore (or in other words aquire the potato). The holder of the potato will keep it for a random amount of time until they release it. The main thread will print out the current results of the simulation, the threads that currently hold a potato, and the variance between them.

------
## Building

this was program was built on posix compliant system (your system should have pthreads on it).
Since this project has a makefile in it, The command to build the project should just be:

> make

use the following command

> make clean

to remove all .d files from directory.

-------
## Program Args

The program has two arguments that can be given to affect the simulation and a __help__ command.

* -h : prints all the commands, what they do, and their input values.
* -n : is used to set the number of players (threads). The defualt value is 4 and it can not be less then one or greater than thirty-two.
* -p : this arg is used to affect the number potatos(times a semphore can be set) in play. The defualt value is one and can not be more than the number of players

## Quiting the program

the program is quit by sending __SIGINT__.