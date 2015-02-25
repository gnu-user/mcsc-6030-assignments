CC = gcc
CFLAGS = -std=c99
LFLAGS = 
LIBS = -lm
OBJECTS = quadrature.o
EXE = test1.exe test2.exe
.PHONY: clean help

%.exe : %.o $(OBJECTS)
	$(CC) $(OBJECTS) $< $(LIBS) -o $@ 

%.o : %.c
	$(CC) $(CFLAGS) -c $< 

all : $(EXE)

clean:
	rm -f $(OBJECTS) $(EXE) *~

help:
	@echo "Valid targets:"
	@echo "  test1.exe"
	@echo "  test2.exe"
	@echo "  all"
	@echo "  clean:  removes .o and .exe files"
