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
	mProc = new QProcess ();
	mExited = false;
}

//====================================
// addArgument
//------------------------------------
void SaXProcessCall::addArgument ( const QString& arg ) {
	mProc->addArgument ( arg );
}

//====================================
// clearArguments
//------------------------------------
void SaXProcessCall::clearArguments ( void ) {
	mProc->clearArguments();
	mExited = false;
}

//====================================
// exitStatus
//------------------------------------
int SaXProcessCall::exitStatus ( void ) {
	return mProc->exitStatus();
}

//====================================
// start
//------------------------------------
bool SaXProcessCall::start ( void ) {
	QObject::connect (
		mProc , SIGNAL ( processExited  (void)),
		this  , SLOT   ( readFromStdout (void))
	);
	if ( ! mProc -> start() ) {
		return false;
	}
	while ( mProc->isRunning() ) {
		usleep (1000);
	}
	while ( ! mExited) {
		usleep (1000);
	}
	return true;
}

//====================================
// readStdout
//------------------------------------
QByteArray SaXProcessCall::readStdout ( void ) {
	return mProc->readStdout();
}

//====================================
// readFromStdout
//------------------------------------
void SaXProcessCall::readFromStdout ( void ) {
	mExited = true;
}
} // end namespace
