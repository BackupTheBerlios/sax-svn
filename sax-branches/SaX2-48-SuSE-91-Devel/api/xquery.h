/**************
FILE          : xquery.h
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
STATUS        : Status: Up-to-date
**************/
#ifndef XQUERY_H
#define XQUERY_H 1

#include <qapplication.h>
#include <qwidget.h>

#include "config.h"

class XQueryServer : public QWidget {
	private:
	QString* mResult;
	QString mOpt;
	QString mArg;
	bool errorCheck;

	public:
	XQueryServer ( const QString&,const QString& );
	XQueryServer ( void );
	QString* run ( void );
	void setArguments ( const QString& );
	void setOption    ( const QString& );
	void useErrorHandler (void);
};

#endif
