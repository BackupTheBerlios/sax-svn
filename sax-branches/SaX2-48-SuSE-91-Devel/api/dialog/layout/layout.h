/**************
FILE          : layout.h
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the XFree86 license
              :
DESCRIPTION   : xapi (X11-Application-Interface)
              : header definitions for layout.c
              :
STATUS        : Status: Up-to-date
**************/
#ifndef LAYOUT_H
#define LAYOUT_H 1

#include <qmessagebox.h>
#include <qframe.h>
#include <qlayout.h>
#include <qregexp.h>

#include "../intro.h"
#include "xplot.h"

//=====================================
// Defines...
//-------------------------------------
enum {
    LayoutPage1 = Layout,
};

//=====================================
// Class Headers...
//-------------------------------------
class XLayout : public XTemplate {
	Q_OBJECT

    private:
    QFrame*       mDialog;
    XIntro*       mIntro;
	QDict<XFile>* mFilePtr;
	XPlot*        mMatrix;
	bool          mAvailable;

    public:
    XLayout (XFrame*,XIntro*);
    XLayout (void);
    void addTo (XFrame* xf,XIntro* xi);
	XPlot* getMatrix (void);
	void updateServerLayout (void);
	void setupLayout (void);

    private:
    void dialogCreate (void);
    void resetPage    (int = PAGE_NOLOAD);
    void pageCalled   (int);
	void initPage     (void);

    protected:
    bool slotRun   (int);
	void slotIntro (int);

	public slots:
	void gotFigure (int);
};


#endif
