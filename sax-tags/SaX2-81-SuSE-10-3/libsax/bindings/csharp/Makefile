DESTDIR = /
BINDLIB = lib
PLUGSDIR=${DESTDIR}/usr/${BINDLIB}
MONODLLD=${DESTDIR}/usr/${BINDLIB}/mono/gac/SaX

RPM_OPT_FLAGS ?= -O2
CFLAGS = $(RPM_OPT_FLAGS)

all:/usr/bin/mcs
	g++ -c $(CFLAGS) -I/usr/X11R6/include \
		-I/usr/include/xorg -I${QTDIR}/include \
		-fno-strict-aliasing -pipe -fPIC SaX_wrap.cxx
	g++ -shared SaX_wrap.o -o libSaX.so \
		-L/usr/X11R6/${BINDLIB} -L${QTDIR}/${BINDLIB} \
		-L../../ -lm -lxf86config -lqt-mt -lsax -lxkbfile
	mcs -nologo -debug -out:SaX.dll -t:library SaX*.cs SWIGTYPE* && \
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
