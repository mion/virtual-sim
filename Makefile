CXX=gcc
FLAGS=-ggdb -Wall

make:
	${CXX} ${FLAGS} -o bin/main util.c simulator.c memory.c main.c

unittest:
	${CXX} ${FLAGS} -o bin/unittest util.c simulator.c memory.c unittest.c

clean:
	rm -f bin/main
	rm -f bin/unittest
