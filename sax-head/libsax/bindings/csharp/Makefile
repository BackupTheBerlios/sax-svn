all:
	g++ -c -I/usr/X11R6/include -I/usr/lib/qt3/include \
		-fno-strict-aliasing -pipe -fPIC SaX_wrap.cxx
	g++ -shared SaX_wrap.o -o libSaX.so -L/usr/X11R6/lib -L/usr/lib/qt3/lib \
		-L../../ -lm -lxf86config -lqt-mt -lsax -lxkbfile
	mcs -nologo -o SaX.dll -t:library SaX*.cs SWIGTYPE* && \
		rm -f SaX*.cs SWIGTYPE*

install_vendor:

clean:
	rm -f SaX*.cs SaX_* libSaX.so
	rm -f SWIGTYPE* *.exe SaX.dll
