/**************
FILE          : keyboard.h
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the XFree86 license
              :
DESCRIPTION   : xapi (X11-Application-Interface)
              : header definitions for keyboard.c
              :
STATUS        : Status: Up-to-date
**************/
#ifndef KEYBOARD_H
#define KEYBOARD_H 1

#include <qmessagebox.h>
#include <qtabwidget.h>
#include <qcombobox.h>
#include <qframe.h>
#include <qlayout.h>
#include <qlcdnumber.h>
#include <qslider.h>
#include <qlineedit.h>

#include "../intro.h"
#include "rules.h"
#include <X11/XKBlib.h>

//=====================================
// Defines...
//-------------------------------------
#define XKBDDELAY_DEFAULT 500
#define XKBDRATE_DEFAULT  30

enum {
    KeyboardPage1 = Keyboard
};

//=====================================
// Class Headers...
//-------------------------------------
class XKeyboard : public XTemplate {
	Q_OBJECT

    private:
    QFrame*       mDialog;
	XTabDialog*   mTop;
	QComboBox*    mType;
	QComboBox*    mLayout;
	QComboBox*    mVariant;
	QComboBox*    mAddVariant;
	QListView*    mAddView;
	QPushButton*  mOption;
	QLineEdit*    mTest;
	QComboBox*    mXkbOption[6];
	QLCDNumber*   mRepeatLCD;
	QLCDNumber*   mDelayLCD;
	QSlider*      mRepeat;
	QSlider*      mDelay;

	private:
	XIntro*       mIntro;
	QDict<XFile>* mFilePtr;
	QDict<char>   mLayoutHash;
	QDict<char>   mModelHash;
	QDict<char>   mOptionHash;
	int           mCurrentDelay;
	int           mCurrentKrate;
	int           mViewStatus;
	int           mViewLayout;
	int           mViewKey;
	int           mViewVariant;
	XKeyRules     mRules;

    public:
    XKeyboard  ( XFrame*,XIntro* );
    XKeyboard  ( void );
    void addTo ( XFrame* xf,XIntro* xi );

    private:
    void dialogCreate ( void );
	int  getItem      ( QComboBox* , const QString& );
    void resetPage    ( int = PAGE_NOLOAD );
    void pageCalled     ( int );
	void initPage       ( void );
	bool apply          ( void );
	void setupTop       ( void );
	void updateVariants ( void );
	void validateLayout ( void );

    protected:
    bool slotRun   ( int );
	void slotIntro ( int );

	public slots:
	void slotAddLayout  ( QListViewItem* );
	void slotLayout     ( int  );
	void slotType       ( int  );
	void slotVariant    ( int  );
	void slotAddVariant ( int  );
	void slotOption     ( void );
	void slotTopCancel  ( void );
	void slotTopOk      ( void );
	void autoRepeat     ( void );
};


#endif
