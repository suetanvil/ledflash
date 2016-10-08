
# Target platform.  Leave both unset for testing on LED-less systems.
PLATFORM=-DPLATFORM_RASPBIAN
#PLATFORM=-DPLATFORM_POGOPLUG

# We compile with -g because a stripped debug binary turns out to be
# smaller than an optimized one and size is what we care about here.
CC=gcc
CFLAGS=-g -Wall $(PLATFORM)

LINK=gcc
LFLAGS=

PREFIX=/usr/local

#######################

.SUFFIXES: .pod .8

all: ledflash ledflash.8

clean:
	-rm *.o ledflash ledflash.8

install: ledflash ledflash.8
	strip ledflash
	-rm $(PREFIX)/bin/ledflash
	install -o root -D ledflash $(PREFIX)/bin/ledflash
	install -o root -D ledflash.8 $(PREFIX)/share/man/man8/ledflash.8

ledflash : ledflash.o
	$(LINK) $(LFLAGS) ledflash.o -o ledflash

ledflash.o : ledflash.c
	$(CC) -c $(CFLAGS) ledflash.c

ledflash.8 : ledflash.pod
	pod2man -s 1 --release "" ledflash.pod ledflash.8

