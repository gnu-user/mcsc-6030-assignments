CC = gcc
CFLAGS = -std=c99
OPENMP = -fopenmp
LFLAGS = 
LIBS = -lm
SOURCES = functions.c quadrature.c test.c
OBJECTS = $(subst .c,.o,$(SOURCES))
.PHONY: clean help

test.exe : test.o quadrature.o functions.o
	$(CC) $(CFLAGS) $(OPENMP) $^ $(LIBS) -o $@ 

test2.exe : test2.o quadrature.o functions.o
	$(CC) $(CFLAGS) $(OPENMP) $^ $(LIBS) -o $@ 

%.o : %.c
	$(CC) $(CFLAGS) $(OPENMP) -c $< 

test.o : functions.o quadrature.o

test2.o : functions.o quadrature.o

all : test.exe test2.exe

clean:
	rm -f $(OBJECTS) test.exe *~

help:
	@echo "Valid targets:"
	@echo "  clean:  removes .o and .exe files"
