OBJS = main.o coloring3.o
CC = g++ -std=c++14
DEBUG = -Wall
CFLAGS = -c $(DEBUG)
LFLAGS = $(DEBUG)

run : $(OBJS) main.cpp
	$(CC) $(LFLAGS) $(OBJS) -o main

main.o : main.cpp coloring3.hpp
	$(CC) $(CFLAGS) main.cpp

coloring3.o : coloring3.cpp coloring3.hpp
	$(CC) $(CFLAGS) coloring3.cpp

clean:
	rm -r *.o *.dSYM main
