TEMPLATE  = lib
VERSION   = 7.3
SOURCES   += storage.cpp
SOURCES   += import.cpp
SOURCES   += process.cpp
SOURCES   += export.cpp
SOURCES   += config.cpp
SOURCES   += exception.cpp
SOURCES   += init.cpp
SOURCES   += desktop.cpp
SOURCES   += extensions.cpp
SOURCES   += card.cpp
SOURCES   += keyboard.cpp
SOURCES   += path.cpp
SOURCES   += pointers.cpp
SOURCES   += layout.cpp
SOURCES   += device.cpp
SOURCES   += file.cpp
SOURCES   += interface.cpp
SOURCES   += processcall.cpp

HEADERS   += storage.h
HEADERS   += import.h
HEADERS   += process.h
HEADERS   += export.h
HEADERS   += config.h
HEADERS   += exception.h
HEADERS   += sax.h
HEADERS   += init.h
HEADERS   += desktop.h
HEADERS   += extensions.h
HEADERS   += card.h
HEADERS   += keyboard.h
HEADERS   += path.h
HEADERS   += pointers.h
HEADERS   += layout.h
HEADERS   += device.h
HEADERS   += file.h
HEADERS   += interface.h
HEADERS   += processcall.h

RPM_OPT_FLAGS ?= -O2

QMAKE_CXXFLAGS += $(RPM_OPT_FLAGS) -rdynamic -DPIC
CONFIG    += qt warn_on release
unix:LIBS += -L/usr/X11R6/lib64 -L/usr/X11R6/lib -lxkbfile -lxf86config
unix:INCLUDEPATH += -iquote /usr/X11R6/include -iquote /usr/include/xorg -I.
unix:INCLUDEPATH += /usr/include/X11
