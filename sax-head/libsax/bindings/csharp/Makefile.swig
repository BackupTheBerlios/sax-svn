all:
	swig -Wall -c++ -csharp SaX.i

clean:
	rm -f SaX*.cs SaX_* SaX.so
	rm -f SWIGTYPE*
