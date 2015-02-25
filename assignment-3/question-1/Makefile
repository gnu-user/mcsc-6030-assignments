CC = gcc
CFLAGS = -Wall -std=c99
OPENMP = -fopenmp
LFLAGS = 
LIBS = -lm
EXE = test1.exe test2.exe test2_omp.exe
.PHONY: clean help

test1.exe : test1.o quadrature.o
	$(CC) $(CFLAGS) test1.o quadrature.o $(LIBS) -o $@ 

test2.exe : test2.o quadrature.o
	$(CC) $(CFLAGS) test2.o quadrature.o $(LIBS) -o $@ 

test1.o : test1.c
	$(CC) $(CFLAGS) -c $<

test2.o : test2.c
	$(CC) $(CFLAGS) -c $< 

quadrature.o : quadrature.c
	$(CC) $(CFLAGS) -c $< 

test2_omp.exe : test2_omp.o quadrature_omp.o
	$(CC) $(CFLAGS) $(OPENMP) test2_omp.o quadrature_omp.o $(LIBS) -o $@ 

test2_omp.o : test2_omp.c
	$(CC) $(CFLAGS) $(OPENMP) -c $< 

quadrature_omp.o : quadrature_omp.c
	$(CC) $(CFLAGS) $(OPENMP) -c $< 


all : $(EXE)


clean:
	rm -f *.o $(EXE) *~

help:
	@echo "Valid targets:"
	@echo "  test1.exe"
	@echo "  test2.exe"
	@echo "  test2_omp.exe"
	@echo "  all"
	@echo "  clean:  removes .o and .exe files"
