/**************
FILE          : card.h
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the XFree86 license
              :
DESCRIPTION   : xapi (X11-Application-Interface)
              : header definitions for card.c
              :
STATUS        : Status: Up-to-date
**************/
#ifndef CARD_H
#define CARD_H 1

#include <qmessagebox.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qfileinfo.h>
#include <qlist.h>
#include <qframe.h>
#include <qlayout.h>
#include <qvbox.h>

#include "../intro.h"
#include "../../xtab.h"
#include "../../xinfo.h"

//=====================================
// Defines...
//-------------------------------------
enum {
    CardPage1 = Card,
};

enum {
	CARD_NEW   = 1,
	CARD_SETUP = 0
};

//=====================================
// Class Headers...
//-------------------------------------
class XCard : public XTemplate {
	Q_OBJECT

	private:
	QFrame*       mDialog;
	XIntro*       mIntro;
	QListBox*     mCardList;
	QDict<XFile>* mFilePtr;
	XTabDialog*   mTop;
	QPushButton*  mAdd;
	QPushButton*  mDel;
	QPushButton*  mSetup;
	QListBox*     mVendor;
	QListBox*     mName;
	QListBox*     mOptions;
	QComboBox*    mBusID;
	QComboBox*    mScrID;
	QCheckBox*    mCheck3D;
	QRadioButton* mRotateNone;
	QRadioButton* mRotateLeft;
	QRadioButton* mRotateRight;
	QButtonGroup* mRotate;
	QVBox*        layer6;
	QVBox*        layer7;
	QLabel*       mPixExpert;

	private:
	int           mSelected;
	int           mState;
	int           mWorkingIndex;
	XFile*        pLayout;
	XFile*        pDesktop;
	XFile*        pCard;
	QString       mTypeString;
	int           mTypeValue;
	QString*      mCardOptions;
	QString*      mCardRawOptions;
	QString*      mModuleList;
	QString*      m3D;
	QString*      m3Dorig;
	QDict<char>   mOptFlags;
	QList<char>   mHiddenRawOptions;
	bool          m3DStateChanged;
	bool          mCheckBoxStatus;
	bool          mShowSetupWindow;
	bool          mCheckType;

    public:
    XCard ( XFrame*,XIntro* );
    XCard ( void );
	void setInputRotateState ( const QString& );
	void setRandRExtension   ( const QString& );
    void addTo ( XFrame* xf,XIntro* xi );
	void showSetupWindow ( bool );
	void check3D     ( bool );
	bool isChecked   ( void );
	bool slotRun     ( int  );
	int  getSelected ( void );

    private:
    void dialogCreate ( void );
    void resetPage    ( int = PAGE_NOLOAD );
    void pageCalled   ( int );
	void initPage     ( void );
	void setupTop     ( int );
	bool validatePage ( void );
	QList<char> get3Dstatus ( void );

	public:
	int setTypedMessage (
		const QString&,XBox::Icon = XBox::Warning,const QString& = 0,int=1
	);
	void setMessage ( 
		const QString&,XBox::Icon = XBox::Warning,const QString& = 0 
	);

    protected:
	void slotIntro ( int );

	public slots:
	void slotAdd       ( void );
	void slot3D        ( void );
	void slotRemove    ( void );
	void slotSetup     ( void );
	void slotSelect    ( QListBoxItem* );
	void slotTopOk     ( void );
	void slotTopCancel ( void );
	void slotVendor    ( QListBoxItem* );
	void slotName      ( QListBoxItem* );
	void slotOption    ( QListBoxItem* );
	void slotBusID     ( int );
	void slotScreenID  ( int );
};

#endif
