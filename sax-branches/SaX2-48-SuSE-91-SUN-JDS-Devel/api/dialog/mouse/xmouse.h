/**************
FILE          : xmouse.h
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the XFree86 license
              :
DESCRIPTION   : header for xmouse.cpp
              :
STATUS        : Status: Development
**************/
#ifndef XSHOWMOUSE_H
#define XSHOWMOUSE_H 1

#include <qtimer.h>
#include <qdict.h>
#include <qstatusbar.h>
#include <qlayout.h>
#include <qpixmap.h>
#include <qpainter.h>
#include "xevent.h"

//=====================================
// Defines
//-------------------------------------
#define PIX_DEFAULT "/usr/X11R6/lib/sax/api/pixmaps/mouse_default.png"
#define PIX_FRONT   "/usr/X11R6/lib/sax/api/pixmaps/mouse_front.png"
#define PIX_LEFT    "/usr/X11R6/lib/sax/api/pixmaps/mouse_left.png"
#define PIX_MIDDLE  "/usr/X11R6/lib/sax/api/pixmaps/mouse_middle.png"
#define PIX_RIGHT   "/usr/X11R6/lib/sax/api/pixmaps/mouse_right.png"
#define PIX_BACK    "/usr/X11R6/lib/sax/api/pixmaps/mouse_back.png"

//=====================================
// Class Definitions
//-------------------------------------
class XShowMouse : public XInputEvent {
	Q_OBJECT

	private:
	int mID;
	QDict<char>* mTextPtr;
	QStatusBar*  mStatus;
	QTimer*      mTimer;
	QPixmap*     mPixmap[6];
	QPixmap*     mBuffer;
	int mXold;
	int mYold;
	int mX;
	int mY;

	public:
	void setPixmap (int);
	XShowMouse ( QDict<char>*, QWidget* = 0 );
	virtual void paintEvent( QPaintEvent* );

	public:
	void singleClickEvent (int);
	void doubleClickEvent (int);
	void pressEvent (int);
	void releaseEvent (int);
	void wheelEvent (int);
	void moveEvent (int,int);

	public slots:
	void timerDone (void);
};

#endif
