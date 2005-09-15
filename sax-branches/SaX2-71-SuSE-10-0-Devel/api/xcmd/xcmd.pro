TEMPLATE  = app
SOURCES   += xcmd.cpp
SOURCES   += color.cpp
SOURCES   += glinfo.cpp
SOURCES   += ../common/log.cpp

HEADERS   += xcmd.h
HEADERS   += color.h
HEADERS   += glinfo.h
HEADERS   += ../common/log.h

CONFIG    += qt warn_on release
TARGET    = xcmd

QMAKE_CXXFLAGS = -fno-strict-aliasing
unix:INCLUDEPATH += -I /usr/X11R6/include
unix:LIBS += -L/usr/X11R6/lib -L../../libsax -lsax
