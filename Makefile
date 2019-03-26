CC=gcc
CFLAGS=-std=gnu99 -pedantic-errors

all:
	$(CC) filecnt.c -o filecnt $(CFLAGS)
	$(CC) filedisp.c -o filedisp $(CFLAGS)
