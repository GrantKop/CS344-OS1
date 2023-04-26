This program can be compiled on the os1 server by typing the following:
make all

To compile individual aspects of the program, type any of the following:
make server
make client
make key

To then run the program using the provided test script, type the following:
./p5testscript RANDOM_PORT1 RANDOM_PORT2 > mytestresults 2>&1

*RANDOM_PORT1/2 should be any number in the 50000+ range. (this is what the canvas page recommends)

Other notes:
When I ran the test script with VS code it would occasionally run into fork errors, but when I used windows command prompt it worked fine