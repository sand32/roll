CC=gcc
OUTNAME=roll

make:
	$(CC) -O2 -o $(OUTNAME) main.c

install:
	cp $(OUTNAME) /usr/local/bin
