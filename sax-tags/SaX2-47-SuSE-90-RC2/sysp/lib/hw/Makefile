#/.../
# Copyright (c) 2001 SuSE GmbH Nuernberg, Germany.  All rights reserved.
# Author: Marcus Schaefer <sax@suse.de>, 2001
#
# Makefile to build sysp libhw part
# --
# Status: Up-to-date
#
WHEEL  = `./check_wheel.sh`
DDC    = `./check_ddc.sh`
CC     = g++ -Wall -fpic -O2 -D${WHEEL} -D${DDC}
OBJ    = mouse.o tv.o keyboard.o monitor.o memory.o
CFLAGS = -Wall -O2

all: ${OBJ}

mouse.o:mouse.c hwdata.h
	${CC} ${CFLAGS} -c mouse.c

keyboard.o:keyboard.c hwdata.h
	${CC} ${CFLAGS} -c keyboard.c

monitor.o:monitor.c hwdata.h
	${CC} ${CFLAGS} -c monitor.c

memory.o:memory.c hwdata.h
	${CC} ${CFLAGS} -c memory.c

tv.o:tv.c hwdata.h
	${CC} ${CFLAGS} -c tv.c

clean:
	rm -f *.o
