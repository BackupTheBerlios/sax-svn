/**************
FILE          : geometry.h
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the XFree86 license
              :
DESCRIPTION   : xapi (X11-Application-Interface)
              : header definitions for geometry.c
              :
STATUS        : Status: Up-to-date
**************/
#ifndef GEOMETRY_H
#define GEOMETRY_H 1

#include <qmessagebox.h>
#include <qframe.h>
#include <qlayout.h>

#include "../intro.h"

//=====================================
// Defines...
//-------------------------------------
enum {
    GeometryPage1 = Geometry * 10,
};

//=====================================
// Class Headers...
//-------------------------------------
class XDisplayGeometry : public XTemplate {
    private:
    QFrame*       mDialog0;
    QFrame*       mDialog1;
    XIntro*       mIntro;
	QDict<XFile>* mFilePtr;

    public:
    XDisplayGeometry (XFrame*,XIntro*);
    XDisplayGeometry (void);
    void addTo (XFrame* xf,XIntro* xi);

    private:
    void dialogCreate (void);
    void resetPage    (int = PAGE_NOLOAD);
    void pageCalled   (int);

    protected:
    bool slotRun   (int);
	void slotIntro (int);
};


#endif
