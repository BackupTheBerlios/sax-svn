CC  = g++ -Wall -L/usr/X11R6/lib -L/usr/X11R6/lib64 `../tools/check_loader.sh`
LIB = ./lib/libsysp.a -lfl -lhd -lstdc++ -lX11

OBJ = sysp.o mouse.o keyboard.o server.o cfg.o xstuff.o d3.o
SYS = ./lib

all: sysp

sysp: ${OBJ}
	${MAKE} -C ${SYS} all
	${CC} -o sysp \
		${OBJ} \
		${LIB}

d3.o: d3.c d3.h
	${CC} -c d3.c

xstuff.o: xstuff.c xstuff.h sysp.h
	${CC} -c xstuff.c

cfg.o: cfg.c cfg.h sysp.h
	${CC} -c cfg.c

server.o: server.c server.h sysp.h
	${CC} -c server.c

keyboard.o: keyboard.c keyboard.h sysp.h
	${CC} -c keyboard.c

sysp.o: sysp.c sysp.h
	${CC} -c sysp.c

mouse.o: mouse.c mouse.h sysp.h
	${CC} -c mouse.c

clean:
	rm -f *.o
	${MAKE} -C ${SYS} clean

