/**************
FILE          : xvirtual.h
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the XFree86 license
              :
DESCRIPTION   : xapi (X11-Application-Interface)
              : widget header to provide virtual area selection
              :
STATUS        : Status: Up-to-date
**************/
#ifndef XVIRTUALAREA_H
#define XVIRTUALAREA_H 1

#include <qapplication.h>
#include <qwidget.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qpixmap.h>
#include <qpainter.h>
#include <qstatusbar.h>
#include <qcursor.h>
#include <qtooltip.h>
#include <qcombobox.h>
#include <qhbox.h>

#include "../../config.h"

//======================================
// Defines...
//--------------------------------------
#define INFO_LEAVE_UNTOUCHED   1
#define INFO_INSERT            2
#define INFO_CHANGE            3

//======================================
// Classes...
//--------------------------------------
class XVirtualArea : public QPushButton {
	Q_OBJECT

	private:
	QComboBox*   mInfo;
	QStatusBar*  mStatus;
	QPixmap*     mPixmap;
	QPixmap*     mBuffer;
	QString      mText;
	QWidget*     mParent;
	QList<char>  mResolution;
	bool         mFirstInsert;
	int          mNOPaint;
	int          mStartx;
	int          mStarty;
	int          mWidth;
	int          mHeight;
	int          mX;
	int          mY;
	int          virtualX;
	int          virtualY;

	public:
	XVirtualArea ( int,int,QWidget* = 0 );
	bool hasChanged ( void );
	bool virtualEqualsReal ( void );
	void setText    ( const QString & );
	void setVirtual ( const QString & );
	QPoint getVirtualResolution ( void );
	void setX ( int );
	void setY ( int );

	protected:
	virtual void mouseReleaseEvent ( QMouseEvent* );
	virtual void mousePressEvent ( QMouseEvent* );
	virtual void mouseMoveEvent  ( QMouseEvent* );
	virtual void paintEvent( QPaintEvent* );

	public slots:
	void slotManual ( int );
};

#endif
