/**************
FILE          : tabletconnection.h
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
              : - tabletconnection.h: SaXGUI::SCCTabletConnection header defs
              : - tabletconnection.cpp: configure tablets
              : ----
              :
STATUS        : Status: Development
**************/
#ifndef SCCTABLET_CONNECTION_H
#define SCCTABLET_CONNECTION_H 1

//=====================================
// Includes
//-------------------------------------
#include <qradiobutton.h>
#include <qlabel.h>
#include <qbuttongroup.h>
#include <qcombobox.h>
#include <qlistbox.h>

//=====================================
// Includes
//-------------------------------------
#include "dialog.h"

namespace SaXGUI {
//====================================
// Class SCCTabletConnection
//------------------------------------
class SCCTabletConnection : public SCCDialog {
	Q_OBJECT

	private:
	//====================================
	// private dialog data for exporting
	//------------------------------------
	QDict<QString>   mSelectedOptions;

	private:
	//====================================
	// private widget data contents
	//------------------------------------
	QDict<QString>   mOptionDict;

	private:
	SaXManipulateTablets* mSaxTablet;

	private:
	QBoxLayout*      mMainLayout;

	private:
	QButtonGroup*    mPortGroup;
	QComboBox*       mPortBox;
	QButtonGroup*    mOptionGroup;
	QListBox*        mOptionList;
	QButtonGroup*    mModeGroup;
	QRadioButton*    mRelative;
	QRadioButton*    mAbsolute;

	public:
	QString getPortName ( void );
	bool isAutoPort     ( void );
	int getTabletMode   ( void );
	QDict<QString> getOptions ( void );

	public:
	void setMode ( const QString& );
	void setPort ( const QString& );
	void setOptions ( const QDict<QString>& );

	public:
	void init   ( void );
	void import ( void );

	public slots:
	void slotTablet ( const QString&,const QString& );
	void slotOption ( QListBoxItem* );

	public:
	SCCTabletConnection (
		QDict<QString>*, QDict<SaXImport>,
		int, QWidget*
	);
};
} // end namespace
#endif
