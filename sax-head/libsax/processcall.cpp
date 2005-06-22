/**************
FILE          : processcall.cpp
***************
PROJECT       : SaX2 - library interface
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
#include "processcall.h"
    
namespace SaX {
//====================================
// Constructor...
//------------------------------------
SaXProcessCall::SaXProcessCall ( void ) {
	mExitCode  = -1;
}

//====================================
// addArgument
//------------------------------------
void SaXProcessCall::addArgument ( const QString& arg ) {
	mArguments.append ( new QString(arg) );
}

//====================================
// clearArguments
//------------------------------------
void SaXProcessCall::clearArguments ( void ) {
	mArguments.clear();
	mExitCode  = -1;
}

//====================================
// exitStatus
//------------------------------------
int SaXProcessCall::exitStatus ( void ) {
	return mExitCode;
}

//====================================
// start
//------------------------------------
bool SaXProcessCall::start ( void ) {
	//====================================
	// create program call string
	//------------------------------------
	QString program;
	QListIterator<QString> it ( mArguments );
	for (; it.current(); ++it ) {
		program.append (*it.current() + " ");
	}
	//====================================
	// start program and connect stream
	//------------------------------------
	char buf[LINESIZE];
	FILE* fp = popen (program.ascii(),"r");
	if ( ! fp ) {
		return false;
	}
	while (NULL != (fgets(buf,sizeof(buf)-1,fp))) {
		int line = strlen(buf)-1;
		buf[line] = 0;
		mData.append (new QString(buf));
	}
	mExitCode = pclose(fp);
	return true;
}

//====================================
// readStdout
//------------------------------------
QList<QString> SaXProcessCall::readStdout ( void ) {
	return mData;
}
} // end namespace
