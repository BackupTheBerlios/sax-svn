CC = g++
OPT=-O2 -fomit-frame-pointer -fpic
CFLAGS=$(OPT) -Wall -Wno-parentheses 

VERSION=2.1.8
DATE=2000-05-20

export

all: lib lspci.o common.o

lib: lib/config.h
	$(MAKE) -C lib all

lib/config.h:
	cd lib && ./configure $(PREFIX) $(VERSION)

lspci.o: lspci.c pciutils.h lib/libpci.a
common.o: common.c pciutils.h lib/libpci.a

clean:
	rm -f *.o
	rm -f lib/config.* lib/*.o

