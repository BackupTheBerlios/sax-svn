all:
	swig -Wall -c++ -java SaX.i

clean:
	rm -f SaX*.java SaX_* SaX.so *.class
	rm -f SWIGTYPE*
