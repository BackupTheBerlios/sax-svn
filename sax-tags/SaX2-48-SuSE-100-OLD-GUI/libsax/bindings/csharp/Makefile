DESTDIR = /
PLUGSDIR=${DESTDIR}/usr/lib
MONODLLD=${DESTDIR}/usr/lib/mono/gac/SaX

all:
	g++ -c -I/usr/X11R6/include -I/usr/lib/qt3/include \
		-fno-strict-aliasing -pipe -fPIC SaX_wrap.cxx
	g++ -shared SaX_wrap.o -o libSaX.so -L/usr/X11R6/lib -L/usr/lib/qt3/lib \
		-L../../ -lm -lxf86config -lqt-mt -lsax -lxkbfile
	mcs -nologo -g -o SaX.dll -t:library SaX*.cs SWIGTYPE* && \
		rm -f SaX*.cs SWIGTYPE*

install_vendor:
	install -d -m 755 ${PLUGSDIR}
	install -d -m 755 ${MONODLLD}
	install -m 755 ./libSaX.so   ${PLUGSDIR}
	install -m 755 ./SaX.dll     ${MONODLLD}
	install -m 644 ./SaX.dll.mdb ${MONODLLD}

clean:
	rm -f SaX*.cs SaX_* libSaX.so SaX.dll.mdb
	rm -f SWIGTYPE* *.exe SaX.dll*
