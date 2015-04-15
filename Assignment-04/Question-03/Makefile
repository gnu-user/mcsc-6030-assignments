MPICC = mpicc
CFLAGS = -Wall -O2 -march=native -std=c99
LFLAGS = 
LIBS = -lm
SOURCES = mpi_ping_pong.c
OBJECTS = $(subst .c,.o,$(SOURCES))
EXE = mpi_ping_pong.exe
.PHONY: clean help

mpi_ping_pong.exe : mpi_ping_pong.o
	$(MPICC) $(CFLAGS) $^ $(LIBS) -o $@ 

%.o : %.c
	$(MPICC) $(CFLAGS) -c $< 

all : $(EXE)

clean:
	rm -f $(OBJECTS) $(EXE) *~

help:
	@echo "Valid targets:"
	@echo "  clean:  removes .o and .exe files"
