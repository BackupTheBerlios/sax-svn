/**************
FILE          : desktop.cpp
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
#include "desktop.h"

//====================================
// Constructor...
//------------------------------------
SaXManipulateDesktop::SaXManipulateDesktop (
	SaXImport* desktop, SaXImport* card , SaXImport* path, int desktopID
) {
	// .../
	// An object of this type is used to configure desktop
	// properties like resolution and colors
	// ----
	if ( (! desktop) || (! card) ) {
		excNullPointerArgument ();
		qError (errorString(),EXC_NULLPOINTERARGUMENT);
		return;
	}
	if ( desktop->getSectionID() != SAX_DESKTOP ) {
		excDesktopImportBindFailed ( desktop->getSectionID() );
		qError (errorString(),EXC_DESKTOPIMPORTBINDFAILED);
		return;
	}
	if ( card->getSectionID() != SAX_CARD ) {
		excCardImportBindFailed ( card->getSectionID() );
		qError (errorString(),EXC_CARDIMPORTBINDFAILED);
		return;
	}
	if ( path->getSectionID() != SAX_PATH ) {
		excPathImportBindFailed ( path->getSectionID() );
		qError (errorString(),EXC_PATHIMPORTBINDFAILED);
		return;
	}
	mDesktopID = desktopID;
	mDesktop   = desktop;
	mCard      = card;
	mPath      = path;
	mDesktop -> setID ( mDesktopID );
	mCard    -> setID ( mDesktopID );
	mPath    -> setID ( 0 );
	mCDBMonitors = 0;
}

//====================================
// set desktop ID
//------------------------------------
bool SaXManipulateDesktop::selectDesktop (int desktop) {
	// .../
	// select the desktop device the changes should be applied to
	// if the device does not exist <false> is returned
	// ----
	if ((! mDesktop) || (! mCard) || (! mPath)) {
		return false;
	}
	if ( (mDesktop->setID (desktop)) && (mCard->setID (desktop)) ) {
		mDesktopID = desktop;
		return true;
	}
	return false;
}

//====================================
// calculateModelines
//------------------------------------
void SaXManipulateDesktop::calculateModelines (bool calc) {
	// .../
	// enable/disable the modeline calculation. This is
	// onyl needed for non VESA modes or modes which are not
	// part of the internal X-Server mode pool
	// ----
	if ((! mDesktop) || (! mCard) || (! mPath)) {
		return;
	}
	QString val ("no");
	if (calc) {
		val = "on";
	}
	mDesktop -> setItem ( "CalcModelines",val );
}

//====================================
// setResolution
//------------------------------------
void SaXManipulateDesktop::setResolution (int c,int x,int y) {
	// .../
	// set resolution <X>x<Y> for colordepth <c>. Using this
	// member will overwrite the current resolution setting
	// ----
	if ((! mDesktop) || (! mCard) || (! mPath)) {
		return;
	}
	QString key;
	QString val;
	key.sprintf ("Modes:%d",c);
	val.sprintf ("%dx%d",x,y);
	mDesktop -> setItem ( key,val );
}

//====================================
// addResolution
//------------------------------------
void SaXManipulateDesktop::addResolution (int c,int x,int y) {
	// .../
	// add resolution <X>x<Y> for colordepth <c>. Using this
	// member will add the resolution to the current setting
	// ----
	if ((! mDesktop) || (! mCard) || (! mPath)) {
		return;
	}
	QString key;
	QString val;
	key.sprintf ("Modes:%d",c);
	if (! mDesktop -> getItem (key).isEmpty()) {
		val = mDesktop -> getItem (key);
	}
	QTextOStream (&val) << val << "," << x << "x" << y;
	mDesktop -> setItem ( key,val );
}

//====================================
// deleteResolution
//------------------------------------
void SaXManipulateDesktop::deleteResolution (int c,int x,int y) {
	// .../
	// delete the resolution <X>x<Y> for colordepth <c> from
	// the current Modes value
	// ----
	if ((! mDesktop) || (! mCard) || (! mPath)) {
		return;
	}
	QString key;
	QString val;
	key.sprintf ("Modes:%d",c);
	val.sprintf ("%dx%d",x,y);
	if (! mDesktop -> getItem (key).isEmpty()) {
		mDesktop -> deleteItem (key,val);
	}
}

//====================================
// setVirtualResolution
//------------------------------------
void SaXManipulateDesktop::setVirtualResolution (int c,int x,int y) {
	// .../
	// set virtual resolution <X>x<Y> for colordepth <c>. This
	// call will overwrite the current setting
	// ----
	if ((! mDesktop) || (! mCard) || (! mPath)) {
		return;
	}
	QString key;
	QString val;
	key.sprintf ("Virtual:%d",c);
	val.sprintf ("%dx%d",x,y);
	mDesktop -> setItem ( key,val );
}

//====================================
// deleteVirtualResolution
//------------------------------------
void SaXManipulateDesktop::deleteVirtualResolution (int c) {
	// .../
	// delete the virtual resolution <X>x<Y> set for
	// colordepth <c>.
	// ----
	if ((! mDesktop) || (! mCard) || (! mPath)) {
		return;
	}
	QString key;
	key.sprintf ("Virtual:%d",c);
	if (! mDesktop -> getItem (key).isEmpty()) {
		mDesktop -> setItem (key,"");
	}
}

//====================================
// setColorDepth
//------------------------------------
void SaXManipulateDesktop::setColorDepth (int c) {
	// .../
	// set the default color depth. Valid values are
	// 4,8,15,16 and 24
	// ----
	if ((! mDesktop) || (! mCard) || (! mPath)) {
		return;
	}
	QString color;
	color.sprintf ("%d",c);
	mDesktop -> setItem ( "ColorDepth",color );
}

//====================================
// is3DCard
//------------------------------------
bool SaXManipulateDesktop::is3DCard (void) {
	// .../
	// check if the card can be used with 3D hardware
	// acceleration according to the CDB information
	// ----
	if ((! mDesktop) || (! mCard) || (! mPath)) {
		return false;
	}
	//====================================
	// retrieve CDB card list
	//------------------------------------
	SaXProcess* CDBCards = new SaXProcess ();
	CDBCards -> start (CDB_CARDS);
	QDict< QDict<QString> > CDBData = CDBCards -> getTablePointerCDB ();
	SaXManipulateCard cardInfo (mCard);
	QString cardName = cardInfo.getCardName();
	QDict<QString>* cardData = CDBData.find ( cardName );
	if ( cardData ) {
		QString* flag = cardData -> find ("Flag");
		if (flag) {
			return true;
		}
		return false;
	}
	//====================================
	// No CDB record found for cardName
	//------------------------------------
	excEmptyCDBGroup ( cardName.data() );
	qError (errorString(),EXC_EMPTYCDBGROUP);
	return false;
}

//====================================
// enable3D
//------------------------------------
bool SaXManipulateDesktop::enable3D (void) {
	// .../
	// enable 3D system. This method will include the needed
	// extension module to activate 3D. It is necessary to restart
	// the X-Server after the configuration has been written. The
	// method will take care about the special nvidia setup which
	// requires the nvidia installer to uninstall -> disable or
	// install -> enable the 3D subsystem. NOTE: libsax will not
	// install or uninstall anything but an exception will be throwed
	// if the binary nvidia driver is not installed
	// ----
	if ((! mDesktop) || (! mCard) || (! mPath)) {
		return false;
	}
	//====================================
	// retrieve CDB card list
	//------------------------------------
	SaXProcess* CDBCards = new SaXProcess ();
	CDBCards -> start (CDB_CARDS);
	QDict< QDict<QString> > CDBData = CDBCards -> getTablePointerCDB ();

	//====================================
	// get Extensions for the active card
	//------------------------------------
	SaXManipulateCard cardInfo (mCard);
	QString cardName = cardInfo.getCardName();
	QDict<QString>* cardData = CDBData.find ( cardName );
	if ( cardData ) {
		QString* driver  = cardData -> find ("3DDriver");
		QString* extends = cardData -> find ("Extension");
		QString* flag    = cardData -> find ("Flag");
		//========================================
		// check 3D flag
		//----------------------------------------
		if (! flag) {
			return false;
		}		
		//========================================
		// nvidia drv. needed, check if installed
		//----------------------------------------
		if ((driver) && (*driver == "nvidia")) {
			QProcess* proc = new QProcess ();
			proc -> addArgument ( XSLOAD );
			proc -> addArgument ( "-vendor" );
			if ( ! proc -> start() ) {
				excProcessFailed();
				qError (errorString(),EXC_PROCESSFAILED);
			}
			while (proc->isRunning()) {
				usleep (1000);
			}
			QByteArray data = proc->readStdout();
			QStringList lines = QStringList::split ("\n",data);
			for ( QStringList::Iterator
				in = lines.begin(); in != lines.end(); ++in
			) {
				QString line (*in);
				if (line = "nvidia:NVIDIA Corporation") {
					return true;
				} 
			}
			excNvidiaDriverMissing();
			qError (errorString(),EXC_NVIDIADRIVERMISSING);
			return false;
		}
		//========================================
		// have extension, add it
		//----------------------------------------
		if (extends) {
			SaXManipulatePath pathInfo (mPath);
			pathInfo.deleteLoadableModule (*extends);
			pathInfo.addLoadableModule (*extends);
		}
		//========================================
		// set driver to use with 3D
		//----------------------------------------
		if (driver) {
			cardInfo.setCardDriver (*driver);
			return true;
		}
		// should not happen...
		return false;
	}
	//====================================
	// No CDB record found for cardName
	//------------------------------------
	excEmptyCDBGroup ( cardName.data() );
	qError (errorString(),EXC_EMPTYCDBGROUP);
	return false;
}

//====================================
// disable3D
//------------------------------------
bool SaXManipulateDesktop::disable3D (void) {
	// ....
	// remove the needed extension module(s) related to the
	// 3D subsystem. This method will handle the special nvidia
	// case as well. An exception will be throwed if the nvidia
	// driver is installed and needs to get uninstalled to
	// deactivate 3D in this case
	// ----
	if ((! mDesktop) || (! mCard) || (! mPath)) {
		return false;
	}
	//====================================
	// retrieve CDB card list
	//------------------------------------
	SaXProcess* CDBCards = new SaXProcess ();
	CDBCards -> start (CDB_CARDS);
	QDict< QDict<QString> > CDBData = CDBCards -> getTablePointerCDB ();

	//====================================
	// get Extensions for the active card
	//------------------------------------
	SaXManipulateCard cardInfo (mCard);
	QString cardName = cardInfo.getCardName();
	QDict<QString>* cardData = CDBData.find ( cardName );
	if ( cardData ) {
		QString* driver  = cardData -> find ("3DDriver");
		QString* driver2D= cardData -> find ("Driver");
		QString* extends = cardData -> find ("Extension");
		QString* flag    = cardData -> find ("Flag");
		//========================================
		// check 3D flag
		//----------------------------------------
		if (! flag) {
			return false;
		}
		//========================================
		// nvidia drv. needed, check if installed
		//----------------------------------------
		if ((driver) && (*driver == "nvidia")) {
			QProcess* proc = new QProcess ();
			proc -> addArgument ( XSLOAD );
			proc -> addArgument ( "-vendor" );
			if ( ! proc -> start() ) {
				excProcessFailed();
				qError (errorString(),EXC_PROCESSFAILED);
			}
			while (proc->isRunning()) {
				usleep (1000);
			}
			QByteArray data = proc->readStdout();
			QStringList lines = QStringList::split ("\n",data);
			for ( QStringList::Iterator
				in = lines.begin(); in != lines.end(); ++in
			) {
				QString line (*in);
				if (line = "nvidia:NVIDIA Corporation") {
					excNvidiaDriverInstalled();
					qError (errorString(),EXC_NVIDIADRIVERINSTALLED);
					return false;
				}
			}
			return true;
		}
		//========================================
		// have extension, delete it
		//----------------------------------------
		if (extends) {
			SaXManipulatePath pathInfo (mPath);
			pathInfo.deleteLoadableModule (*extends);
		}
		//========================================
		// set driver to use with 2D
		//----------------------------------------
		if (driver2D) {
			cardInfo.setCardDriver (*driver2D);
			return true;
		}
		// should not happen...
		return false;
	}
	//====================================
	// No CDB record found for cardName
	//------------------------------------
	excEmptyCDBGroup ( cardName.data() );
	qError (errorString(),EXC_EMPTYCDBGROUP);
	return false;
}

//====================================
// setDisplaySize
//------------------------------------
void SaXManipulateDesktop::setDisplaySize (int width,int height) {
	// .../
	// set the DisplaySize in [mm] for the width and height
	// of the currently selected display
	// ----
	if ((! mDesktop) || (! mCard) || (! mPath)) {
		return;
	}
	QString size;
	size.sprintf ("%d %d",width,height);
	mDesktop -> setItem ( "DisplaySize",size );
}

//====================================
// setHsyncRange
//------------------------------------
void SaXManipulateDesktop::setHsyncRange (double hsmin,double hsmax) {
	// .../
	// set the horizontal sync range in [Khz] of the currently
	// selected display.
	// ----
	if ((! mDesktop) || (! mCard) || (! mPath)) {
		return;
	}
	QString range;
	range.sprintf ("%.1f-%.1f",hsmin,hsmax);
	mDesktop -> setItem ( "HorizSync",range );
}

//====================================
// setVsyncRange
//------------------------------------
void SaXManipulateDesktop::setVsyncRange (double vsmin,double vsmax) {
	// .../
	// set the vertical sync range in [Hz] of the currently
	// selected display.
	// ----
	if ((! mDesktop) || (! mCard) || (! mPath)) {
		return;
	}
	QString range;
	range.sprintf ("%.1f-%.1f",vsmin,vsmax);
	mDesktop -> setItem ( "VertRefresh",range );
}

//====================================
// enableDPMS
//------------------------------------
void SaXManipulateDesktop::enableDPMS (void) {
	// .../
	// enable monitor option DPMS, currently there is only
	// one monitor option so we will set the value and overwrite
	// the current setting
	// ----
	if ((! mDesktop) || (! mCard) || (! mPath)) {
		return;
    }
	mDesktop -> setItem ( "MonitorOptions","DPMS" );
}

//====================================
// disableDPMS
//------------------------------------
void SaXManipulateDesktop::disableDPMS (void) {
	// .../
	// disable monitor option DPMS by removing the complete
	// entry. This works because there is only one option available
	// ----
	if ((! mDesktop) || (! mCard) || (! mPath)) {
		return;
	}
	mDesktop -> setItem ( "MonitorOptions","" );
}

//====================================
// setMonitorVendor
//------------------------------------
void SaXManipulateDesktop::setMonitorVendor (const QString& vendor) {
	// .../
	// set monitor vendor name. This method will only set
	// the vendor string to identify the monitor
	// ----
	if ((! mDesktop) || (! mCard) || (! mPath)) {
		return;
	}
	mDesktop -> setItem ( "VendorName",vendor );
}

//====================================
// setMonitorName
//------------------------------------
void SaXManipulateDesktop::setMonitorName (const QString& name) {
	// .../
	// set the monitor model name. This method will only set
	// the model string to identify the monitor
	// ----
	if ((! mDesktop) || (! mCard) || (! mPath)) {
		return;
	}
	mDesktop -> setItem ( "ModelName",name );
}

//====================================
// is3DEnabled
//------------------------------------
bool SaXManipulateDesktop::is3DEnabled (void) {
	// .../
	// check if 3D is enabled. first we will have a look at
	// the driver used. In case of nvidia 3D is always enabled
	// in any other case the dri module must be used to enable
	// the 3D subsystem
	// ----
	if ((! mDesktop) || (! mCard) || (! mPath)) {
		return false;
	}
	QString driver = mCard -> getItem ("Driver");
	if (driver == "nvidia") {
		return true;
	}
	SaXManipulatePath pathInfo (mPath);
	QList<QString> modules = pathInfo.getModules();
	QListIterator<QString> it (modules);
	for (; it.current();++it) {
	if (*it.current() == "dri") {
		return true;
	}
	}
	return false;
}

//====================================
// getResolutions
//------------------------------------
QList<QString> SaXManipulateDesktop::getResolutions (int color) {
	// .../
	// return a list of resolutions refering to the given
	// color depth. If there is no resolution defined for the given
	// color an empty list is returned
	// ----
	if ((! mDesktop) || (! mCard) || (! mPath)) {
		return QList<QString>();
	}
	QString modes;
	modes.sprintf ("Modes:%d",color);
	QString resolutions = mDesktop -> getItem (modes);
	if (resolutions.isEmpty()) {
		return QList<QString>();
	}
	QList<QString> result;
	QStringList resList = QStringList::split ( ",", resolutions );
	for (QStringList::Iterator it=resList.begin(); it!=resList.end();++ it) {
		QString* data = new QString (*it);
		result.append (data);
	}
	return result;
}

//====================================
// getDisplaySize
//------------------------------------
QList<QString> SaXManipulateDesktop::getDisplaySize (void) {
	// .../
	// get the values for the display size wheras the
	// first entry in the list refers to the X value in mm
	// and the second value refers to the Y value in mm. If
	// there is no display size defined an empty list is
	// returned
	// ----
	if ((! mDesktop) || (! mCard) || (! mPath)) {
		return QList<QString>();
	}
	QString size = mDesktop -> getItem ("DisplaySize");
	if (size.isEmpty()) {
		return QList<QString>();
	}
	QList<QString> result;
	QStringList sizeList = QStringList::split ( " ", size );
	result.append (
		new QString(sizeList.first())
	);
	result.append (
		new QString(sizeList.last())
	);
	return result;
}

//====================================
// getHsyncRange
//------------------------------------
QList<QString> SaXManipulateDesktop::getHsyncRange (void) {
	// .../
	// get the values for the horizontal sync range in Khz
	// the first entry specify the start value and the second
	// value specify the end of the range
	// ----
	if ((! mDesktop) || (! mCard) || (! mPath)) {
		return QList<QString>();
	}
	QString range = mDesktop -> getItem ("HorizSync");
	if (range.isEmpty()) {
		return QList<QString>();
	}
	QList<QString> result;
	QStringList rangeList = QStringList::split ( "-", range );
	result.append (
		new QString(rangeList.first())
	);
	result.append (
		new QString(rangeList.last())
	);
	return result;
}

//====================================
// getVsyncRange
//------------------------------------
QList<QString> SaXManipulateDesktop::getVsyncRange (void) {
	// .../
	// get the values for the vertical sync range in Khz
	// the first entry specify the start value and the second
	// value specify the end of the range
	// ----
	if ((! mDesktop) || (! mCard) || (! mPath)) {
		return QList<QString>();
	}
	QString range = mDesktop -> getItem ("VertRefresh");
	if (range.isEmpty()) {
		return QList<QString>();
	}
	QList<QString> result;
	QStringList rangeList = QStringList::split ( "-", range );
	result.append (
		new QString(rangeList.first())
	);
	result.append (
		new QString(rangeList.last())
	);
	return result;
}

//====================================
// DPMSEnabled
//------------------------------------
bool SaXManipulateDesktop::DPMSEnabled (void) {
	// .../
	// check if DPMS is enabled by checking the value for
	// MonitorOptions. Currently there is only one monitor
	// option available so the check is rather simple
	// ----
	if ((! mDesktop) || (! mCard) || (! mPath)) {
		return false;
	}
	QString options = mDesktop -> getItem ("MonitorOptions");
	if (options == "DPMS") {
		return true;
	}
	return false;
}

//====================================
// getMonitorVendor
//------------------------------------
QString SaXManipulateDesktop::getMonitorVendor (void) {
	// .../
	// get the name of the monitor vendor for the
	// currently selected desktop
	// ----
	if ((! mDesktop) || (! mCard) || (! mPath)) {
		return QString();
	}
	return mDesktop -> getItem ("VendorName");
}

//====================================
// getMonitorName
//------------------------------------
QString SaXManipulateDesktop::getMonitorName (void) {
	// .../
	// get the name of the monitor model for the
	// currently selected desktop
	// ----
	if ((! mDesktop) || (! mCard) || (! mPath)) {
		return QString();
	}
	return mDesktop -> getItem ("ModelName");
}

//====================================
// getColorDepth
//------------------------------------
QString SaXManipulateDesktop::getColorDepth (void) {
	// .../
	// get the currently set default color depth for
	// the active desktop
	// ----
	if ((! mDesktop) || (! mCard) || (! mPath)) {
		return QString();
	}
	return mDesktop -> getItem ("ColorDepth");
}

//====================================
// willCalculateModelines
//------------------------------------
bool SaXManipulateDesktop::willCalculateModelines (void) {
	// .../
	// check if the modeline calculation is enabled or not
	// The method will return true if enabled otherwise false
	// ----
	if ((! mDesktop) || (! mCard) || (! mPath)) {
		return false;
	}
	QString calculate = mDesktop -> getItem ("CalcModelines");
	if ((calculate == "on") || (calculate == "yes")) {
		return true;
	}
	return false;
}

//====================================
//getVirtualResolution
//------------------------------------
QString SaXManipulateDesktop::getVirtualResolution (int color) {
	// .../
	// get the virtual resolution if defined for the given
	// color depth. If there is no virtual resolution a (null)
	// string is returned
	// ----
	if ((! mDesktop) || (! mCard) || (! mPath)) {
		return QString();
	}
	QString key;
	QString val;
	key.sprintf ("Virtual:%d",color);
	val = mDesktop -> getItem (key);
	if (val.isEmpty()) {
		return QString();
	}
	QStringList resList = QStringList::split ( " ", val );
	QString result (resList.join("x"));
	return result;
}

//====================================
// set monitor record from the CDB
//------------------------------------
void SaXManipulateDesktop::setCDBMonitor ( const QString& group ) {
	// .../
	// set all monitor data associated with the given group name to
	// the current desktop data. The group name consists of the
	// vendor and model name separated by a colon
	// ----
	if ( ! mCDBMonitors ) {
		mCDBMonitors = new SaXProcess ();
		mCDBMonitors -> start (CDB_MONITORS);
	}
	QList< QDict<QString> > data;
	data = mCDBMonitors -> getTablePointerCDB_DATA (
		group
	);
	if (data.isEmpty()) {
		excCDBRecordNotFound (group);
		qError (errorString(),EXC_CDBRECORDNOTFOUND);
		return;
	}
	QStringList nameList = QStringList::split ( ":", group );
	setMonitorVendor ( nameList.first() );
	setMonitorName	 ( nameList.last() );
	mDesktop -> merge ( data );
}

//====================================
// get list of CDB monitors
//------------------------------------
QList<QString> SaXManipulateDesktop::getCDBMonitorList ( void ) {
	// .../
	// retrieve a list of supported monitor groups. Each item
	// contains the vendor and the model in its name separated
	// by a colon
	// ----
	mCDBMonitorList.clear();
	if ( ! mCDBMonitors ) {
		mCDBMonitors = new SaXProcess ();
		mCDBMonitors -> start (CDB_MONITORS);
	}
	QDict< QDict<QString> > CDBData = mCDBMonitors -> getTablePointerCDB ();
	QDictIterator< QDict<QString> > it (CDBData);
	for (; it.current(); ++it) {
		mCDBMonitorList.append (new QString (it.currentKey()));
	}
	return mCDBMonitorList;
}
