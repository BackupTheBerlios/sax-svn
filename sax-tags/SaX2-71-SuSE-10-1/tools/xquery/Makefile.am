sbin_PROGRAMS = xquery

WARN_CFLAGS = @WARN_CFLAGS@

xquery_SOURCES = \
	render.c \
	nstring.c \
	xquery.c

xquery_LDFLAGS = \
	-L/usr/X11R6/lib -L/usr/X11R6/lib64 \
	-lX11 -lXext -lXft

xquery_CFLAGS = `/usr/X11R6/bin/xft-config --cflags`
xquery_CFLAGS+= $(WARN_CFLAGS)

xquery_LDADD  = -lXinerama -lXxf86vm
