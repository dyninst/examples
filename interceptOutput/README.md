# Intercept Stream Output

Add code to a running program to save all data the program writes to its
standard output file descriptor. In this way it
works like "tee," which passes output along to its own standard out
while also saving it in a file. The motivation for the example program
is that you run a program, and it starts to print copious lines of
output to your screen, and you wish to save that output in a file
without having to re-run the program.
