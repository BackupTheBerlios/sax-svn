all:
	g++ -c -I/usr/X11R6/include -I/usr/lib/qt3/include \
		-D_REENTRANT -D_GNU_SOURCE -DTHREADS_HAVE_PIDS \
		-fno-strict-aliasing -pipe -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 \
		-O2 -fPIC SaX_wrap.cxx
	g++ -shared SaX_wrap.o -o libSaX.so -L/usr/X11R6/lib -L/usr/lib/qt3/lib \
		-L../../ -lm -lxf86config -lqt-mt -lsax -lxkbfile
	mcs -nologo -o SaX.dll -t:library SaX*.cs SWIGTYPE* && \
		rm -f SaX*.cs SWIGTYPE*

install_vendor:

clean:
	rm -f SaX*.cs SaX_* libSaX.so
	rm -f SWIGTYPE* *.exe SaX.dll
