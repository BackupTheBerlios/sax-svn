/**************
FILE          : monitorcard.h
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
              : - monitorcard.h: SaXGUI::SCCMonitor header definitions
              : - monitorcard.cpp: configure monitor system
              : ----
              :
STATUS        : Status: Development
**************/
#ifndef SCCMONITOR_CARD_H
#define SCCMONITOR_CARD_H 1

//=====================================
// Includes
//-------------------------------------
#include <qpushbutton.h>
#include <qtabwidget.h>
#include <qvbox.h>
#include <qcheckbox.h>
#include <qbuttongroup.h>
#include <qpushbutton.h>
#include <qcombobox.h>
#include <qradiobutton.h>
#include <qtabdialog.h>
#include <qlabel.h>
#include <qlistbox.h>
#include <qspinbox.h>

//=====================================
// Includes
//-------------------------------------
#include "dialog.h"

namespace SaXGUI {
//====================================
// Class SCCMonitorCard
//------------------------------------
class SCCMonitorCard : public SCCDialog {
	Q_OBJECT

	private:
	//====================================
	// private dialog data for exporting
	//------------------------------------
	QString        mRotateSetup;
	QDict<QString> mSelectedOptions;
	bool           mNeedImport;
	int            mDisplay;

	private:
	//====================================
	// private widget data contents
	//------------------------------------
	QDict<QString> mProfileDriverOptions;
	QDict<QString> mOptDict;

	private:
	QHBox*         mCardTab;

	private:
	QTabDialog*    mCardDialog;
	QButtonGroup*  mOptionGroup;
	QListBox*      mOption;
	QButtonGroup*  mRotateGroup;
	QRadioButton*  mRotateNot;
	QRadioButton*  mRotateLeft;
	QRadioButton*  mRotateRight;

	public:
	void show ( void );

	public:
	QDict<QString> getOptions ( void );
	int getRotate ( void );
	void setTitle ( const QString& );

	public:
	void init   ( void );
	void import ( void );

	public slots:
	void slotOk     ( void );
	void slotOption ( QListBoxItem* );

	public:
	SCCMonitorCard (
		QDict<QString>*,QDict<SaXImport>,
		const QString&,int,QWidget*
	);
};
} // end namespace
#endif
