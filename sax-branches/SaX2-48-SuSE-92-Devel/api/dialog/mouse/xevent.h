/**************
FILE          : xevent.h
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the GPL license
              :
DESCRIPTION   : catch all mouse events to identify buttons
              : wheels etc...
              :
STATUS        : Status: Development
**************/
#ifndef XEVENT_H
#define XEVENT_H 1

#include <qwidget.h>
#include <qtimer.h>

//====================================
// Definitions...
//------------------------------------
#define WHEEL_DELTA 120

//====================================
// Class Definitions...
//------------------------------------
class XInputEvent : public QWidget {
	Q_OBJECT

	private:
	QTimer *timer;
	int mBtn;
	int doubleClick;
	int mouseReleased;

	public:
	XInputEvent ( QWidget* = 0);

    public:
    virtual bool eventFilter ( QObject*, QEvent* );

	public:
	virtual void doubleClickEvent (int);
	virtual void singleClickEvent (int);
	virtual void wheelEvent (int);
	virtual void pressEvent (int);
	virtual void releaseEvent (int);
	virtual void moveEvent (int,int);

	public slots:
	void timerDone (void);
};

#endif
