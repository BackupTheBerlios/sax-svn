/**************
FILE          : xfig.h
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the XFree86 license
              :
DESCRIPTION   : xapi (X11-Application-Interface)
              : header definitions for xfig.c
              :
STATUS        : Status: Up-to-date
**************/
#ifndef XFIG_H
#define XFIG_H 1

#include <qapplication.h>
#include <qpushbutton.h>
#include <qwidget.h>
#include <qpainter.h>

#include "../../config.h"

//============================================
// Defines...
//--------------------------------------------
#define FIGURE_SQUARE 60

//============================================
// Class definitions...
//--------------------------------------------
class XFig : public QPushButton {
	Q_OBJECT

	private:
	int mID;
	int mIndex;
	int mStartx;
	int mStarty;
	QRect mStartg;
	QPixmap* mPixmap;
	QString* tipText;

	private:
	float mCurrentx;
	float mCurrenty;

	public:
	~XFig (void);
	XFig ( QWidget *p,int = 0,int = 0,int = 0, int = 0 );
	QPoint getPosition ( void );
	const char* getTip ( void );
	int  getDistance ( int,int );
	void moveTo      ( int,int );
	void setToolTip  ( const QString& );
	int  getDistance ( QPoint  );
	int  getIndex ( void );
	void setIndex ( int  );
	int  getID    ( void );

	protected:
	virtual void mouseMoveEvent ( QMouseEvent* );
	virtual void mouseDoubleClickEvent ( QMouseEvent* );
	virtual void mousePressEvent ( QMouseEvent* );
	virtual void mouseReleaseEvent ( QMouseEvent* );
	virtual void paintEvent ( QPaintEvent* );

	signals:
	void sigReleased ( XFig* );
	void sigPressed  ( int );
};

#endif
