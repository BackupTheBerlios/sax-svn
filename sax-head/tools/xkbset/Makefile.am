bin_PROGRAMS = xkbset
xkbset_SOURCES = \
	xkbset.c \
	string.c \
	../whois/remote.c \
	../whois/ifaddrs.c

xkbset_LDFLAGS = \
	-L/usr/X11R6/lib -L/usr/X11R6/lib64 -lX11 -lXext

xkbset_LDADD = -lXxf86misc

INCLUDES = -I../whois
