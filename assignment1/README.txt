This program can be compiled on the os1 server by typing the following:
gcc --std=gnu99 -o movies test_movies.c movie.c list.c

To then run the program, type the following:
./movies file.csv

**file.csv should be replaced with your movie file name, for example, in this folder I have the test file named 'ms.csv', so I would type './movies ms.csv' to properly run the program**

Other notes:
The main function can be found in test_movies.c
The movie struct definition and functions can be found in movie.c, which also has a header file movie.h
The list/node struct definitions and functions can be found in list.c, which also has a header file list.h