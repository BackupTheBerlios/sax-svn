/**************
FILE          : accessx.h
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the XFree86 license
              :
DESCRIPTION   : xapi (X11-Application-Interface)
              : header definitions for accessx.c
              :
STATUS        : Status: Up-to-date
**************/
#ifndef ACCESSX_H
#define ACCESSX_H 1

#include <unistd.h>
#include <qmessagebox.h>
#include <qradiobutton.h>
#include <qstatusbar.h>
#include <qmainwindow.h>
#include <qframe.h>
#include <qlayout.h>
#include <qlcdnumber.h>
#include <qslider.h>
#include <qcheckbox.h>
#include <X11/XKBlib.h>

// ...
// X11 defines Below which causes conflicht on 
// QSlider::Below. Therefore this definition is
// switched of here
// ---
#ifdef Below
#undef Below
#endif

#include "../intro.h"
#include "../../xbox.h"

//=====================================
// Defines...
//-------------------------------------
#define XACCESS_CODE(c) ((c)->ctrls->enabled_ctrls & (XkbMouseKeysMask | XkbMouseKeysAccelMask))

enum {
    XAccessPage1 = Xaccess,
};

//=====================================
// Class Headers...
//-------------------------------------
class XAccessX : public XTemplate {
	Q_OBJECT

    private:
    QFrame*       mDialog;
    XIntro*       mIntro;
	QDict<XFile>* mFilePtr;
	QRadioButton* mEnable;
	QRadioButton* mDisable;
	QSlider*      mSpeed;
	QPushButton*  mApply;
	QLCDNumber*   mLCD;
	QButtonGroup* mControl;

	private:
	XkbDescPtr xkb;
	bool isActive;
	int  major;
	int  minor;
	int  op;
	int  event;
	int  error;

    public:
    XAccessX (XFrame*,XIntro*);
    XAccessX (void);
    void addTo (XFrame* xf,XIntro* xi);

    private:
    void dialogCreate (void);
    void resetPage    (int = PAGE_NOLOAD);
    void pageCalled   (int);
	void initPage     (void);

    protected:
    bool slotRun   (int);
	void slotIntro (int);

	public slots:
	void slotLCD   (int);
	void slotState (bool);
	void slotApply (void);
};

#endif
