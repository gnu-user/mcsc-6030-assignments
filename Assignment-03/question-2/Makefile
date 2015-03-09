CC = gcc
CFLAGS = -std=c99
LFLAGS = 
LIBS = -lgsl -lgslcblas -lm
SOURCES = main.c function.c 
OBJECTS = $(subst .c,.o,$(SOURCES))
.PHONY: clean help

output.txt: main.exe
	./$< > $@

main.exe: $(OBJECTS)
	$(CC) $(CFLAGS) $(LFLAGS) $(OBJECTS) $(LIBS) -o $@

%.o : %.c
	$(CC) $(CFLAGS) -c $< 

clean:
	rm -f $(OBJECTS) main.exe

clobber: clean
	rm -f output.txt

help:
	@echo "Valid targets:"
	@echo "  output.txt: runs main.exe, generates output.txt"
	@echo "  clean:  removes .o and .exe files"
	@echo "  clobber:  removes .o, .exe, and output files"
