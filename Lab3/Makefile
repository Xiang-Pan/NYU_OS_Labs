CC = g++
CFLAGS = -std=gnu++11 -static-libstdc++

headers=$(wildcard ./*.hpp)
mmu: main.cpp
	$(CC) $(CFLAGS) main.cpp -o mmu

# event_manager.o: event_manager.hpp
# 	$(CC) $(CFLAGS)  -c main.cpp event_manager.hpp


clean:
	rm -f mmu *~
	rm *.o