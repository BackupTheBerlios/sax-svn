/**************
FILE          : tabletpadproperty.cpp
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
              : - tabletpadproperty.h: SaXGUI::SCCTabletPadProperty implement.
              : - tabletpadproperty.cpp: configure tablet system
              : ----
              :
STATUS        : Status: Development
**************/
#include "tabletpadproperty.h"

namespace SaXGUI {
//====================================
// Constructor
//------------------------------------
SCCTabletPadProperty::SCCTabletPadProperty (
	QDict<QString>* text, QDict<SaXImport> section,
	const QString& penname, int display,QWidget* parent
) : SCCDialog ( 0,text,section,parent ) {
	// ...
}
//====================================
// getID
//------------------------------------
int SCCTabletPadProperty::getID ( void ) {
	return mDisplay;
}
//====================================
// setID
//------------------------------------
void SCCTabletPadProperty::setID ( int id ) {
	mDisplay = id;
}
} // end namespace
