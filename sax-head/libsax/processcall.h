/**************
FILE          : processcall.h
***************
PROJECT       : SaX2 - library interface [header]
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : SaX2 - SuSE advanced X11 configuration 
              : 
              :
DESCRIPTION   : native C++ class library to access SaX2
              : functionality. Easy to use interface for
              : //.../
              : - importing/exporting X11 configurations
              : - modifying/creating X11 configurations 
              : ---
              :
              :
STATUS        : Status: Development
**************/
#ifndef SAX_PROCESS_CALL_H
#define SAX_PROCESS_CALL_H 1

//====================================
// Includes...
//------------------------------------
#include <qprocess.h>

namespace SaX {
//====================================
// Interface class for dlopen ability
//------------------------------------
/*! \brief SaX2 -  SaX process execution class interface.
*
* The interface class is provided to be able to dlopen the
* library and have all methods available in the compilers
* virtual table. For a detailed description of the class itself
* please refer to the derived class definition
*/
class SaXProcessCallIF : public QObject {
	public:
	virtual void addArgument ( const QString& ) = 0;
	virtual void clearArguments ( void ) = 0;
	virtual QByteArray readStdout ( void ) = 0;
	virtual bool start ( void ) = 0;
	virtual int exitStatus ( void ) = 0;

	public:
	virtual ~SaXProcessCallIF ( void ) { }
};

//====================================
// Class SaXProcessCall...
//------------------------------------
/*! \brief SaX2 -  Linux process execution class.
*
*/
class SaXProcessCall : public SaXProcessCallIF {
	Q_OBJECT

	private:
	QProcess*  mProc;
	bool       mExited;

	public:
	void addArgument ( const QString& );
	void clearArguments ( void );
	QByteArray readStdout ( void );
	bool start ( void );
	int exitStatus ( void );

	public slots:
	void readFromStdout ( void );

	public:
	SaXProcessCall ( void );
};
} // end namespace
#endif
