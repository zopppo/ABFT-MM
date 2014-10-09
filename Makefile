.SUFFIXES:
.SUFFIXES: .c .cpp .o

CC = gcc

OBJS= abft.o\
    matrix.o   \

CFLAGS=-g -Wall -std=gnu89 

all: matrix makeMatrix printMatrix checksumA checksumB detect correct multiplyMatrix compareMatrix abft 


${OBJS}: matrix.h     \
         Makefile

matrix: ${OBJS}
	-echo Linking $@
	gcc -o $@ ${OBJS}

test:
	./matrix

gdb:
	gdb ./matrix
clean:
	rm -f *.o *.dat matrix makeMatrix printMatrix checksumA checksumB \
	 detect correct multiplyMatrix compareMatrix abft 

makeMatrix: makeMatrix.o matrix.o
	gcc makeMatrix.o matrix.o -o makeMatrix

printMatrix: printMatrix.o matrix.o
	gcc printMatrix.o matrix.o -o printMatrix 

checksumA: checksumA.o matrix.o
	gcc checksumA.o matrix.o -g -std=gnu89 -o checksumA

checksumB: checksumB.o matrix.o
	gcc checksumB.o matrix.o -g -std=gnu89 -o checksumB

detect: detect.o matrix.o
	gcc detect.o matrix.o -g -std=gnu89 -o detect

correct: correct.o matrix.o
	gcc correct.o matrix.o -g -std=gnu89 -o correct

compareMatrix: compareMatrix.o matrix.o
	gcc compareMatrix.o matrix.o -g -std=gnu89 -o compareMatrix

abft: abft.o matrix.o
	gcc abft.o matrix.o -g -std=gnu89 -o abft
