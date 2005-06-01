/**************
FILE          : xidle.h
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the GPL license
              :
DESCRIPTION   : xidle timer control widget 
              : header for xidle.c
              :
STATUS        : Status: Up-to-date
**************/
#ifndef XIDLE_H
#define XIDLE_H 1

#include <qtimer.h>
#include <qlcdnumber.h>
#include <locale.h>
#include <libintl.h>

#include "config.h"

//===================================
// Defines...
//-----------------------------------
#define XSS_TIMEOUT 30

//===================================
// Signal prototype...
//-----------------------------------
void startSignal (int);

//===================================
// XTimeElapsed class...
//-----------------------------------
class XTimeElapsed : public QWidget {
	Q_OBJECT

	private:
	int mPID;
	QLCDNumber* LCD;

	private:
	int mTimeout;
	int mTime;
	Display* dpy;
	
	public:
	XTimeElapsed (QString* =0,QWidget* =0, const char* =0);
	void buildWidget (void);

	protected:
	void timerEvent (QTimerEvent*);
};

#endif
