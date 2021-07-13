CC = g++
CFLAGS = -std=gnu++11 -static-libstdc++

headers=$(wildcard ./*.hpp)
sched: main.cpp
	$(CC) $(CFLAGS) main.cpp -o sched
	
clean:
	rm -f sched *~
	rm *.o
