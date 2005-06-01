/**************
FILE          : virtual.h
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the GPL license
              :
DESCRIPTION   : xapi (X11-Application-Interface)
              : header definitions for virtual.c
              :
STATUS        : Status: Up-to-date
**************/
#ifndef VIRTUAL_H
#define VIRTUAL_H 1

#include <qcombobox.h>
#include <qframe.h>
#include <qlayout.h>

#include "xvirtual.h"
#include "../intro.h"

//=====================================
// Enumarators...
//-------------------------------------
enum {
	VirtualPage1 = Virtual,
};

//=====================================
// Class Headers...
//-------------------------------------
class XVirtual : public XTemplate {
	Q_OBJECT

	private:
	QFrame*       mOuterFrame;
	QComboBox*    mDesktop;
	XVirtual*     mVirtual;
	QWidgetStack* mVirtualStack;

	private:
	QFrame*       mDialog;
	XIntro*       mIntro;
	QDict<XFile>* mFilePtr;
	QList<XVirtualArea> mArea;

	public:
	XVirtual (XFrame*,XIntro*);
	XVirtual (void);
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
	void slotDesktop ( int  );
	void slotResize  ( void );
};

#endif
