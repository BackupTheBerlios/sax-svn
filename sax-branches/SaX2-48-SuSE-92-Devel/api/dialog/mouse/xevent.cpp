/**************
FILE          : xevent.cpp
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the GPL license
              :
DESCRIPTION   : event filter for mouse events
              :
STATUS        : Status: Development
**************/

#include "xevent.h"

//====================================
// Constructor...
//------------------------------------
XInputEvent::XInputEvent ( QWidget* parent ): QWidget (parent) {
	doubleClick   = FALSE;
	mouseReleased = FALSE;
	timer = new QTimer( this );
	connect ( 
		timer, SIGNAL (timeout()), this, SLOT (timerDone()) 
	);
	installEventFilter (this);
}

//====================================
// Event filter for application...
//------------------------------------
bool XInputEvent::eventFilter ( QObject*, QEvent* event ) {
	QMouseEvent* mouse = (QMouseEvent*)event;
	QWheelEvent* wheel = (QWheelEvent*)event;
	if ((! mouse) && (! wheel)) {
		return (false);
	}
	switch (mouse->type()) {
	case QEvent::MouseButtonDblClick:
		timer->stop();
		doubleClick   = TRUE;
		mouseReleased = FALSE;
		mBtn = mouse->button();
		doubleClickEvent (mBtn);
	break;
	case QEvent::MouseButtonPress:
		mBtn = mouse->button();
		pressEvent (mBtn);
		mouseReleased = FALSE;
		timer->start ( 300, TRUE );
		doubleClick = FALSE;
	break;
	case QEvent::MouseButtonRelease:
		if (! doubleClick) {
			mouseReleased = TRUE;
			mBtn = mouse->button();
			releaseEvent (mBtn);
		}
	break;
	case QEvent::Wheel:
		wheelEvent ( wheel->delta()/WHEEL_DELTA );
	break;
	case QEvent::MouseMove:
		moveEvent (mouse->x(),mouse->y());
	break;
	default:
	break;
	}
	return (false);
}

//====================================
// Timer for double clicks...
//------------------------------------
void XInputEvent::timerDone ( void ) {
	if (! doubleClick) {
	if (! mouseReleased) {
		timer->start ( 300, TRUE );
	} else {
		singleClickEvent (mBtn);
		mouseReleased = FALSE;
	}
	}
}

//====================================
// Callback function moving mouse
//------------------------------------
void XInputEvent::moveEvent (int x,int y) {
	printf("moveEvent(%d:%d)...\n",x,y);
}

//====================================
// Callback function for double click
//------------------------------------
void XInputEvent::doubleClickEvent (int) {
	printf("doubleClickEvent()...\n");
}

//====================================
// Callback function for single click
//------------------------------------
void XInputEvent::singleClickEvent (int) {
	printf("singleClickEvent()...\n");
}

//====================================
// Callback function for wheel event
//------------------------------------
void XInputEvent::wheelEvent (int) {
	printf("wheelEvent()...\n");
}

//====================================
// Callback function for press event
//------------------------------------
void XInputEvent::pressEvent (int) {
	printf("pressEvent()...\n");
}

//====================================
// Callback function for release event
//------------------------------------
void XInputEvent::releaseEvent (int) {
	printf("releaseEvent()...\n");
}
