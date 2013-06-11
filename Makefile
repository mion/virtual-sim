CXX=gcc
FLAGS=-ggdb -Wall

make:
	${CXX} ${FLAGS} -o bin/main util.c simulator.c memory.c main.c

clean:
	rm -f bin/main
