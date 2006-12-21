/**************
FILE          : tabletpadproperty.h
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
              : - tabletpadproperty.h: SaXGUI::SCCTabletPadProperty headers
              : - tabletpadproperty.cpp: configure tablets
              : ----
              :
STATUS        : Status: Development
**************/
#ifndef SCCTABLET_PAD_PROPERTY_H
#define SCCTABLET_PAD_PROPERTY_H 1

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
class SCCTabletPadProperty : public SCCDialog {
	Q_OBJECT

	private:
	//====================================
	// private dialog data for exporting
	//------------------------------------
	int            mDisplay;	

	public:
	void setID ( int );
	int getID ( void );

	public:
	SCCTabletPadProperty (
		QDict<QString>*,QDict<SaXImport>,
		const QString&,int,QWidget*
	);
};
} // end namespace
#endif
