/**************
FILE          : layout.cpp
***************
PROJECT       : SaX2 - library interface
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : SaX2 - SuSE advanced X11 configuration 
              : 
              :
DESCRIPTION   : native C++ class library to access SaX2
              : functionality. Easy to use interface for
              : //.../
              : - importing/exporting X11 configurations
              : - modifying/creating X11 configurations 
              : ---
              :
              :
STATUS        : Status: Development
**************/
#include "layout.h"

namespace SaX {
//====================================
// Constructor...
//------------------------------------
SaXManipulateLayout::SaXManipulateLayout (SaXImport* layout,SaXImport* card ) {
	// .../
	//! An object of this type is used to configure the
	//! server layout. This includes mainly the configuration
	//! of the multihead arrangement and the multihead mode
	// ----
	if ((! layout) || (! card)) {
		excNullPointerArgument ();
		qError (errorString(),EXC_NULLPOINTERARGUMENT);
		return;
	}
	if ( layout->getSectionID() != SAX_LAYOUT ) {
		excLayoutImportBindFailed ( layout->getSectionID() );
		qError (errorString(),EXC_LAYOUTIMPORTBINDFAILED);
		return;
	}
	if ( card->getSectionID() != SAX_CARD ) {
		excCardImportBindFailed ( card->getSectionID() );
		qError (errorString(),EXC_CARDIMPORTBINDFAILED);
		return;
	}
	mLayout = layout;
	mCard   = card;
	mLayout -> setID ( 0 );
	mCard   -> setID ( 0 );
}

//====================================
// setMultiheadMode
//------------------------------------
void SaXManipulateLayout::setXOrgMultiheadMode ( int mode ) {
	// .../
	//! set the multihead mode which can be one of the
	//! following settings: SAX_XINERAMA, SAX_TRADITIONAL or
	//! SAX_CLONE
	// ----
	switch (mode) {
		case SAX_XINERAMA:
			mLayout -> setItem ("Clone","off");
			mLayout -> setItem ("Xinerama","on");
		break;
		case SAX_TRADITIONAL:
			mLayout -> setItem ("Clone","off");
			mLayout -> setItem ("Xinerama","off");
		break;
		case SAX_CLONE:
			mLayout -> setItem ("Clone","on");
			mLayout -> setItem ("Xinerama","off");
		break;
		default:
			excInvalidArgument (mode);
			qError (errorString(),EXC_INVALIDARGUMENT);
		break;
	}
}

//====================================
// setLayout
//------------------------------------
bool SaXManipulateLayout::setXOrgLayout (
	int screen, int left, int right, int top, int bottom
) {
	// .../
	//! set the layout for the given screen (screen). The parameters
	//! define the neighbour IDs at the positions left , right , top
	//! and bottom. If there is no neighbour at position X a negative
	//! value must be set
	// ----
	SaXManipulateCard cardData (mCard);
	if (! cardData.selectCard (screen)) {
		return false;
	}
	QString leftID   ("<none>");
	QString rightID  ("<none>");
	QString topID    ("<none>");
	QString bottomID ("<none>");
	if (left >= 0) {
		leftID.sprintf ("Screen[%d]",left);
	}
	if (right >= 0) {
		rightID.sprintf ("Screen[%d]",right);
	}
	if (top >= 0) {
		topID.sprintf ("Screen[%d]",top);
	}
	if (bottom >= 0) {
		bottomID.sprintf ("Screen[%d]",bottom);
	}
	QString screenID; screenID.sprintf ("%d",screen);
	mLayout -> setItem (
		QString("Screen:Screen["+ screenID + "]"),
		QString(leftID + " " + rightID + " " + topID + " " + bottomID)
	);
	return true;
}

//====================================
// getMultiheadMode
//------------------------------------
int SaXManipulateLayout::getMultiheadMode ( void ) {
	// .../
	//! return an identifier for the currently used multihead
	//! mode. Currently there are the following modes:
	//! SAX_TWINVIEW,SAX_TRADITIONAL,SAX_SINGLE_HEAD,SAX_CLONE
	//! and SAX_XINERAMA. The return of SAX_UNKNOWN_MODE should
	//! never happen
	// ----
	QString clone = mLayout -> getItem ("Clone");
	QString xrama = mLayout -> getItem ("Xinerama");
	//====================================
	// check for nvidia TwinView mode...
	//------------------------------------
	if (mCard->getCount() == 1) {
		SaXManipulateCard cardData (mCard);
		for (int card=0;card<mCard->getCount();card++) {
			cardData.selectCard (card);
			QDict<QString> options = cardData.getOptions();
			if (options["TwinView"]) {
				if (*options["TwinView"] == "Clone") {
					return SAX_TWINVIEW_CLONE;
				} else {
					return SAX_TWINVIEW;
				}
			}
			// TODO...
			// more driver specific multihead
			// tactics may follow here
		}
	}
	//====================================
	// check for traditional mode...
	//------------------------------------
	if ((clone == "off") && (xrama == "off") && (mCard->getCount() > 1)) {
		return SAX_TRADITIONAL;
	}
	//====================================
	// check for singlehead mode...
	//------------------------------------
	if ((clone == "off") && (xrama == "off")) {
		return SAX_SINGLE_HEAD;
	}
	//====================================
	// check for clone mode...
	//------------------------------------
	if (clone == "on") {
		return SAX_CLONE;
	}
	//====================================
	// check for Xinerama mode...
	//------------------------------------
	if (xrama == "on") {
		return SAX_XINERAMA;
	}
	return SAX_UNKNOWN_MODE;
}

//====================================
// getLayout
//------------------------------------
QList<QString> SaXManipulateLayout::getXOrgLayout ( int screen ) {
	// .../
	//! return the layout for the given screen (screen). The
	//! returned list has four elements which defines the
	//! (left | right | top | bottom) neighbours of the requested
	//! screen
	// ----
	SaXManipulateCard cardData (mCard);
	if (! cardData.selectCard (screen)) {
		return QList<QString> ();
	}
	QString idString; idString.sprintf ("%d",screen);
	QString layout = mLayout -> getItem (
		QString("Screen:Screen["+ idString + "]")
	);
	if (layout.isEmpty()) {
		excGetScreenLayoutFailed (screen);
		qError (errorString(),EXC_GETSCREENLAYOUTFAILED);
		return QList<QString> ();
	}
	QList<QString> layoutList;
	QStringList positions = QStringList::split ( " ", layout );
	for ( QStringList::Iterator
		in = positions.begin(); in != positions.end(); ++in
	) {
		QString* pos = new QString (*in);
		layoutList.append (pos);
	}
	return layoutList;
}

//====================================
// getInputLayout
//------------------------------------
QList<QString> SaXManipulateLayout::getInputLayout ( void ) {
	// .../
	//! return the layout of the pointer devices. The list
	//! contains the Identifier numbers of the activated devices
	// ----
	QString layout = mLayout -> getItem ("InputDevice");
	if (layout.isEmpty()) {
		excGetInputLayoutFailed ();
		qError (errorString(),EXC_GETINPUTLAYOUTFAILED);
		return QList<QString> ();
	}
	QList<QString> inputList;
	QStringList tokens = QStringList::split ( ",", layout );
	for ( QStringList::Iterator
		in = tokens.begin(); in != tokens.end(); ++in
	) {
		QString* item = new QString (*in);
		QRegExp identifier ("\\[(.+)\\]");
		int pos = identifier.search (*item);
		if (pos >= 0) {
			inputList.append (
				new QString (identifier.cap(1))
			);
		}
	}
	return inputList;
}
} // end namespace
