JAVAC=/usr/lib/jvm/java-1.4.2-sun/bin/javac

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

clean:
	rm -f SaX[^_]*.java SaX.so *.class SaX.java
	rm -f SWIGTYPE* SaX.jar SaX_wrap*
