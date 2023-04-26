This program can be compiled on the os1 server by typing the following:
gcc --std=gnu99 -pthread -o myCounter main.c

To then run the program, type the following:
./myCounter

Other notes:
I have two commented out sleep(1) functions that you can uncomment (one at a time works better) if no "waiting on cond" messages appear, there is 
one in each thread function