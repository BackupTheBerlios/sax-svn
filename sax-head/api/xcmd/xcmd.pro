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
unix:LIBS += -L../../libsax -lsax
unix:INCLUDEPATH += -I /usr/X11R6/include
