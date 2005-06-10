/**************
FILE          : touchselection.h
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
              : - touchselection.h: SaXGUI::SCCTabletSelection header defs
              : - touchselection.cpp: configure touchscreens
              : ----
              :
STATUS        : Status: Development
**************/
#ifndef SCCTOUCH_SELECTION_H
#define SCCTOUCH_SELECTION_H 1

//=====================================
// Includes
//-------------------------------------
#include <qcheckbox.h>
#include <qlabel.h>
#include <qbuttongroup.h>
#include <qlistbox.h>
#include <qcombobox.h>

//=====================================
// Includes
//-------------------------------------
#include "dialog.h"

namespace SaXGUI {
//====================================
// Class SCCTouchSelection
//------------------------------------
class SCCTouchSelection : public SCCDialog {
	Q_OBJECT

	private:
	//====================================
	// private dialog data for exporting
	//------------------------------------
	bool           mEnabled;
	int            mDisplay;

	private:
	//====================================
	// private widget data contents
	//------------------------------------
	QList<QString> mCDBTouchScreenVendors;
	QList<QString> mCDBTouchScreenModels;

	private:
	SaXManipulateTouchscreens* mSaxToucher;

	private:
	QBoxLayout*      mMainLayout;

	private:
	QCheckBox*       mCheckEnable;
	QButtonGroup*    mModelVendorGroup;
	QListBox*        mVendorList;
	QListBox*        mModelList;
	QButtonGroup*    mPortGroup;
	QComboBox*       mPortBox;

	public:
	QString getVendor   ( void );
	QString getModel    ( void );
	QString getPortName ( void );
	bool isEnabled      ( void );

	public:
	void init   ( void );
	void import ( void );

	public slots:
	void slotActivateToucher ( void );
	void slotVendor ( QListBoxItem* );
	void slotName   ( QListBoxItem* );

	signals:
	void sigActivate ( bool );

	public:
	SCCTouchSelection (
		QDict<QString>*, QDict<SaXImport>,
		int, QWidget*
	);
};
} // end namespace

#endif
