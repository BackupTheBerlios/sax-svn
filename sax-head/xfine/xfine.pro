TEMPLATE  += app

SOURCES   += frame.cpp
SOURCES   += xfine.cpp
SOURCES   += xquery.cpp
SOURCES   += file.cpp
SOURCES   += ../api/common/log.cpp
SOURCES   += ../api/common/xlock.cpp
SOURCES   += ../api/common/qx.cpp

HEADERS   += frame.h
HEADERS   += xfine.h
HEADERS   += xquery.h
HEADERS   += file.h
HEADERS   += ../api/common/log.h
HEADERS   += ../api/common/xlock.h
HEADERS   += ../api/common/qx.h

CONFIG    += thread warn_on release
unix:LIBS += -lm -lXss -lXinerama -lGL -lXxf86vm
TARGET    += xfine
