bin_PROGRAMS = xlook
xlook_SOURCES = \
	xlook.c xlook.h

xlook_CFLAGS  = -DRENDER `/usr/X11R6/bin/xft-config --cflags`
xlook_LDFLAGS = \
	-L/usr/X11R6/lib -L/usr/X11R6/lib64 \
	-lX11 -lXxf86misc -lXext -lm -lXft
