all:
	g++ -c -I/usr/X11R6/include \
		-I/usr/lib/qt3/include -I/usr/include/python \
		-O2 -fPIC -fno-strict-aliasing -pipe SaX_wrap.cxx
	g++ -shared SaX_wrap.o -o _SaX.so -L/usr/X11R6/lib -L/usr/lib/qt3/lib \
		-L../../ -lm -lxf86config -lqt-mt -lsax -lxkbfile

install_vendor:

clean:
	rm -f  SaX_wrap.cxx
	rm -f  Makefile.old
	rm -f  SaX.py
	rm -f  SaX_wrap.o
	rm -f  SaX.pyc
	rm -f  _SaX.so
