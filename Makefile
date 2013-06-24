CXX=gcc
FLAGS=-ggdb -Wall

make:
	${CXX} ${FLAGS} -o bin/main util.c simulator.c memory.c main.c

report:
	${CXX} ${FLAGS} -o bin/report util.c simulator.c memory.c report.c

test:
	${CXX} ${FLAGS} -o bin/test util.c simulator.c memory.c test.c

clean:
	rm -f bin/main
	rm -f bin/report
	rm -f bin/unittest
