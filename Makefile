CXX=gcc
FLAGS=-ggdb -Wall

make:
	${CXX} ${FLAGS} -o bin/main main.c

clean:
	rm -f bin/main
