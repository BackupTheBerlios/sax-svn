/**************
FILE          : touchscreen.h
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the GPL license
              :
DESCRIPTION   : xapi (X11-Application-Interface)
              : header definitions for touchscreen.c
              :
STATUS        : Status: Up-to-date
**************/
#ifndef TOUCHSCREEN_H
#define TOUCHSCREEN_H 1

#include <qmessagebox.h>
#include <qlistbox.h>
#include <qframe.h>
#include <qlayout.h>

#include "../intro.h"
#include "../../xtab.h"

//=====================================
// Defines...
//-------------------------------------
enum {
    TouchScreenPage1 = Toucher,
};

enum {
	TOUCHER_NEW   = 1,
	TOUCHER_SETUP = 0
};

//=====================================
// Class Headers...
//-------------------------------------
class XTouchScreen : public XTemplate {
	Q_OBJECT

    private:
    QFrame*       mDialog;
	QListBox*     mVendor;
	QListBox*     mName;
	XTabDialog*   mTop;
	QPushButton*  mAdd;
	QPushButton*  mDel;
	QPushButton*  mSetup;
	QListBox*     touchList;

	private:
	int           mSelected;
	int           mState;
	int           mWorkingIndex;
	XIntro*       mIntro;
	QDict<XFile>* mFilePtr;
	QList<int>    mIndexList;

    public:
    XTouchScreen ( XFrame*,XIntro* );
    XTouchScreen ( void );
	int isRotated ( void );
    void addTo   ( XFrame* xf,XIntro* xi );

    private:
    void dialogCreate ( void );
    void resetPage    ( int = PAGE_NOLOAD );
    void pageCalled   ( int );
	void setupTop     (int);
	void initPage     ( void );

    protected:
    bool slotRun   ( int );
	void slotIntro ( int );

	public slots:
	void slotVendor    ( QListBoxItem* );
	void slotName      ( QListBoxItem* );
	void slotSelect    ( QListBoxItem* );
	void slotTopOk     ( void );
	void slotTopCancel ( void );
	void slotAdd       ( void );
	void slotRemove    ( void );
	void slotSetup     ( void );
};

#endif
