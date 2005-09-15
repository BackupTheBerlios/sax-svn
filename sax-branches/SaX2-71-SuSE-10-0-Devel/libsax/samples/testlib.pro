TEMPLATE  = app
#SOURCES   += exception.cpp
SOURCES   += testlib.cpp
#HEADERS   += exception.h
CONFIG    += qt warn_on release
TARGET    = testlib.bin
unix:LIBS += -L.. -lsax
unix:INCLUDEPATH += -I /usr/X11R6/include
