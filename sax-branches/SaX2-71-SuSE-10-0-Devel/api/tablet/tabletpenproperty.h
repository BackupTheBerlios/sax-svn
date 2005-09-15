/**************
FILE          : tabletpenproperty.h
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
              : - tabletpenproperty.h: SaXGUI::SCCTabletPenProperty headers
              : - tabletpenproperty.cpp: configure tablets
              : ----
              :
STATUS        : Status: Development
**************/
#ifndef SCCTABLET_PEN_PROPERTY_H
#define SCCTABLET_PEN_PROPERTY_H 1

//=====================================
// Includes
//-------------------------------------
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qlistbox.h>
#include <qpushbutton.h>
#include <qtabdialog.h>
#include <qvbox.h>

//=====================================
// Includes
//-------------------------------------
#include "dialog.h"

namespace SaXGUI {
//====================================
// Class SCCTabletPenProperty
//------------------------------------
class SCCTabletPenProperty : public SCCDialog {
	Q_OBJECT

	private:
	//====================================
	// private dialog data for exporting
	//------------------------------------
	QDict<QString> mSelectedOptions;
	int            mPenMode;
	bool           mNeedImport;
	int            mDisplay;

	private:
	//====================================
	// private widget data contents
	//------------------------------------
	QDict<QString> mOptionDict;

	private:
	SaXManipulateTablets* mSaxTablet;

	private:
	QVBox*         mTabletPenTab;

	private:
	QTabDialog*    mTabletPenDialog;
	QButtonGroup*  mModeGroup;
	QRadioButton*  mRelative;
	QRadioButton*  mAbsolute;
	QButtonGroup*  mOptionGroup;
	QListBox*      mOptionList;

	public:
	void show ( void );
	
	public:
	int getPenMode ( void );
	QDict<QString> getPenOptions ( void );
	void setupPen ( const QString&,const QString& );
	void setID ( int );
	int getID ( void );

	public:
	void init   ( void );
	void import ( void );

	public slots:
	void slotOk     ( void );
	void slotOption ( QListBoxItem* );

	public:
	SCCTabletPenProperty (
		QDict<QString>*,QDict<SaXImport>,
		const QString&,int,QWidget*
	);
};
} // end namespace
#endif
