CC = g++
CFLAGS = -std=gnu++11 -static-libstdc++

headers=$(wildcard ./*.hpp)
sched: main.cpp
	$(CC) $(CFLAGS) main.cpp -o sched


clean:
	rm -f sched *~
	rm *.o

# edit: main.o AAA.o BBB.o CCC.o 
#     cc -o edit main.o AAA.o BBB.o CCC.o 
# sched: main.o 
# main.o: main.cpp 
# 	cc -c main.cpp
# event.o: even.hpp
# 	cc -c even.hpp
# event_manager.o: event_manager.hpp 
# 	cc -c event_manager.hpp 
# input_handler.o: input_handler.hpp
# 	cc -c input_handler.hpp
# output_handler.o: output_handler.hpp
# 	cc -c output_handler.hpp
# clean:
# 	rm edit main.o AAA.o BBB.o CCC.o 

#       test:test.cppw1.o w2.o w3.o

#                 g++ -o test test.cpp w1.o w2.o w3.o

#         w1.o:w1.cpp

#                 g++ -c -o w1.o w1.cpp

#         w2.o:w2.cpp

#                 g++ -c -o w2.o w2.cpp

#         w3.o:w3.cpp

#                 g++ -c -o w3.o w3.cpp
