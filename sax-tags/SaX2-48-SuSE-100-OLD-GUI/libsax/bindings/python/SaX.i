//==================================
// Interface definition for libsax
//----------------------------------
#define NO_OPERATOR_SUPPORT 1
%module SaX
%{
#include "../../sax.h"
%}

//==================================
// SWIG includes
//----------------------------------
%include exception.i

//==================================
// Typemaps
//----------------------------------
//==================================
// Allow QString return types
//----------------------------------
%typemap(out) QString {
	$result = PyString_FromStringAndSize($1.data(),$1.length());
}
//==================================
// Allow QString refs as parameters
//----------------------------------
%typemap(in) QString&,QString& (QString temp) {
	if (PyString_Check($input)) {
		temp = QString (PyString_AsString($input));
		$1 = &temp;
	} else {
		SWIG_exception(SWIG_TypeError, "QString& expected");
	}
}
//==================================
// Allow QDict<QString> return types
//----------------------------------
%typemap(out) QDict<QString> {
	$result = PyDict_New();
	QDictIterator<QString> it ($1);
	for (; it.current(); ++it) {
		QString* key = new QString (it.currentKey());
		QString* val = new QString (*it.current());
		PyDict_SetItemString (
			$result, (char*)key->ascii(),
			PyString_FromString((char*)val->ascii())
		);
	}
}
//==================================
// Allow QList<QString> return types
//----------------------------------
%typemap(out) QList<QString> {
	$result = PyList_New($1.count());
	QListIterator<QString> it ($1);
	for (; it.current(); ++it) {
		PyList_Append(
			$result,PyString_FromString((char*)it.current()->ascii())
		);
	}
}
//==================================
// Allow QStringList return types
//----------------------------------
%typemap(out) QStringList {
	$result = PyList_New($1.count());
	for ( QStringList::Iterator it=$1.begin(); it != $1.end(); ++it ) {
		QString item (*it);
		PyList_Append ($result, PyString_FromString((char*)item.ascii()));
	}
}

//==================================
// Exception class wrapper...
//----------------------------------
class SaXException {
	public:
	int   errorCode          ( void );
	bool  havePrivileges     ( void );
	void  errorReset         ( void );

	public:
	QString errorString  ( void );
	QString errorValue   ( void );

	public:
	void setDebug ( bool = true );
};

//==================================
// ANSI C/C++ declarations...
//----------------------------------
%include "../../storage.h"
%include "../../process.h"
%include "../../export.h"
%include "../../import.h"
%include "../../init.h"
%include "../../config.h"
%include "../../card.h"
%include "../../keyboard.h"
%include "../../pointers.h"
%include "../../desktop.h"
%include "../../extensions.h"
%include "../../layout.h"
%include "../../path.h"

%include "../../sax.h"
