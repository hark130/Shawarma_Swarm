CC = gcc
HL_DIR = ../Harkle_Library/
HL_HDR = ../Harkle_Library/hdr/
HL_SRC = ../Harkle_Library/src/
HL_BLD = ../Harkle_Library/build/

shwarm:
	make -C $(HL_DIR) Harklecurse
	make -C $(HL_DIR) Harklemath
	make -C $(HL_DIR) Randoroad
	$(CC) -I $(HL_HDR) -c shwarm_it.c
	$(CC) -I $(HL_HDR) -c Harkleswarm.c
	$(CC) -o shwarm_it.exe $(HL_BLD)Harklecurse.o $(HL_BLD)Harklemath.o $(HL_BLD)Randoroad.o Harkleswarm.o shwarm_it.o -lncurses -lm

all:
	$(MAKE) shwarm

clean: 
	rm -f *.o *.exe *.so
