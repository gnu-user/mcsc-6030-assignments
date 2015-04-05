MPICC = mpicc
CFLAGS = -Wall -std=c99
OPENMP = -fopenmp
LFLAGS = 
LIBS = -lm
SOURCES = mpi_root.c
OBJECTS = $(subst .c,.o,$(SOURCES))
EXE = mpi_root.exe
.PHONY: clean help

mpi_root.exe : mpi_root.o
	$(MPICC) $(CFLAGS) $(OPENMP) $^ $(LIBS) -o $@ 

%.o : %.c
	$(MPICC) $(CFLAGS) $(OPENMP) -c $< 

all : $(EXE)

clean:
	rm -f $(OBJECTS) $(EXE) *~

help:
	@echo "Valid targets:"
	@echo "  clean:  removes .o and .exe files"
