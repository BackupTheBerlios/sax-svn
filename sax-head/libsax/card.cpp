/**************
FILE          : card.cpp
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
#include "card.h"

//====================================
// Constructor...
//------------------------------------
SaXManipulateCard::SaXManipulateCard (SaXImport* in, int card) {
	// .../
	// An object of this type is used to manipulate the graphics
	// card settings refering to the X11 Device sections. An
	// import of type SAX_CARD is required with this class
	// ----
	mImport = 0;
	if ( ! in ) {
		excNullPointerArgument ();
		qError (errorString(),EXC_NULLPOINTERARGUMENT);
		return;
	}
	if ( in->getSectionID() != SAX_CARD ) {
		excCardImportBindFailed ( in->getSectionID() );
		qError (errorString(),EXC_CARDIMPORTBINDFAILED);
		return;
	}
	mImport = in;
	mCard   = card;
	mImport -> setID ( mCard );
	mCDBCardModules = 0;
}

//====================================
// set card ID
//------------------------------------
bool SaXManipulateCard::selectCard (int card) {
	// .../
	// select the card device the changes should be applied to
	// if the device does not exist <false> is returned
	// ----
	if (! mImport) {
		return false;
	}
	if (mImport -> setID ( card )) {
		mCard = card;
		return true;
	}
	return false;
}

//====================================
// setCardDriver
//------------------------------------
void SaXManipulateCard::setCardDriver (const QString& driver ) {
	// .../
	// set the card driver to use with the selected device
	// ----
	if (! mImport) {
		return;
	}
	mImport -> setItem ("Driver",driver);
}

//====================================
// setCardOption
//------------------------------------
void SaXManipulateCard::setCardOption (
	const QString& keyword, const QString& value
) {
	// .../
	// set an option to the graphics card used with the
	// currently selected device
	// ----
	if (! mImport) {
		return;
	}
	QString key (keyword);
	QString val (value);
	if (! value) {
		//====================================
		// set bool option
		//------------------------------------
		mImport->setItem ("Option",key);
	} else {
		//====================================
		// set value added option
		//------------------------------------
		mImport->setRawItem ("RawData","Option \""+key+"\"","\""+val+"\"");
	}
}

//====================================
// addCardOption 
//------------------------------------
void SaXManipulateCard::addCardOption (
	const QString& keyword, const QString& value
) {
	// .../
	// add an option to the graphics card used with the
	// currently selected device
	// ----
	if (! mImport) {
		return;
	}
	QString key (keyword);
	QString val (value);
	if (! value) {
		//====================================
		// add bool option
		//------------------------------------
		mImport->addItem ("Option",key);
	} else {
		//====================================
		// add value added option
		//------------------------------------
		mImport->addRawItem ("RawData","Option \""+key+"\"","\""+val+"\"");
	}
}

//====================================
// deleteCardOption
//------------------------------------
void SaXManipulateCard::deleteCardOption (const QString& value) {
	// .../
	// delete an option which has been set to the currently
	// selected device. If the option does not exist nothing
	// will happen
	// ----
	if (! mImport) {
		return;
	}
	QString val (value);
	QString str = mImport->getItem("RawData");
	if (! str.contains ("\""+val+"\"")) {
		//====================================
		// delete bool option
		//------------------------------------
		mImport->deleteItem ("Option",val);
	} else {
		//====================================
		// delete value added option
		//------------------------------------
		mImport->deleteRawItem ("RawData","Option \""+val+"\"");
	}
}

//====================================
// setBusID
//------------------------------------
void SaXManipulateCard::setBusID  (const QString& busid) {
	// .../
	// set busID location for the selected graphics device
	// ----
	if (! mImport) {
		return;
	}
	mImport -> setItem ("BusID",busid);
}

//====================================
// setScreenID
//------------------------------------
void SaXManipulateCard::setScreenID (int screen) {
	// .../
	// set screen identification for the selected graphics device
	// this is only useful for multihead cards to separate the
	// heads
	// ----
	if (! mImport) {
		return;
	}
	QString screenID; screenID.sprintf ("%d",screen);
	mImport -> setItem ("Screen",screenID);
}

//====================================
// getCardName
//------------------------------------
QString SaXManipulateCard::getCardName (void) {
	// .../
	// retrieve the card vendor and name as one string
	// separated by a colon from the currently selected card
	// ----
	if (! mImport) {
		return QString();
	}
	QString fullName;
	QTextOStream (&fullName)
		<< mImport -> getItem ("Vendor") << ":"
		<< mImport -> getItem ("Name");
	return fullName;
}

//====================================
// getBusID
//------------------------------------
QString SaXManipulateCard::getBusID (void) {
	// .../
	// retrieve the cards busID location from the currently
	// selected card
	// ----
	if (! mImport) {
		return QString();
	}
	return mImport -> getItem ("BusID");
}

//====================================
// getScreenID
//------------------------------------
QString SaXManipulateCard::getScreenID (void) {
	// .../
	// retrieve the card screen identification from the currently
	// selected card. This value is always 0 for non multihead cards
	// ----
	if (! mImport) {
		return QString();
	}
	return mImport -> getItem ("Screen");
}

//====================================
// getOptions
//------------------------------------
QDict<QString> SaXManipulateCard::getOptions (void) {
	// .../
	// retrieve an option list of all options set for the
	// selected card. The storage is a dictionary saving the
	// option name as key and the options value as value for
	// this key. If there is no value set for the option
	// (bool options) the value for the key is the (null) string
	// ----
	if (! mImport) {
		return QDict<QString>();
	}
	QDict<QString> result;
	QString stdOptions = mImport -> getItem ("Option");
	QString rawOptions = mImport -> getItem ("RawData");
	//====================================
	// handle bool options
	//------------------------------------
	QStringList stdlist = QStringList::split ( ",", stdOptions );
	for ( QStringList::Iterator
		in = stdlist.begin(); in != stdlist.end(); ++in
	) {
		QString item (*in);
		result.insert (item,new QString);
	}
	//====================================
	// handle value added options
	//------------------------------------
	QStringList rawlist = QStringList::split ( "Option", rawOptions );
	for ( QStringList::Iterator
		in = rawlist.begin(); in != rawlist.end(); ++in
	) {
		QString item (*in);
		QRegExp itemExp ("\"(.*)\" \"(.*)\"");
		int pos = itemExp.search (item);
		if (pos >= 0) {
			QString key = itemExp.cap(1);
			QString* val = new QString (itemExp.cap(2));
			result.insert (key,val);
		}
	}
	return result;
}

//====================================
// getCardDrivers
//------------------------------------
QList<QString> SaXManipulateCard::getCardDrivers ( void ) {
	// .../
	// retrieve a list of X11 card driver names. The items
	// if the list can be used to retrieve the coresponding
	// option list using the getCardOptions() method
	// ----
	mCDBCardDrivers.clear();
	if ( ! mCDBCardModules ) {
		mCDBCardModules = new SaXProcess ();
		mCDBCardModules -> start (CDB_CARDMODULES);
	}
	QDict< QDict<QString> > CDBModules = mCDBCardModules->getTablePointerCDB();
	QDictIterator< QDict<QString> > it (CDBModules);
	for (; it.current(); ++it) {
		mCDBCardDrivers.append (new QString(it.currentKey()));
	}
	return mCDBCardDrivers;
}

//====================================
// getCardOptions
//------------------------------------
QDict<QString> SaXManipulateCard::getCardOptions ( const QString& driver ) {
	// .../
	// retrieve a list of card options for the given driver
	// name <driver>. A list of drivers can be obtained using the
	// getCardDrivers() member method
	// ----
	mCDBCardOptions.clear();
	if ( ! mCDBCardModules ) {
		mCDBCardModules = new SaXProcess ();
		mCDBCardModules -> start (CDB_CARDMODULES);
	}
	QList< QDict<QString> > opts;
	opts = mCDBCardModules -> getTablePointerCDB_DATA (
		driver
	);
	if (opts.isEmpty()) {
		excCDBRecordNotFound (driver);
		qError (errorString(),EXC_CDBRECORDNOTFOUND);
		return QDict<QString>();
	}
	mCDBCardOptions = *opts.at(0);
	return mCDBCardOptions;
}
