TEMPLATE  = app
CONFIG    = qt warn_on release
HEADERS   = xplot.h xfig.h
SOURCES   = xplot.cpp main.cpp xfig.cpp
unix:LIBS = -lm
TARGET    = xplot
