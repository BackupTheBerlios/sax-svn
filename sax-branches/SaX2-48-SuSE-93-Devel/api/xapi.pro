TEMPLATE  += app

SOURCES   += frame.cpp
SOURCES   += xapi.cpp
SOURCES   += template.cpp
SOURCES   += file.cpp
SOURCES   += xbox.cpp
SOURCES   += xtab.cpp
SOURCES   += xprogress.cpp
SOURCES   += xinfo.cpp
SOURCES   += xidle.cpp
SOURCES   += data.cpp
SOURCES   += common/log.cpp
SOURCES   += common/xlock.cpp
SOURCES   += common/qx.cpp
SOURCES   += dialog/intro.cpp
SOURCES   += dialog/virtual/virtual.cpp
SOURCES   += dialog/virtual/virtualhandler.cpp
SOURCES   += dialog/virtual/xvirtual.cpp
SOURCES   += dialog/opengl/opengl.cpp
SOURCES   += dialog/opengl/openglhandler.cpp
SOURCES   += dialog/accessx/accessx.cpp
SOURCES   += dialog/accessx/accessxhandler.cpp
SOURCES   += dialog/desktop/desktop.cpp
SOURCES   += dialog/desktop/desktophandler.cpp
SOURCES   += dialog/layout/layout.cpp
SOURCES   += dialog/layout/layouthandler.cpp
SOURCES   += dialog/layout/xfig.cpp
SOURCES   += dialog/layout/xplot.cpp
SOURCES   += dialog/mouse/mouse.cpp
SOURCES   += dialog/mouse/mousehandler.cpp
SOURCES   += dialog/mouse/xevent.cpp
SOURCES   += dialog/mouse/xmouse.cpp
SOURCES   += dialog/tablet/tablet.cpp
SOURCES   += dialog/tablet/tablethandler.cpp
SOURCES   += dialog/card/card.cpp
SOURCES   += dialog/card/cardhandler.cpp
SOURCES   += dialog/geometry/geometry.cpp
SOURCES   += dialog/geometry/geometryhandler.cpp
SOURCES   += dialog/keyboard/keyboard.cpp
SOURCES   += dialog/keyboard/keyboardhandler.cpp
SOURCES   += dialog/keyboard/rules.cpp
SOURCES   += dialog/monitor/monitor.cpp
SOURCES   += dialog/monitor/monitorhandler.cpp
SOURCES   += dialog/multihead/multihead.cpp
SOURCES   += dialog/multihead/multiheadhandler.cpp
SOURCES   += dialog/touchscreen/touchscreen.cpp
SOURCES   += dialog/touchscreen/touchhandler.cpp
SOURCES   += dialog/vnc/vnc.cpp
SOURCES   += dialog/vnc/vnchandler.cpp
SOURCES   += xquery.cpp

HEADERS   += frame.h
HEADERS   += template.h
HEADERS   += file.h
HEADERS   += xinfo.h
HEADERS   += xidle.h
HEADERS   += data.h
HEADERS   += xbox.h
HEADERS   += xtab.h
HEADERS   += xprogress.h
HEADERS   += common/log.h
HEADERS   += common/xlock.h
HEADERS   += common/qx.h
HEADERS   += dialog/intro.h
HEADERS   += dialog/virtual/virtual.h
HEADERS   += dialog/virtual/xvirtual.h
HEADERS   += dialog/opengl/opengl.h
HEADERS   += dialog/accessx/accessx.h
HEADERS   += dialog/desktop/desktop.h
HEADERS   += dialog/layout/layout.h
HEADERS   += dialog/layout/xfig.h
HEADERS   += dialog/layout/xplot.h
HEADERS   += dialog/mouse/mouse.h
HEADERS   += dialog/mouse/xevent.h
HEADERS   += dialog/mouse/xmouse.h
HEADERS   += dialog/tablet/tablet.h
HEADERS   += dialog/card/card.h
HEADERS   += dialog/geometry/geometry.h
HEADERS   += dialog/keyboard/keyboard.h
HEADERS   += dialog/keyboard/rules.h
HEADERS   += dialog/monitor/monitor.h
HEADERS   += dialog/multihead/multihead.h
HEADERS   += dialog/touchscreen/touchscreen.h
HEADERS   += dialog/vnc/vnc.h
HEADERS   += xquery.h

CONFIG    += thread warn_on release
unix:LIBS += -lm -lXss -lxkbfile -lcrypt
TARGET    += xapi

QMAKE_CXXFLAGS  += `../tools/check_loader.sh`
