/**************
FILE          : multihead.h
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the GPL license
              :
DESCRIPTION   : xapi (X11-Application-Interface)
              : header definitions for multihead.c
              :
STATUS        : Status: Up-to-date
**************/
#ifndef MULTIHEAD_H
#define MULTIHEAD_H 1

#include <qmessagebox.h>
#include <qradiobutton.h>
#include <qframe.h>
#include <qlayout.h>

#include "../intro.h"

//=====================================
// Enumarators...
//-------------------------------------
enum {
    MultiheadPage1 = Modus,
};

//=====================================
// Defines...
//-------------------------------------
#define TRADITIONAL 0
#define CLONE       1
#define XINERAMA    2

//=====================================
// Class Headers...
//-------------------------------------
class XMultihead : public XTemplate {
    private:
    QFrame*       mDialog;
    XIntro*       mIntro;
	QDict<XFile>* mFilePtr;
	QRadioButton* mTraditional;
	QRadioButton* mClone;
	QRadioButton* mXinerama;
	bool          mAvailable;

    public:
    XMultihead (XFrame*,XIntro*);
    XMultihead (void);
    void addTo (XFrame* xf,XIntro* xi);

    private:
    void dialogCreate (void);
    void resetPage    (int = PAGE_NOLOAD);
    void pageCalled   (int);
	void initPage     (void);
	void updateModus  (void);

    protected:
    bool slotRun   (int);
	void slotIntro (int);
};


#endif
