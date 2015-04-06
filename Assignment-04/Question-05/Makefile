MPICC = mpicc
CFLAGS = -Wall -O2 -march=native -std=c99
OPENMP = -fopenmp
LFLAGS = 
LIBS = -lm
SOURCES = matvec.c
OBJECTS = $(subst .c,.o,$(SOURCES))
EXE = matvec.exe
.PHONY: clean help

matvec.exe : matvec.o
	$(MPICC) $(CFLAGS) $(OPENMP) $^ $(LIBS) -o $@ 

%.o : %.c
	$(MPICC) $(CFLAGS) $(OPENMP) -c $< 

all : $(EXE)

clean:
	rm -f $(OBJECTS) $(EXE) *~

help:
	@echo "Valid targets:"
	@echo "  clean:  removes .o and .exe files"
