//==================================
// Interface definition for libsax
//----------------------------------
#define NO_OPERATOR_SUPPORT 1
%module SaX
%{
#include "../../sax.h"
%}

//==================================
// Typemaps
//----------------------------------
//==================================
// Allow QString return types
//----------------------------------
// TODO
//================================== 
// Allow QString refs as parameters
//----------------------------------
// TODO
//==================================
// Allow QDict<QString> return types
//----------------------------------
// TODO
//==================================
// Allow QList<QString> return types
//----------------------------------
// TODO
//==================================
// Allow QStringList return types
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
