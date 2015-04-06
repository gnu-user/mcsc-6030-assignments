MPICC = mpicc
CFLAGS = -Wall -O2 -march=native -std=c99
LFLAGS = 
LIBS = -lblas -lcblas -lm
SOURCES = matvec.c
OBJECTS = $(subst .c,.o,$(SOURCES))
EXE = matvec.exe
.PHONY: clean help

matvec.exe : matvec.o
	$(MPICC) $(CFLAGS) $^ $(LIBS) -o $@ 

%.o : %.c
	$(MPICC) $(CFLAGS) -c $< 

all : $(EXE)

clean:
	rm -f $(OBJECTS) $(EXE) *~

help:
	@echo "Valid targets:"
	@echo "  clean:  removes .o and .exe files"
