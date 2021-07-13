CC = g++
CFLAGS = -std=gnu++11 -static-libstdc++

headers=$(wildcard ./*.hpp)
sched: main.cpp
	$(CC) $(CFLAGS) main.cpp -o sched

event_manager.o: event_manager.hpp
	$(CC) $(CFLAGS)  -c main.cpp event_manager.hpp


clean:
	rm -f sched *~
	rm *.o
