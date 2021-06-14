CC = g++
CFLAGS = -std=gnu++11
linker: linker.cpp main.cpp
	$(CC) $(CFLAGS) linker.cpp main.cpp -o linker
clean:
	rm -f linker *~
	rm *.o
# TARGET = linker
# all: $(TARGET)

# linker: main.cpp
# 	$(CC) main.cpp $(CFLAGS) -o linker.o
# linker.o : linker.c linker.h
# 	cc -c linker.c
# clean:
# 	$(RM) $(TARGET) $(TARGET).o