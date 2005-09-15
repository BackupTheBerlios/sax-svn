DESTDIR = /
BINDLIB = lib
PYTHON  = `readlink /usr/${BINDLIB}/python`
PACKDIR = ${DESTDIR}/usr/${BINDLIB}/${PYTHON}/site-packages
MODDIR  = ${PACKDIR}/SaX

export LD_LIBRARY_PATH=../../

all:
	g++ -c -I/usr/X11R6/include \
		-I/usr/${BINDLIB}/qt3/include -I/usr/include/python \
		-O2 -fPIC -fno-strict-aliasing -pipe SaX_wrap.cxx
	g++ -shared SaX_wrap.o -o _SaX.so \
		-L/usr/X11R6/${BINDLIB} -L/usr/${BINDLIB}/qt3/${BINDLIB} \
		-L../../ -lm -lxf86config -lqt-mt -lsax -lxkbfile
	python -c "import SaX"
	python -O -c "import SaX"

install_vendor:
	install -d -m 755      ${MODDIR}
	install -m 755 _SaX.so ${MODDIR}
	install -m 644 SaX.py  ${MODDIR}
	install -m 644 SaX.pyc ${MODDIR}
	install -m 644 SaX.pyo ${MODDIR}
	install -m 644 SaX.pth ${PACKDIR}

clean:
	rm -f  SaX_wrap.cxx
	rm -f  Makefile.old
	rm -f  SaX.py
	rm -f  SaX_wrap.o
	rm -f  SaX.pyc SaX.pyo
	rm -f  _SaX.so
