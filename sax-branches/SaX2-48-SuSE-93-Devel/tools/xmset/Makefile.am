bin_PROGRAMS = xmset
xmset_SOURCES = \
	mouse.c \
	monitor.c \
	xmset.c \
	string.c \
	../whois/remote.c \
	../whois/ifaddrs.c

xmset_LDFLAGS = \
	-L/usr/X11R6/lib -L/usr/X11R6/lib64 -lX11 -lXext
xmset_CFLAGS = -D`./check_wheel.sh` -D`./check_ddc.sh`

xmset_LDADD = -lXxf86misc -lhd

xupdate:xupdate.c
	gcc xupdate.c -o xupdate \
	`./.static` -ldl -lcrypt -lm -lpthread

xupdate.c:xupdate.pl
	perlcc -c xupdate.pl

INCLUDES = -I../whois
