CC = g++
CFLAGS = -std=gnu++11 -static-libstdc++
linker: tokenizer.hpp main.cpp
	$(CC) $(CFLAGS) tokenizer.hpp main.cpp -o linker
clean:
	rm -f linker *~
	rm *.o
