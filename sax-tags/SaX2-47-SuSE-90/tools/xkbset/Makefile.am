bin_PROGRAMS = xkbset
xkbset_SOURCES = \
	xkbset.c \
	string.c \
	../whois/remote.c \
	../whois/ifaddrs.c

xkbset_LDFLAGS = \
	-L/usr/X11R6/lib -L/usr/X11R6/lib64 -lX11 -lXext -lXxf86misc

INCLUDES = -I../whois

xkbctrl:xkbctrl.c
	gcc xkbctrl.c -o xkbctrl \
		`./.static` -ldl -lcrypt -lm -lpthread

xkbctrl.c:xkbctrl.pl
	perlcc -c xkbctrl.pl
