TEMPLATE  = app
SOURCES   += capi.cpp
SOURCES   += color.cpp
SOURCES   += ../api/common/log.cpp

HEADERS   += capi.h
HEADERS   += color.h
HEADERS   += ../api/common/log.h

CONFIG    += qt warn_on release
TARGET    = capi
unix:LIBS += -L../libsax -lsax
unix:INCLUDEPATH += -I /usr/X11R6/include
