CC = gcc
CLIBS = -lm -lconfigini -lgsl -lgslcblas
CFLAGS = -Wall -std=gnu99 -pedantic

lattice2DLJ: lattice2DLJ.c
	$(CC) $(CFLAGS) lattice2DLJ.c -o lattice2DLJ $(CLIBS)