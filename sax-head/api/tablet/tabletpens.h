/**************
FILE          : tabletpens.h
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
              : - tabletpens.h: SaXGUI::SCCTabletPens header defs
              : - tabletpens.cpp: configure tablets
              : ----
              :
STATUS        : Status: Development
**************/
#ifndef SCCTABLET_PENS_H
#define SCCTABLET_PENS_H 1

//=====================================
// Includes
//-------------------------------------
#include <qcheckbox.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qbuttongroup.h>

//=====================================
// Includes
//-------------------------------------
#include "dialog.h"
#include "tablet.h"
#include "tabletpenproperty.h"

namespace SaXGUI {
//====================================
// Class SCCTabletPens
//------------------------------------
class SCCTabletPens : public SCCDialog {
	Q_OBJECT

	private:
	//====================================
	// private dialog data for exporting
	//------------------------------------

	private:
	//====================================
	// private widget data contents
	//------------------------------------

	private:
	SaXManipulateTablets* mSaxTablet;

	private:
	QBoxLayout*           mMainLayout;

	private:
	QHBox*                mPenBox;
	QHBox*                mEraserBox;
	QButtonGroup*         mToolGroup;
	QCheckBox*            mCheckPen;
	QCheckBox*            mCheckEraser;
	QPushButton*          mPenProperties;
	QPushButton*          mEraserProperties;
	SCCTabletPenProperty* mPenPropertyDialog;
	SCCTabletPenProperty* mEraserPropertyDialog;

	public:
	bool hasPen    ( void );
	bool hasEraser ( void );

	public:
	void init   ( void );
	void import ( void );

	public slots:
	void slotTablet ( const QString&,const QString& );
	void slotActivatePen ( void );
	void slotActivateEraser ( void );
	void slotEraser ( void );
	void slotPen    ( void );

	public:
	SCCTabletPens (
		QDict<QString>*, QDict<SaXImport>,
		int, QWidget*
	);
};
} // end namespace
#endif
