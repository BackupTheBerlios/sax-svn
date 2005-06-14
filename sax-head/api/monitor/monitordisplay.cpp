/**************
FILE          : monitordisplay.cpp
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
              : - monitordisplay.h: SaXGUI::SCCMonitor header definitions
              : - monitordisplay.cpp: configure monitor system
              : ----
              :
STATUS        : Status: Development
**************/
#include "monitordisplay.h"

namespace SaXGUI {
//====================================
// Constructor
//------------------------------------
SCCMonitorDisplay::SCCMonitorDisplay (
	QDict<QString>* text, QDict<SaXImport> section,
	int display, QWidget* parent
) : SCCDialog ( 0,text,section,parent ) {
	//=====================================
	// get translation pointer
	//-------------------------------------
	SCCWrapPointer< QDict<QString> > mText (mTextPtr);

	//=====================================
	// create layout for this widget
	//-------------------------------------
	mMainLayout = new QVBoxLayout ( this );

	//=====================================
	// create macro widgets
	//-------------------------------------
	// toplevel activate check box
	mCheckEnable = new QCheckBox ( mText["ActivateDisplay"],this );
	// first group with card and monitor name
	mCardMonitorGroup = new QButtonGroup (
		1,Horizontal,"",this
	);
	QHBox* cardBox = new QHBox ( mCardMonitorGroup );
	QHBox* moniBox = new QHBox ( mCardMonitorGroup );
	mLabelCardName = new QLabel ( mText["Card"],cardBox );
	mCardOptions   = new QPushButton ( mText["Options"],cardBox);
	cardBox -> setStretchFactor ( mLabelCardName, 20 );
	mLabelMonitorName = new QLabel ( mText["Monitor"],moniBox );
	mChangeMonitor    = new QPushButton ( mText["Change"],moniBox);
	moniBox -> setStretchFactor ( mLabelMonitorName, 20 );
	// second group with properties
	mPropertyGroup = new QButtonGroup (
		1,Vertical,mText["Properties"],this
	);
	QVBox* resBox = new QVBox  ( mPropertyGroup );
	QLabel* space = new QLabel ( mPropertyGroup );
	space -> setFixedWidth ( 10 );
	QVBox* colBox = new QVBox  ( mPropertyGroup );
	mLabelResolution = new QLabel ( mText["Resolution"],resBox );
	resBox -> setSpacing ( 10 );
	mResolution  = new QComboBox ( false, resBox );
	mLabelColors = new QLabel ( mText["Colors"],colBox );
	colBox -> setSpacing ( 10 );
	mColors      = new QComboBox ( false, colBox );
	// third group with dual head mode
	mDualHeadGroup = new QButtonGroup (
		1,Horizontal,mText["DualHeadMode"],this
	);
	QHBox* dualBox = new QHBox ( mDualHeadGroup );
	QHBox* infoBox = new QHBox ( mDualHeadGroup );
	mCheckDualHead = new QCheckBox ( mText["ActivateDualHead"],dualBox );
	mConfigureDualHead = new QPushButton ( mText["Configure"],dualBox );
	dualBox -> setStretchFactor ( mCheckDualHead, 20 );
	QLabel* infoSpace  = new QLabel ( infoBox );
	infoSpace -> setFixedWidth ( 21 );
	mDualHeadInfoLabel = new QLabel ( infoBox );
	
	//=====================================
	// create toplevel dialogs
	//-------------------------------------
	SaXManipulateCard saxCard ( mSection["Card"],display );
	QString cardTitle (saxCard.getCardVendor() + " " + saxCard.getCardModel());
	mCardOptionDialog = new SCCMonitorCard (
		text,section,cardTitle,
		display,this
	);
	QString monitorTitle ( mText["MonitorForCard"]+" "+cardTitle );
	mChangeMonitorDialog = new SCCMonitorModel (
		text,section,monitorTitle,mText["Sync"],mText["DisplaySize"],
		display,this
	);
	QString dualMonitorTitle ( cardTitle );
	mConfigureDualHeadDialog = new SCCMonitorDual (
		text,section,dualMonitorTitle,
		display,this
	);
	//=====================================
	// initialize toplevel dialogs
	//-------------------------------------
	mCardOptionDialog -> init();
	mCardOptionDialog -> import();
	mChangeMonitorDialog -> init();
	mChangeMonitorDialog -> import();
	mConfigureDualHeadDialog -> init();
	mConfigureDualHeadDialog -> import();

	//=====================================
	// set same width for all push buttons
	//-------------------------------------
	QPushButton* thisButtonWidgets[] = {
		mCardOptions,mChangeMonitor,mConfigureDualHead
	};
	int fixedWidth = 0;
	for (int i=0;i<3;i++) {
	if (thisButtonWidgets[i]->width() > fixedWidth) {
		fixedWidth = thisButtonWidgets[i]->width();
	}
	}
	for (int i=0;i<3;i++) {
		thisButtonWidgets[i]->setFixedWidth (fixedWidth);
	}
	//=====================================
	// connect widgets
	//-------------------------------------
	QObject::connect (
		mConfigureDualHead , SIGNAL ( clicked               ( void )),
		this               , SLOT   ( slotConfigureDualHead ( void ))
	);
	QObject::connect (
		mChangeMonitor     , SIGNAL ( clicked               ( void )),
		this               , SLOT   ( slotChangeMonitor     ( void ))
	);
	QObject::connect (
		mCardOptions       , SIGNAL ( clicked               ( void )),
		this               , SLOT   ( slotChangeCardOptions ( void ))
	);
	QObject::connect (
		mCheckEnable       , SIGNAL ( clicked               ( void )),
		this               , SLOT   ( slotActivateDisplay   ( void ))
	);
	QObject::connect (
		mCheckDualHead     , SIGNAL ( clicked               ( void )),
		this               , SLOT   ( slotActivateDualHead  ( void ))
	);
	QObject::connect (
		mResolution        , SIGNAL ( activated             ( int )),
		this               , SLOT   ( slotResolution        ( int ))
	);
	QObject::connect (
		mColors            , SIGNAL ( activated             ( int )),
		this               , SLOT   ( slotColors            ( int ))
	);
	//=====================================
	// add widgets to the layout
	//-------------------------------------
	mMainLayout -> setMargin  ( 15 );
	mMainLayout -> addWidget  ( mCheckEnable );
	mMainLayout -> addSpacing ( 15 );
	mMainLayout -> addWidget  ( mCardMonitorGroup );
	mMainLayout -> addSpacing ( 15 );
	mMainLayout -> addWidget  ( mPropertyGroup );
	mMainLayout -> addSpacing ( 15 );
	mMainLayout -> addWidget  ( mDualHeadGroup );
	mMainLayout -> addStretch ( 10 );

	//=====================================
	// save display number
	//-------------------------------------
	mDisplay = display;
}
//====================================
// init
//------------------------------------
void SCCMonitorDisplay::init ( void ) {
	//====================================
	// create card a. desktop manipulators
	//------------------------------------
	SaXManipulateCard saxCard (
		mSection["Card"]
	);
	SaXManipulateDesktop saxDesktop (
		mSection["Desktop"],mSection["Card"],mSection["Path"]
	);
	//====================================
	// select card and desktop
	//------------------------------------
	saxDesktop.selectDesktop ( mDisplay );
	saxCard.selectCard ( mDisplay );

	//====================================
	// get virtual card count
	//------------------------------------
	int usedDevices = saxCard.getDevices();

	//====================================
	// check activate box
	//------------------------------------
	mCheckEnable -> setChecked ( true );
	if (usedDevices == 1) {
		mCheckEnable -> hide();
		mEnabled = true;
	} else {
		mCheckEnable -> setChecked ( true );
		mEnabled = true;
	}
	//====================================
	// check multihead group
	//------------------------------------
	if (! saxCard.isNoteBook()) {
		if (! saxDesktop.isDualHeadCard()) {
			mDualHeadGroup -> hide();
		}
	} else {
		if (saxDesktop.getDualHeadProfile().isEmpty()) {
			mDualHeadGroup -> setDisabled ( true );
		}
	}
	//====================================
	// insert available resolutions
	//------------------------------------
	SCCFile resHandle ( RES_FILE );
	mResolutionDict = resHandle.readDict();
	QDict<QString> metaResolution;
	long sortResolution[mResolutionDict.count()];
	QDictIterator<QString> ir (mResolutionDict);
	int count = 0;
	for (; ir.current(); ++ir) {
		QStringList tokens = QStringList::split ( "x", ir.currentKey() );
		int x = tokens.first().toInt();
		int y = tokens.last().toInt();
		long pixels = x * y;
		QString* key = new QString;
		QTextOStream (key) << pixels;
		metaResolution.insert (*key,ir.current());
		sortResolution[count] = pixels;
		count++;
	}
	for (int n=0;n<count;n++) {
	for (int i=n+1;i<count;i++) {
		if (sortResolution[n] > sortResolution[i]) {
			long save = sortResolution[n];
			sortResolution[n] = sortResolution[i];
			sortResolution[i] = save;
		}
	}	
	}
	for (int n=0;n<count;n++) {
		QString key;
		QTextOStream (&key) << sortResolution[n];
		mResolution -> insertItem ( *metaResolution[key] );
	}
	//====================================
	// insert available colors
	//------------------------------------
	SCCFile colHandle ( COL_FILE );
	mColorDict = colHandle.readDict();
	QDictIterator<QString> ic (mColorDict);
	for (; ic.current(); ++ic) {
		mColors -> insertItem ( *ic.current() );
	}
}
//====================================
// import
//------------------------------------
void SCCMonitorDisplay::import ( void ) {
	//=====================================
	// get translation pointer
	//-------------------------------------
	SCCWrapPointer< QDict<QString> > mText (mTextPtr);

	//====================================
	// create needed manipulators
	//------------------------------------
	SaXManipulateCard saxCard (
		mSection["Card"]
	);
	SaXManipulateDesktop saxDesktop (
		mSection["Desktop"],mSection["Card"],mSection["Path"]
	);
	//====================================
	// select card and desktop
	//------------------------------------
	saxDesktop.selectDesktop ( mDisplay );
	saxCard.selectCard ( mDisplay );

	//====================================
	// get options defined for the card
	//------------------------------------
	QDict<QString> mOptions = saxCard.getOptions();

	//====================================
	// handle card name
	//------------------------------------
	mCardModel  = saxCard.getCardModel();
	mCardVendor = saxCard.getCardVendor();

	//====================================
	// handle monitor name
	//------------------------------------
	mMonitorModel  = saxDesktop.getMonitorName();
	mMonitorVendor = saxDesktop.getMonitorVendor();

	//====================================
	// handle color depth
	//------------------------------------
	mSelectedColor = saxDesktop.getColorDepth().toInt();

	//====================================
	// handle resolution list
	//------------------------------------
	mSelectedResolution = saxDesktop.getResolutions (mSelectedColor);

	//====================================
	// handle dualhead mode
	//------------------------------------
	if (mOptions["SaXDualHead"]) {
		mCheckDualHead -> setChecked (true);
		mConfigureDualHead -> setDisabled (false);
		QString* dualMode = mOptions["SaXDualMode"];
		QString* dualResolution = mOptions["SaXDualResolution"];
		if ((*dualMode != "off") && ( *dualResolution != "off")) {
			QString translation = mText["ActivateInfo"];
			translation.replace (
				QRegExp("\%1"),*dualMode
			);
			translation.replace (
				QRegExp("\%2"),*dualResolution
			);
			mDualInfo = translation;
		} else {
			mDualInfo = mText["Activated"];
		}
	} else {
		mDualHeadInfoLabel -> setDisabled (true);
		mConfigureDualHead -> setDisabled (true);
		mDualInfo = mText["NoConfigAvailable"];
	}
	//====================================
	// set widgets to imported data
	//------------------------------------
	// set card and monitor name...
	mLabelCardName -> setText (
		mText["Card"]+":\t "+mCardVendor+" "+mCardModel
	);
	if (mMonitorVendor.contains ("Unknown",false) ||
	   (mMonitorModel.contains("Unknonw",false))
	) {
		mLabelMonitorName -> setText (
			mText["Monitor"]+":\t "+mText["UnknownMonitorPleaseChange"]
		);
	} else {
		mLabelMonitorName -> setText (
			mText["Monitor"]+":\t "+mMonitorVendor+" "+mMonitorModel
		);
	}
	// set tab titles for toplevel dialogs...
	mChangeMonitorDialog -> setTitle (
		mText["MonitorForCard"]+" "+mCardVendor+" "+mCardModel
	);
	mCardOptionDialog -> setTitle (
		mCardVendor+" "+mCardModel
	);
	mConfigureDualHeadDialog -> setTitle (
		mCardVendor+" "+mCardModel
	);
	// set info text for dual mode
	mDualHeadInfoLabel -> setText (mDualInfo);
	// set color depth
	QString colorKey;
	QTextOStream (&colorKey) << mSelectedColor;
	mColors -> setCurrentText (*mColorDict[colorKey]);
	// set primary resolution
	QString resolutionKey = *mSelectedResolution.at(0);
	mResolution -> setCurrentText (*mResolutionDict[resolutionKey]);
}
//====================================
// isEnabled
//------------------------------------
bool SCCMonitorDisplay::isEnabled ( void ) {
	return mEnabled;
}
//====================================
// isDualModeEnabled
//------------------------------------
bool SCCMonitorDisplay::isDualModeEnabled ( void ) {
	return mCheckDualHead->isChecked();
}
//====================================
// isDualHeadDisplay
//------------------------------------
bool SCCMonitorDisplay::isDualHeadDisplay ( void ) {
	return mDualHeadGroup->isShown();
}
//====================================
// getResolution
//------------------------------------
QList<QString> SCCMonitorDisplay::getResolution ( void ) {
	return mSelectedResolution;
}
//====================================
// getColorDepth
//------------------------------------
int SCCMonitorDisplay::getColorDepth ( void ) {
	return mSelectedColor;
}
//====================================
// getMonitorData
//------------------------------------
SCCMonitorModel* SCCMonitorDisplay::getMonitorData ( void ) {
	return mChangeMonitorDialog;
}
//====================================
// getCardData
//------------------------------------
SCCMonitorCard* SCCMonitorDisplay::getCardData ( void ) {
	return mCardOptionDialog;
}
//====================================
// getDualData
//------------------------------------
SCCMonitorDual* SCCMonitorDisplay::getDualData ( void ) {
	return mConfigureDualHeadDialog;
}
//====================================
// getCardName
//------------------------------------
QString SCCMonitorDisplay::getCardName ( void ) {
	return mLabelCardName->text();
}
//====================================
// getMonitorName
//------------------------------------
QString SCCMonitorDisplay::getMonitorName ( void ) {
	return mLabelMonitorName->text();
}
//====================================
// getDisplay
//------------------------------------
int SCCMonitorDisplay::getDisplay ( void ) {
	return mDisplay;
}
//====================================
// getDualPixmap
//------------------------------------
QPixmap SCCMonitorDisplay::getDualPixmap ( void ) {
	if (isDualModeEnabled()) {
		return getDualData()->getLayoutPixmap();
	} else {
		return QPixmap ( FIGURE_SINGLE );
	}
}
//====================================
// setMonitorResolution
//------------------------------------ 
void SCCMonitorDisplay::setMonitorResolution ( const QString& res ) {
	mResolution -> setCurrentText (*mResolutionDict[res]);
	slotResolution ( mResolution->currentItem() );
}
//====================================
// setMonitorName
//------------------------------------
void SCCMonitorDisplay::setMonitorName ( const QString& name ) {
	mLabelMonitorName -> clear();
	mLabelMonitorName -> setText ( name );
}
//====================================
// setDualModeInfo
//------------------------------------
void SCCMonitorDisplay::setDualModeInfo ( const QString& info ) {
	mDualHeadInfoLabel -> clear();
	mDualHeadInfoLabel -> setText ( info );
}
//====================================
// slotConfigureDualHead 
//------------------------------------
void SCCMonitorDisplay::slotConfigureDualHead ( void ) {
	mConfigureDualHeadDialog -> show();
}
//====================================
// slotChangeMonitor
//------------------------------------
void SCCMonitorDisplay::slotChangeMonitor ( void ) {
	mChangeMonitorDialog -> show();
}
//====================================
// slotChangeCardOptions
//------------------------------------
void SCCMonitorDisplay::slotChangeCardOptions ( void ) {
	mCardOptionDialog -> show();
}
//====================================
// setEnabled
//------------------------------------
void SCCMonitorDisplay::setEnabled ( bool status ) {
	mCheckEnable -> setChecked ( status );
	slotActivateDisplay();
}
//====================================
// slotActivateDisplay
//------------------------------------
void SCCMonitorDisplay::slotActivateDisplay ( void ) {
	if (mCheckEnable -> isChecked()) {
		mCardMonitorGroup -> setDisabled (false);
		mPropertyGroup    -> setDisabled (false);
		mDualHeadGroup    -> setDisabled (false);
		SaXManipulateDesktop saxDesktop (
			mSection["Desktop"],mSection["Card"],mSection["Path"]
		);
		saxDesktop.selectDesktop ( mDisplay );
		if (saxDesktop.getDualHeadProfile().isEmpty()) {
			mDualHeadGroup -> setDisabled ( true );
		}
		mEnabled = true;
	} else {
		mCardMonitorGroup -> setDisabled (true);
		mPropertyGroup    -> setDisabled (true);
		mDualHeadGroup    -> setDisabled (true);
		mEnabled = false;
	}
	emit sigActivate();
}
//====================================
// slotActivateDualHead
//------------------------------------
void SCCMonitorDisplay::slotActivateDualHead ( void ) {
	if (mCheckDualHead -> isChecked()) {
		mDualHeadEnabled = true;
		mDualHeadInfoLabel -> setDisabled (false);
		mConfigureDualHead -> setDisabled (false);
	} else {
		mDualHeadEnabled = false;
		mDualHeadInfoLabel -> setDisabled (true);
		mConfigureDualHead -> setDisabled (true);
	}
	emit sigDualModeChanged ( this );
}
//====================================
// slotResolution
//------------------------------------
void SCCMonitorDisplay::slotResolution ( int index ) {
	//====================================
	// find first startup resolution
	//------------------------------------
	QString selected = mResolution->text(index);
	QDictIterator<QString> it (mResolutionDict);
	long basePixels  = 0;
	int  basePixelsX = 0;
	int  basePixelsY = 0;
	mSelectedResolution.clear();
	for (; it.current(); ++it) {
		if (*it.current() == selected) {
			QString* baseResolution = new QString (it.currentKey());
			QStringList tokens = QStringList::split ( "x", *baseResolution );
			int x = tokens.first().toInt();
			int y = tokens.last().toInt();
			basePixels  = x * y;
			basePixelsX = x;
			basePixelsY = y;
			mSelectedResolution.append ( baseResolution );
		}
	}
	//====================================
	// append lower resolutions
	//------------------------------------
	it.toFirst();
	int count = 0;
	QDict<QString> metaResolution;
	long sortResolution[mResolutionDict.count()];
	for (; it.current(); ++it) {
		QString* addResolution = new QString (it.currentKey());
		QStringList tokens = QStringList::split ( "x", *addResolution );
		int x = tokens.first().toInt();
		int y = tokens.last().toInt();
		int pixelSpace = x * y;
		if ((pixelSpace < basePixels) && (x<=basePixelsX) && (y<=basePixelsY)) {
			QString key;
			QTextOStream (&key) << pixelSpace;
			metaResolution.insert ( key,addResolution );
			sortResolution[count] = pixelSpace;
			count++;
		}
	}
	for (int n=0;n<count;n++) {
	for (int i=n+1;i<count;i++) {
		if (sortResolution[n] < sortResolution[i]) {
			long save = sortResolution[n];
			sortResolution[n] = sortResolution[i];
			sortResolution[i] = save;
		}
	}
	}
	for (int n=0;n<count;n++) {
		QString key;
		QTextOStream (&key) << sortResolution[n];
		mSelectedResolution.append ( metaResolution[key] );
	}
}
//====================================
// slotColors
//------------------------------------
void SCCMonitorDisplay::slotColors ( int index ) {
	QString selected = mColors->text(index);
	QDictIterator<QString> it (mColorDict);
	for (; it.current(); ++it) {
		if (*it.current() == selected) {
			mSelectedColor = it.currentKey().toInt();
			break;
		}
	}
}
} // end namespace