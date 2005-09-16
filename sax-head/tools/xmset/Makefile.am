bin_PROGRAMS = xmset
xmset_SOURCES = \
	mouse.c \
	monitor.c \
	xmset.c \
	string.c \
	../whereiam/remote.c \
	../whereiam/ifaddrs.c

xmset_LDFLAGS = \
	-L/usr/X11R6/lib -L/usr/X11R6/lib64 -lX11 -lXext
xmset_CFLAGS = -D`./check_wheel.sh` -D`./check_ddc.sh`

xmset_LDADD = -lXxf86misc -lhd

INCLUDES = -I../whereiam
