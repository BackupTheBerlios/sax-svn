/**************
FILE          : keyboardoptions.h
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the GPL license
              :
DESCRIPTION   : SaX2 GUI system using libsax to provide
              : configurations for a wide range of X11 capabilities
              : //.../
              : - keyboardoptions.h: SaXGUI::SCCKeyboardOptions header defs
              : - keyboardoptions.cpp: configure XKB keyboard options
              : ----
              :
STATUS        : Status: Development
**************/
#ifndef SCCKEYBOARD_OPTIONS_H
#define SCCKEYBOARD_OPTIONS_H 1

//=====================================
// Includes
//-------------------------------------
#include <qlistview.h>

//=====================================
// Includes
//-------------------------------------
#include "dialog.h"

namespace SaXGUI {
//====================================
// Class SCCKeyboardOptions
//------------------------------------
class SCCKeyboardOptions : public SCCDialog {
	Q_OBJECT

	private:
	//====================================
	// private dialog data for exporting
	//------------------------------------
	QString          mXKBOption;

	private:
	//====================================
	// private widget data contents
	//------------------------------------
	QDict<QString>   mOptionDict;

	private:
	QBoxLayout*      mMainLayout;

	private:
	QListView*       mXKBOptionView;
	QCheckListItem*  mIDGroupShiftLock;
	QCheckListItem*  mIDKbdLedToShow;
	QCheckListItem*  mIDCompose;
	QCheckListItem*  mIDControl;
	QCheckListItem*  mIDThirdLevel;
	QCheckListItem*  mIDAltWinKey;
	QCheckListItem*  mIDCapsLock;
	QCheckListItem*  mIDCapsLockNone;
	QCheckListItem*	 mIDAltWinKeyNone;
	QCheckListItem*  mIDControlNone;

	private:
	void updateTree ( QListViewItem*,bool );
	QString findOption ( const QString& );
	QString translateOption ( const QString& );

	public:
	QString getOptions     ( void );
	QString getApplyString ( void );

	public:
	void init   ( void );
	void import ( void );

	public slots:
	void slotXKBOption ( QListViewItem* );

	signals:
	void sigApply ( void );

	public:
	SCCKeyboardOptions (
		QDict<QString>*, QDict<SaXImport>,
		int, QWidget*
	);
};
} // end namespace

#endif
