CC = g++
CFLAGS = -std=gnu++11 -static-libstdc++

headers=$(wildcard ./*.hpp)

iosched: main.cpp
	$(CC) $(CFLAGS) main.cpp -o iosched

clean:
	rm -f mmu *~
	rm *.o