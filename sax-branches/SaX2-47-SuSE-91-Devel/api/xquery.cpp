/**************
FILE          : xquery.c
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the XFree86 license
              :
DESCRIPTION   : provide general use of xquery tool
              : run xquery with given options and save the
              : result for later use
              :
              :
STATUS        : Status: Up-to-date
**************/
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "xquery.h"
#include "xapi.h"

//=====================================
// XQuery Constructor...
//-------------------------------------
XQueryServer::XQueryServer (void) {
	mResult = new QString();
	errorCheck = false;
}

//=====================================
// XQuery Constructor...
//-------------------------------------
XQueryServer::XQueryServer (const QString& option,const QString& arg) {
	mResult = new QString();
	errorCheck = false;
	mOpt = option;
	mArg = arg;
}

//=====================================
// XQuery set xquery Option...
//-------------------------------------
void XQueryServer::setOption (const QString& option) {
	mOpt = option;
}

//=====================================
// XQuery set argument for option...
//-------------------------------------
void XQueryServer::setArguments (const QString& arguments) {
	mArg = arguments;
}

//=====================================
// XQuery need STDERR output...
//-------------------------------------
void XQueryServer::useErrorHandler (void) {
	errorCheck = true;
}

//=====================================
// XQuery run query...
//-------------------------------------
QString* XQueryServer::run (void) {
	QString optd ("-d");
	QString dpy  (DisplayString (x11Display()));
	if (mArg.isEmpty()) {
		if (! errorCheck) {
		*mResult = qx ( XQUERY,STDOUT,3,"%s %s %s",
			optd.ascii(),dpy.ascii(),mOpt.ascii()
		);
		} else {
		*mResult = qx ( XQUERY,STDERR,3,"%s %s %s",
			optd.ascii(),dpy.ascii(),mOpt.ascii()
		);
		}
	} else {
		if (! errorCheck) {
		*mResult = qx ( XQUERY,STDOUT,4,"%s %s %s %s",
			optd.ascii(),dpy.ascii(),mOpt.ascii(),mArg.ascii()
		);
		} else {
		*mResult = qx ( XQUERY,STDERR,4,"%s %s %s %s",
			optd.ascii(),dpy.ascii(),mOpt.ascii(),mArg.ascii()
		);
		}
	}
	return (mResult);
}
