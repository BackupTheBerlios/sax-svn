JAVAC=/usr/lib/jvm/java-1.4.2-sun/bin/javac

all:
	g++ -c \
		-I/usr/X11R6/include \
		-I/usr/lib/qt3/include \
		-I/usr/lib/jvm/java/include \
		-I/usr/lib/jvm/java/include/linux\
		-fno-strict-aliasing -pipe -O2 -fPIC SaX_wrap.cxx
	g++ -shared SaX_wrap.o -o SaX.so -L/usr/X11R6/lib -L/usr/lib/qt3/lib \
		-L/suse/ms/libsax -lm -lxf86config -lqt-mt -lsax -lxkbfile
	${JAVAC} *.java

clean:
	rm -f SaX*.java SaX_* SaX.so *.class
	rm -f SWIGTYPE*
