bin_PROGRAMS = xw
xw_SOURCES = \
	xw.c \
	xaccess.c \
	fashion.c

xw_LDFLAGS = \
	-L/usr/X11R6/lib -L/usr/X11R6/lib64 \
	-lX11 -lXext -lXmu -lm -lXft

xw_CFLAGS = `/usr/X11R6/bin/xft-config --cflags`
xw_LDADD = -lXinerama
