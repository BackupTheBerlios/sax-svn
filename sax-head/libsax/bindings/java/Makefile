DESTDIR = /
PLUGSDIR=${DESTDIR}/usr/lib/sax/plugins
SHAREDIR=${DESTDIR}/usr/share/java
JAVAC=/usr/lib/jvm/java-*-sun/bin/javac

all:
	g++ -c \
		-I/usr/X11R6/include \
		-I/usr/lib/qt3/include \
		-I/usr/lib/jvm/java/include \
		-I/usr/lib/jvm/java/include/linux\
		-fno-strict-aliasing -pipe -O2 -fPIC SaX_wrap.cxx
	g++ -shared SaX_wrap.o -o SaX.so -L/usr/X11R6/lib -L/usr/lib/qt3/lib \
		-L../../ -lm -lxf86config -lqt-mt -lsax -lxkbfile
	${JAVAC} SaX*.java SWIGTYPE*
	jar cf SaX.jar *.class && rm -f *.class SaX[^_]*.java SWIGTYPE*

install_vendor:
	install -d -m 755 ${PLUGSDIR}
	install -d -m 755 ${SHAREDIR}
	install -m 755 ./SaX.so  ${PLUGSDIR}
	install -m 644 ./SaX.jar ${SHAREDIR}

clean:
	rm -f SaX[^_]*.java SaX.so *.class SaX.java
	rm -f SWIGTYPE* SaX.jar SaX_wrap*
