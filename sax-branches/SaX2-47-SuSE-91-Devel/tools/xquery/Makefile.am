bin_PROGRAMS = xquery
xquery_SOURCES = \
	render.c \
	nstring.c \
	xquery.c

xquery_LDFLAGS = \
	-L/usr/X11R6/lib -L/usr/X11R6/lib64 \
	-lX11 -lXext -lXft

xquery_CFLAGS = `/usr/X11R6/bin/xft-config --cflags`
xquery_LDADD  = -lXinerama -lXxf86vm
