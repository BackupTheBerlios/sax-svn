/**************
FILE          : template.h
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the XFree86 license
              :
DESCRIPTION   : xapi (X11-Application-Interface)
              : abstract header definition for template
              : every dialog should enherit this class
              : 
              :
STATUS        : Status: Up-to-date
**************/
#ifndef XTEMPLATE_H
#define XTEMPLATE_H 1

#include <qapplication.h>
#include <qmessagebox.h>
#include <qwidget.h>
#include <qwidgetstack.h>
#include <qpushbutton.h>

#include "frame.h"
#include "file.h"
#include "common/log.h"
#include "xbox.h"

//====================================
// Pre-Definitions...
//------------------------------------
class XIntro;

//====================================
// Defines...
//------------------------------------
#define PAGE_RELOAD 1
#define PAGE_NOLOAD 0
#define PAGE_CANCEL 2

//====================================
// Class headers...
//------------------------------------
class XTemplate : public QWidget {
	Q_OBJECT

	protected:
	QWidgetStack* mStack;
	XFrame*       mFrame;
	XIntro*       mIntro;
	bool          mVisible;
	int           mLastPage;
	int           mPage;
	int           mIndex;
	QDict<char>   mInit;
	QDict<char>*  mTextPtr;
	QStatusBar*   mStatus;

	public:
	XTemplate (XFrame*,XIntro* = NULL);
	XTemplate (void);
	void getConnected (XFrame*);

	public slots:
	virtual void slotNext   (void);
	virtual void slotBack   (void);
	virtual bool slotRun    (int);
	virtual void slotCheck  (int);
	virtual void slotIntro  (int);
	virtual void slotAbort  (int);
	virtual void slotExit   (void);
	virtual void resetPage  (int = PAGE_RELOAD);
	virtual int  savePage   (void);
	virtual void pageCalled (int);

	signals:
	void sigExit (void);
};

#endif
