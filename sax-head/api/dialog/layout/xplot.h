/**************
FILE          : xplot.h
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the GPL license
              :
DESCRIPTION   : xapi (X11-Application-Interface)
              : header definitions for xplot.c
              :
STATUS        : Status: Up-to-date
**************/
#ifndef XPLOT_H
#define XPLOT_H 1

#include <qapplication.h>
#include <qlist.h>
#include <qwidget.h>
#include <qintdict.h>

#include "xfig.h"

//============================================
// Defines...
//--------------------------------------------
#define MAX_FIGURES 12

//============================================
// Class XLayoutLine definitions...
//--------------------------------------------
class XLayoutLine {
	public:
	int ID;
	int mLeft,mRight,mTop,mBottom;

	public:
	XLayoutLine ( void );
};


//============================================
// Class XMatrixInfo definitions...
//--------------------------------------------
class XMatrixInfo {
	public:
	int ID;
	int mLeft,mRight;
	int mTop,mBottom;
	QString mTip;

	public:
	XMatrixInfo ( void );
};

//============================================
// Class XMatrixPoint definitions...
//--------------------------------------------
class XMatrixPoint {
	private:
	int mIndex;

	public:
	QPoint mBasePoint;
	QPoint mLeft;
	QPoint mRight;
	QPoint mTop;
	QPoint mBottom;

	public:
	XMatrixPoint (int = 0);
	void setIndex ( int  );
	int  getIndex ( void );
};

//============================================
// Class XPlot definitions...
//--------------------------------------------
class XPlot : public QWidget {
	Q_OBJECT

	private:
	int mID;
	int mRect;
	int mRows;
	int mColumns;
	QWidget* mParent;
	QList<XMatrixPoint>  mPoint;
	QIntDict<char> mFree;
	QIntDict<XFig> mWidget;
	bool mPainted;
	QList<XMatrixInfo> mWidgetStack;

	public:
	XPlot ( QWidget* = 0,int = 40 );
	XFig* addWidget ( int,int,int=-1,const QString& = NULL );
	void setWidget ( int,int,int,int,int,const QString& = NULL );
	void setLayout ( int=1,int=1 );
	XFig* searchWidget ( int );
	XFig* searchWidget ( int,int );
	XFig* searchWidget ( QPoint );
	void removeWidget ( int );
	void buildMatrix  ( void );
	void clear ( void );
	int getPointIndex ( int,int );
	int getRows ( void );
	int getColumns ( void );
	QList<XLayoutLine> getLayout ( void );
	bool isClean ( void );

	private:
	void clearInternal ( void );

	protected:
	virtual void paintEvent  ( QPaintEvent* );
	virtual void resizeEvent ( QResizeEvent* );

	public slots:
	void gotReleased ( XFig* );
	void gotPressed  ( int );

	signals:
	void sigFigure ( int );
};

#endif
