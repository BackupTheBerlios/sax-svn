TEMPLATE  = app
#SOURCES   += exception.cpp
SOURCES   += testlib.cpp
#HEADERS   += exception.h
CONFIG    += qt warn_on release
TARGET    = testlib.bin
unix:LIBS += -L.. -lsax
unix:INCLUDEPATH += -I/usr/include/X11
