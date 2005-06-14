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
%typemap(ctype)  QString "char*"
%typemap(imtype) QString "string"
%typemap(cstype) QString "string"
%typemap(out) QString {
	$result = SWIG_csharp_string_callback($1.data());
}
%typemap(csin) QString "$csinput"
%typemap(csout) QString {
	return $imcall;
}
%typemap(typecheck) QString = char*;

//================================== 
// Allow QString refs as parameters
//----------------------------------
%typemap(ctype)  QString& "char*"
%typemap(imtype) QString& "string"
%typemap(cstype) QString& "string"
%typemap(in) QString&,QString& {
	if (! $input) {
		SWIG_CSharpThrowException (
			SWIG_CSharpNullReferenceException, "null string"
		);
	}
	$1 = new QString ($input);
}
%typemap(csin) QString& "$csinput"
%typemap(csout) QString& {
	return $imcall;
}
%typemap(typecheck) QString& = char*;

//==================================
// Allow QDict<QString> return types
//----------------------------------
// TODO
//==================================
// Allow QList<QString> return types
//----------------------------------
// TODO

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
// Process class wrapper...
//----------------------------------
class SaXProcess {
	public:
	SaXProcess ( int = SAX_ISAX );
};

//==================================
// ANSI C/C++ declarations...
//----------------------------------
%include "../../storage.h"
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
