/**************
FILE          : xfig.c
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the GPL license
              :
DESCRIPTION   : xapi (X11-Application-Interface)
              : XFig is one class used with the XPlot widget
              : we implement the single monitors building the
              : ServerLayout here
              :
STATUS        : Status: Up-to-date
**************/
#include <stdio.h>
#include <qpixmap.h>
#include <qtooltip.h>
#include <qframe.h>
#include <math.h>
#include "xfig.h"

//============================================
// XFig Constructor
//--------------------------------------------
XFig::XFig ( QWidget *p,int x,int y,int index,int id ) : QPushButton( p ) {
	mPixmap = new QPixmap (FIGURE);
	tipText = new QString();
    setPixmap   (*mPixmap);
	setGeometry (mPixmap->rect());
	setFlat (TRUE);
	mCurrentx = x;
	mCurrenty = y;
	mIndex    = index;
	mID       = id;
	move (x-(width()/2),y-(height()/2));
}

//============================================
// XFig Destructor
//--------------------------------------------
XFig::~XFig ( void ) {
	QToolTip::remove (this);
}

//============================================
// XFig set tool tip for this widget
//--------------------------------------------
void XFig::setToolTip ( const QString& text ) {
	if (text) {
	tipText->sprintf("%s",text.ascii());
	QToolTip::add (this,text);
	}
}

//============================================
// receive mouse move event
//--------------------------------------------
void XFig::mouseMoveEvent ( QMouseEvent* m ) {
	// ...
	// move the button widget acording to the mouse movement
	// and save the current position as middle point
	// coordinates
	// ------------
	QRect geo = frameGeometry();
	int x = (geo.x() + m->x()) - mStartx;
	int y = (geo.y() + m->y()) - mStarty;
	move (mStartg.x()+x,mStartg.y()+y);
	mCurrentx = geo.x() + ( width()  / 2 );
	mCurrenty = geo.y() + ( height() / 2 );
}

//============================================
// receive mouse press event
//--------------------------------------------
void XFig::mousePressEvent ( QMouseEvent* m ) {
	// ...
	// save the start point if we receive a press
	// event from the mouse (drag start)
	// ----------------------------------
	QRect geo = frameGeometry();
	mStartx = geo.x() + m->x();
	mStarty = geo.y() + m->y();
	mStartg = geo;
}

//============================================
// receive mouse double click event
//--------------------------------------------
void XFig::mouseDoubleClickEvent ( QMouseEvent* ) {
	sigPressed ( mID );
}

//============================================
// receive release event
//--------------------------------------------
void XFig::mouseReleaseEvent ( QMouseEvent* ) {
	// ...
	// send signal with the XFig object used
	// for this drag action
	// ---------------------
	sigReleased (this);
}

//============================================
// paint me: a flat button :-)
//--------------------------------------------
void XFig::paintEvent ( QPaintEvent * p ) {
	QPushButton::paintEvent (p);
	setPixmap (*mPixmap);
}

//============================================
// get current middle point
//--------------------------------------------
QPoint XFig::getPosition ( void ) {
	QPoint pos;
	pos.setX ((int)mCurrentx);
	pos.setY ((int)mCurrenty);
	return (pos);
}

//============================================
// calculate distance between me and X/Y
//--------------------------------------------
int XFig::getDistance ( int x, int y ) {
	if ((x == 0) || (y == 0)) {
		return (-1);
	}
	float kx = mCurrentx - x;
	float ky = mCurrenty - y;
	float dist = sqrt ((kx*kx) + (ky*ky));
	return ((int)dist);
}

//============================================
// calculate distance QPoint wrapper
//--------------------------------------------
int XFig::getDistance ( QPoint p ) {
	return (getDistance (p.x(),p.y()));
}

//============================================
// move widget to middle point x/y
//--------------------------------------------
void XFig::moveTo ( int x, int y) {
    move (x - width()/2, y - height()/2);
    mCurrentx = x;
    mCurrenty = y;
}

//============================================
// set index number for this widget
//--------------------------------------------
void XFig::setIndex ( int index ) {
	mIndex = index;
}

//============================================
// get currently used tool tip text
//--------------------------------------------
const char* XFig::getTip ( void ) {
	return (tipText->ascii());
}

//============================================
// get index number 
//--------------------------------------------
int XFig::getIndex ( void ) {
	return (mIndex);
}

//============================================
// get widget ID
//--------------------------------------------
int XFig::getID ( void ) {
	return (mID);
}
