TEMPLATE  += app

SOURCES   += frame.cpp
SOURCES   += xapi.cpp
SOURCES   += file.cpp
SOURCES   += dialog.cpp
SOURCES   += mouse.cpp
SOURCES   += xidle.cpp
SOURCES   += mouse/mousedisplay.cpp
SOURCES   += mouse/mousetest.cpp
SOURCES   += mouse/mouseevent.cpp
SOURCES   += mouse/mousemodel.cpp
SOURCES   += keyboard.cpp
SOURCES   += keyboard/keyboardlayout.cpp
SOURCES   += keyboard/keyboardoptions.cpp
SOURCES   += vnc.cpp
SOURCES   += vnc/vncdisplay.cpp
SOURCES   += tablet.cpp
SOURCES   += tablet/tabletselection.cpp
SOURCES   += tablet/tabletconnection.cpp
SOURCES   += tablet/tabletpens.cpp
SOURCES   += tablet/tabletpenproperty.cpp
SOURCES   += touchscreen.cpp
SOURCES   += touchscreen/touchselection.cpp
SOURCES   += monitor.cpp
SOURCES   += monitor/monitorfig.cpp
SOURCES   += monitor/monitorplot.cpp
SOURCES   += monitor/monitorarrange.cpp
SOURCES   += monitor/monitorcard.cpp
SOURCES   += monitor/monitordisplay.cpp
SOURCES   += monitor/monitordual.cpp
SOURCES   += monitor/monitordualmodel.cpp
SOURCES   += monitor/monitormodel.cpp
SOURCES   += message.cpp
SOURCES   += common/log.cpp

HEADERS   += frame.h
HEADERS   += xapi.h
HEADERS   += file.h
HEADERS   += dialog.h
HEADERS   += mouse.h
HEADERS   += xidle.h
HEADERS   += mouse/mousedisplay.h
HEADERS   += mouse/mousetest.h
HEADERS   += mouse/mouseevent.h
HEADERS   += mouse/mousemodel.h
HEADERS   += keyboard.h
HEADERS   += keyboard/keyboardlayout.h
HEADERS   += keyboard/keyboardoptions.h
HEADERS   += vnc.h
HEADERS   += vnc/vncdisplay.h
HEADERS   += tablet.h
HEADERS   += tablet/tabletselection.h
HEADERS   += tablet/tabletconnection.h
HEADERS   += tablet/tabletpens.h
HEADERS   += tablet/tabletpenproperty.h
HEADERS   += touchscreen.h
HEADERS   += touchscreen/touchselection.h
HEADERS   += monitor.h
HEADERS   += monitor/monitorfig.h
HEADERS   += monitor/monitorplot.h
HEADERS   += monitor/monitorarrange.h
HEADERS   += monitor/monitorcard.h
HEADERS   += monitor/monitordisplay.h
HEADERS   += monitor/monitordual.h
HEADERS   += monitor/monitordualmodel.h
HEADERS   += monitor/monitormodel.h
HEADERS   += message.h
HEADERS   += common/log.h

CONFIG    += thread warn_on release
unix:LIBS += -lm -lXss -lxkbfile -lcrypt
TARGET    += xapi

QMAKE_CXXFLAGS = -fno-strict-aliasing
unix:INCLUDEPATH += -I /usr/X11R6/include
unix:LIBS += -L/usr/X11R6/lib -L../libsax -lsax
