all:
	swig -Wall -c++ -csharp SaX.i

clean:
	rm -f SaX*.cs SaX_* libSaX.so
	rm -f SWIGTYPE* *.exe SaX.dll
