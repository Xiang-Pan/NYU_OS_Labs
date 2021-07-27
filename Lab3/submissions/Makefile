CC = g++
CFLAGS = -std=gnu++11 -static-libstdc++

headers=$(wildcard ./*.hpp)

mmu: main.cpp
	$(CC) $(CFLAGS) main.cpp -o mmu

clean:
	rm -f mmu *~
	rm *.o