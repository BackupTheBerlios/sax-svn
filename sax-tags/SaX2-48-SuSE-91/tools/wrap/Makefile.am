bin_PROGRAMS = catch fake wrap xidle xlook
catch_SOURCES = \
	catch.c
fake_SOURCES = \
	fake.c
wrap_SOURCES = \
	wrap.c
xidle_SOURCES = \
	xidle.c
xlook_SOURCES = \
	xlook.c xlook.h

catch_LDFLAGS = \
	-L/usr/X11R6/lib -L/usr/X11R6/lib64 -lX11 -lXxf86misc -lXext -lm

fake_LDFLAGS = \
	-L/usr/X11R6/lib -L/usr/X11R6/lib64 -lX11 -lXxf86misc -lXext -lXtst -lm

wrap_LDFLAGS = \
	-L/usr/X11R6/lib -L/usr/X11R6/lib64 -lX11 -lXxf86misc -lXext -lm

xidle_LDFLAGS = \
	-L/usr/X11R6/lib -L/usr/X11R6/lib64 -lX11 -lXxf86misc -lXext -lm
xidle_LDADD = \
	-lXss

xlook_CFLAGS  = -DRENDER `/usr/X11R6/bin/xft-config --cflags`
xlook_LDFLAGS = \
	-L/usr/X11R6/lib -L/usr/X11R6/lib64 \
	-lX11 -lXxf86misc -lXext -lm -lXft
