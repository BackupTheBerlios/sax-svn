/**************
FILE          : desktop.h
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the XFree86 license
              :
DESCRIPTION   : xapi (X11-Application-Interface)
              : header definitions for desktop.c
              :
STATUS        : Status: Up-to-date
**************/
#ifndef DESKTOP_H
#define DESKTOP_H 1

#include <qmessagebox.h>
#include <qspinbox.h>
#include <qframe.h>
#include <qlayout.h>
#include <qlistview.h>
#include <qvbox.h>
#include <errno.h>

#include "../intro.h"
#include "../../xtab.h"

//=====================================
// Defines...
//-------------------------------------
enum {
    DesktopPage1 = Colors,
};

//=====================================
// Class Headers...
//-------------------------------------
class XDesktop : public XTemplate {
	Q_OBJECT

    private:
    QFrame*       mDialog;
	XTabDialog*   mTop;
	QPushButton*  mSetup;
	QPushButton*  mAdd;
	QSpinBox*     mNewX;
	QSpinBox*     mNewY;
	QListView*    mResolution;
	QLabel*       mInfo;
	QComboBox*    mColor;
	QListBox*     mDesktopList;
    XIntro*       mIntro;

	public:
	int           mCurrentColorDepth;

	private:
	int           mSelected;
	bool          mFramebufferUsed;
	bool          mNewResAdded;
	QDict<XFile>* mFilePtr;
	QDict<char>*  mColorPtr;
	QDict<char>*  mFBResPtr;
	QList<char>   m4ConfigRes;
	QList<char>   m8ConfigRes;
	QList<char>   m15ConfigRes;
	QList<char>   m16ConfigRes;
	QList<char>   m24ConfigRes;
	QDict<char>   mClipBoard;
	QVBox*        layer6;
	QVBox*        layer7;
	QVBox*        layer8;

    public:
    XDesktop ( XFrame*,XIntro* );
    XDesktop ( void );
    void addTo ( XFrame* xf,XIntro* xi );
	QListBox* getList (void);
	void setMessage   ( 
		const QString&,XBox::Icon = XBox::Warning,const QString& = 0
	);

	public:
	void resetPage    ( int = PAGE_NOLOAD );

    private:
    void dialogCreate ( void );
    void pageCalled   ( int );
	void writeLILO    ( int, const QString& );
	void initPage     ( void );
	void setupTop     ( int = -1 );
	bool addOk        ( void );

    protected:
    bool slotRun   ( int );
	void slotIntro ( int );

	public slots:
	void slotSelect       ( QListBoxItem* );
	void slotDoubleSelect ( QListBoxItem* );
	void slotSetup     ( void );
	void slotColor     ( int );
	void slotUsed      ( QListViewItem* );
	void slotAdd       ( void );
	void slotXChanged  ( int );
	void slotYChanged  ( int );
	void slotTopCancel ( void );
	void slotTopOk     ( void );
};

//=====================================
// Class Headers...
//-------------------------------------
class XCheckListItem: public QCheckListItem {
	private:
	QListView* mResolution;
	XIntro*    mIntro;
	
	public:
	XCheckListItem (
		QListView*,const QString &,
		QCheckListItem::Type,XIntro* = NULL
	);

	protected:
	virtual void stateChange ( bool );
};

#endif
