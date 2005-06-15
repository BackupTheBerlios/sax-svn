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
	// .../
	//! An object of this type is used to create a QProcess
	//! instance which receives all the data to run the
	//! requested process. The start() method behaves differently
	//! from the original QProcess::start() method. We will wait
	//! for process termination within start() to be sure there
	//! is all output data available after returning.
	// ----
	mProc = new QProcess ();
	QObject::connect (
		mProc , SIGNAL ( processExited  (void)),
		this  , SLOT   ( readFromStdout (void))
	);
}

//====================================
// addArgument
//------------------------------------
void SaXProcessCall::addArgument ( const QString& arg ) {
	// .../
	//! wrapper arround the QProcess::addArgument method
	// ----
	mProc->addArgument ( arg );
}

//====================================
// clearArguments
//------------------------------------
void SaXProcessCall::clearArguments ( void ) {
	// .../
	//! wrapper arround the QProcess::clearArguments method
	// ----
	mProc->clearArguments();
}

//====================================
// exitStatus
//------------------------------------
int SaXProcessCall::exitStatus ( void ) {
	// .../
	//! wrapper arround the QProcess::exitStatus method
	// ----
	return mProc->exitStatus();
}

//====================================
// start
//------------------------------------
bool SaXProcessCall::start ( void ) {
	// .../
	//! run the process now using QProcess::start() but
	//! wait for the process while it's running. After
	//! the process is done wait for data to be ready for
	//! reading...
	// ----
	if ( ! mProc -> start() ) {
		return false;
	}
	while ( mProc->isRunning() ) {
		usleep (1000);
	}
	return true;
}

//====================================
// readStdout
//------------------------------------
QByteArray SaXProcessCall::readStdout ( void ) {
	// .../
	//! wrapper arround the QProcess::readStdout method
	// ----
	return mProc->readStdout();
}

//====================================
// readFromStdout
//------------------------------------
void SaXProcessCall::readFromStdout ( void ) {
	// .../
	//! slot method to be called from QProcess after the process
	//! is really finished. The manual page for QProcess suggest
	//! to wait for this signal to become ready for reading
	//! STDOUT data
	// ----
	fflush (stdout);
}
} // end namespace
