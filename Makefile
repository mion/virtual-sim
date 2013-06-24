CXX=gcc
FLAGS=-ggdb -Wall

make:
	${CXX} ${FLAGS} -o bin/main util.c simulator.c memory.c main.c

test:
	${CXX} ${FLAGS} -o bin/test util.c simulator.c memory.c test.c

clean:
	rm -f bin/main
	rm -f bin/unittest
