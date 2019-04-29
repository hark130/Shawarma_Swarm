CC = gcc
HLIB = ../Harkle_Library/

shwarm:
	# Placeholder...
	# $(CC) -c $(HLIB)Harklecurse.c -o Harklecurse.o
	$(CC) -c shwarm_it.c
	$(CC) -o shwarm_it.exe shwarm_it.o

all:
	$(MAKE) shwarm

clean: 
	rm -f *.o *.exe *.so
