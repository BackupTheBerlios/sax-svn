all:
	swig -Wall -c++ -python SaX.i
	g++ -c -I/usr/X11R6/include -I/usr/lib/qt3/include \
		-D_REENTRANT -D_GNU_SOURCE -DTHREADS_HAVE_PIDS \
		-fno-strict-aliasing -pipe -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 \
		-I/usr/include/python -O2 -fPIC SaX_wrap.cxx
	g++ -shared SaX_wrap.o -o _SaX.so -L/usr/X11R6/lib -L/usr/lib/qt3/lib \
		-L../../ -lm -lxf86config -lqt-mt -lsax -lxkbfile

clean:
	rm -f  SaX_wrap.cxx
	rm -f  Makefile.old
	rm -f  SaX.py
	rm -f  SaX_wrap.o
	rm -f  SaX.pyc
	rm -f  _SaX.so
