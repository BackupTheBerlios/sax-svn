TEMPLATE  += app

SOURCES   += rules.cpp
SOURCES   += xkb.cpp

HEADERS   += rules.h

CONFIG    += thread warn_on release
unix:LIBS += -lm -lxkbfile
TARGET    += xkb
