/**************
FILE          : xidle.c
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the GPL license
              :
DESCRIPTION   : xidle timer control widget 
              : we will use the xidle tool (sax/tools/wrap)
              : to create a widget which can display the
              : idle time
              :
              :
STATUS        : Status: Up-to-date
**************/

#include <qapplication.h>
#include <qtimer.h>
#include <qlcdnumber.h>
#include <qlayout.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <X11/Xcms.h>
#include <X11/extensions/scrnsaver.h>

#include "xidle.h"
#include "common/log.h"

bool gotStartSignal = false;

//===================================
// XTimeElapsed constructor...
//-----------------------------------
XTimeElapsed::XTimeElapsed (
	QString* pid,QWidget *parent,const char* name) : 
	QWidget (parent,name)
	//QWidget (parent,name,Qt::WStyle_Customize |Qt::WStyle_NoBorderEx) 
{
	dpy = x11Display();
	int timeout,interval;
	int prefer_blank,allow_exp;
	int ss_event;
	int ss_error;

	mPID = 0;
	if (pid) {
		mPID = pid->toInt();
	}

	mTimeout = XSS_TIMEOUT;
	buildWidget();

	signal (SIGUSR2 , startSignal);
	while (1) {
	if (gotStartSignal) {
		break;
	}
	sleep (1);
	}
	if (! XScreenSaverQueryExtension (dpy, &ss_event, &ss_error)) {
	log ( L_ERROR,
		"XTimeElapsed::could not init MIT extension\n"
	);
	}
	XSync (dpy, false);
	XGetScreenSaver (
		dpy,&timeout,&interval,
		&prefer_blank,&allow_exp
	);
	timeout = mTimeout;
	XSetScreenSaver (
		dpy,timeout,interval,prefer_blank,allow_exp
	);
	startTimer (1000);
}

//===================================
// XTimeElapsed build widget...
//-----------------------------------
void XTimeElapsed::buildWidget (void) {
	setGeometry(20,20,150,70);

	setlocale (LC_ALL,"");
	bindtextdomain ("sax", TDOMAIN);
	textdomain ("sax");

	QString caption = gettext("Idle timer...");

	setCaption (QString::fromLocal8Bit(caption));
	QBoxLayout* layout = new QVBoxLayout (this);
	LCD = new QLCDNumber ( 2, this );
	LCD -> display ( mTimeout );
	layout -> addWidget ( LCD );
}

//===================================
// XTimeElapsed timer event...
//-----------------------------------
void XTimeElapsed::timerEvent (QTimerEvent*) {
	mTime = mTimeout;
	XScreenSaverInfo *info;
	info = XScreenSaverAllocInfo();
	XScreenSaverQueryInfo (
		dpy, DefaultRootWindow(dpy), info
	);
	mTime = (int)(info->til_or_since/1000);
	LCD -> display (mTime);
	if (mTime == 0) {
		if (mPID > 0) {
			kill (mPID,15);
		} else {
			kill (getppid(),15);
		}
		exit (0);
	}
}

void startSignal (int) {
	gotStartSignal = true;
}
