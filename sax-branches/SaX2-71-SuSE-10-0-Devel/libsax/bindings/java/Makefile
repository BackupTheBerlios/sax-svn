DESTDIR = /
BINDLIB = lib
PLUGSDIR=${DESTDIR}/usr/${BINDLIB}/sax/plugins
SHAREDIR=${DESTDIR}/usr/share/java
JAVAC=${JAVA_BINDIR}/javac

all:
	if [ ! -f ${JAVAC} ];then \
		exit 1 ;\
	fi
	g++ -c \
		-I/usr/X11R6/include \
		-I/usr/${BINDLIB}/qt3/include \
		-I/usr/lib/jvm/java/include \
		-I/usr/lib/jvm/java/include/linux\
		-fno-strict-aliasing -pipe -O2 -fPIC SaX_wrap.cxx
	g++ -shared SaX_wrap.o -o SaX.so \
		-L/usr/X11R6/${BINDLIB} -L/usr/${BINDLIB}/qt3/${BINDLIB} \
		-L../../ -lm -lxf86config -lqt-mt -lsax -lxkbfile
	${JAVAC} SaX*.java SWIGTYPE*
	jar cf SaX.jar *.class && rm -f *.class SaX[^_]*.java SWIGTYPE*

install_vendor:
	if [ ! -f ./SaX.jar ];then \
		exit 1 ;\
	fi
	install -d -m 755 ${PLUGSDIR}
	install -d -m 755 ${SHAREDIR}
	install -m 755 ./SaX.so  ${PLUGSDIR}
	install -m 644 ./SaX.jar ${SHAREDIR}

clean:
	rm -f SaX[^_]*.java SaX.so *.class SaX.java
	rm -f SWIGTYPE* SaX.jar SaX_wrap*
