Scheduling
==========

Implemented scheduling algorithms using C and Pthread

This program implements scheduling algorithms using pthreads in c language. When user executes a file named scheduling.c, first it will ask for input file. After entering any filename by user, it asks user about PID, Burst time & Priority and this data stored in the input file. All the work stated above, creating the file, data input and store is done by one thread.
Now, second thread opens the file, reads the data and asks you to for desired output method.

Algorithms implemented:
FCFS
Round Robin
Shortest job first
