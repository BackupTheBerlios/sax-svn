/**************
FILE          : mouse.h
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the XFree86 license
              :
DESCRIPTION   : xapi (X11-Application-Interface)
              : header definitions for mouse.c
              :
STATUS        : Status: Up-to-date
**************/
#ifndef MOUSE_H
#define MOUSE_H 1

#include <qmessagebox.h>
#include <qmainwindow.h>
#include <qbuttongroup.h>
#include <qcombobox.h>
#include <qcheckbox.h>
#include <qframe.h>
#include <qlayout.h>
#include <qspinbox.h>
#include <qvbox.h>

#include "../intro.h"
#include "../../xtab.h"
#include "xmouse.h"

//=====================================
// Defines...
//-------------------------------------
enum {
	MousePage1 = Mouse,
};

enum {
	MOUSE_NEW   = 1,
	MOUSE_SETUP = 0
};

//=====================================
// Class Headers...
//-------------------------------------
class XMouse : public XTemplate {
	Q_OBJECT

	private:
	XTabDialog*   mTop;
	QFrame*       mDialog;
	QListBox*     mVendor;
	QListBox*     mName;
	QListBox*     mouseList;
	QCheckBox*    mCheckEmulate;
	QCheckBox*    mCheckWheel;
	QComboBox*    mPort;
	QPushButton*  mAdd;
	QPushButton*  mDel;
	XShowMouse*   mEvents;
	QPushButton*  mSetup;
	QCheckBox*    mCheckWheelEmu;
	QSpinBox*     mWheelKey;
	QVBox*        layer6;
	QVBox*        layer7;
	QVBox*        layer8;

	private:
	int           mSelected;
	int           mState;
	int           mWorkingIndex;
	XIntro*       mIntro;
	QDict<XFile>* mFilePtr;
	QList<int>    mIndexList;

	public:
	XMouse (XFrame*,XIntro*);
	XMouse (void);
	void addTo (XFrame* xf,XIntro* xi);

	private:
	void dialogCreate  (void);
	void resetPage     (int = PAGE_NOLOAD);
	void pageCalled    (int);
	void setupTop      (int);
	void initPage      (void);

	protected:
	bool slotRun       (int);
	void slotIntro     (int);

	public slots:
	void slotAdd       (void);
	void slotRemove    (void);
	void slotSetup     (void);
	void slotSelect    (QListBoxItem *);
	void slotTopOk     (void);
	void slotTopCancel (void);
	void slotVendor    (QListBoxItem *);
	void slotName      (QListBoxItem *);
	void slotPort      (int);
	void slotWheelKey  (int);
};

#endif
