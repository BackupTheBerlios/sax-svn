/**************
FILE          : tablet.h
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the XFree86 license
              :
DESCRIPTION   : xapi (X11-Application-Interface)
              : header definitions for tablet.c
              :
STATUS        : Status: Up-to-date
**************/
#ifndef TABLET_H
#define TABLET_H 1

#include <qmessagebox.h>
#include <qframe.h>
#include <qlayout.h>

#include "../intro.h"

//=====================================
// Defines...
//-------------------------------------
enum {
    TabletPage1 = Tablet,
};

//=====================================
// Class Headers...
//-------------------------------------
class XTablet : public XTemplate {
	Q_OBJECT

    private:
    QFrame*       mDialog;
	QListBox*     mVendor;
	QListBox*     mName;
	QCheckBox*    mPen;
	QCheckBox*    mEraser;
	QPushButton*  mRemove;

	private:
	XIntro*       mIntro;
	QDict<XFile>* mFilePtr;
	bool          gotTablet;
	bool          gotPen;
	bool          gotEraser;
	bool          deleteOnly;
	int           IDTablet;
	int           IDPen;
	int           IDEraser;

    public:
    XTablet ( XFrame*,XIntro* );
    XTablet ( void );
    void addTo ( XFrame* xf,XIntro* xi );

    private:
    void dialogCreate ( void );
    void resetPage    ( int = PAGE_NOLOAD );
    void pageCalled   ( int );
	void initPage     ( void );
	void addTablet    ( void );
	QDict<char>* searchTool ( const QString&,const QString& );

    protected:
    bool slotRun    ( int );
	void slotIntro  ( int );

	public slots:
	void slotVendor ( QListBoxItem* );
	void slotName   ( QListBoxItem* );
	void slotRemove ( void );
};

#endif
