/**************
FILE          : monitor.cpp
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
              : - monitor.h: SaXGUI::SCCMonitor header definitions
              : - monitor.cpp: configure monitor system
              : ----
              :
STATUS        : Status: Development
**************/
#include "monitor.h"

namespace SaXGUI {
//====================================
// Constructor
//------------------------------------
SCCMonitor::SCCMonitor (
	QWidgetStack* stack, QDict<QString>* text,
	QDict<SaXImport> section, QWidget* parent
) : SCCDialog ( stack,text,section,parent ) {
	//=====================================
	// get translation pointer
	//-------------------------------------
	SCCWrapPointer< QDict<QString> > mText (mTextPtr);

	//=====================================
	// check for graphics cards
	//-------------------------------------
	SaXManipulateCard saxCard ( mSection["Card"] );
	mCard = saxCard.getDevices();

	//=====================================
	// create monitor dialog
	//-------------------------------------
	mMonitorTab = new QTabWidget ( mDialogFrame );
	mCheck3D    = new QCheckBox  ( mText["Activate3D"],mDialogFrame );

	//=====================================
	// connect widgets
	//-------------------------------------
	QObject::connect (
		mCheck3D , SIGNAL (clicked        ( void )),
		this     , SLOT   (slotActivate3D ( void ))
	);
	//=====================================
	// add widgets to the layout
	//-------------------------------------
	mDialogLayout -> addWidget  ( mMonitorTab );
	mDialogLayout -> addSpacing ( 15 );
	mDialogLayout -> addWidget  ( mCheck3D );

	//=====================================
	// create arrangement dialog
	//-------------------------------------
	mMonitorArrange = 0;
	if ( mCard > 1 ) {
		mMonitorArrange = new SCCMonitorArrange ( text,section,this );
	}
	//=====================================
	// create basic display tabs
	//-------------------------------------
	for ( int n=0;n<mCard;n++) {
		QString displayName;
		QTextOStream (&displayName) << mText["Display"] << " " << (n + 1);
		SCCMonitorDisplay* display = new SCCMonitorDisplay (
			text,section,n,this
		);
		mMonitorTab -> addTab (
			display, displayName
		);
		display -> init();
		display -> import();
		//=====================================
		// connect monitors activate checkbox
		//-------------------------------------
		QObject::connect (
			display , SIGNAL (sigActivate  ( void )),
			this    , SLOT   (slotActivate ( void ))
		);
		//=====================================
		// connect monitors dualhead checkbox
		//-------------------------------------
		if ( mMonitorArrange ) {
			QObject::connect (
				display, SIGNAL (
					sigDualModeChanged ( SCCMonitorDisplay* )
				),
				mMonitorArrange, SLOT (
					slotDualModeChanged ( SCCMonitorDisplay* )
				)
			);
		}
		//=====================================
		// connect monitor dual popup slotOk
		//-------------------------------------
		if ( mMonitorArrange ) {
			SCCMonitorDual* mMonitorDual = display->getDualData();
			QObject::connect (
				mMonitorDual, SIGNAL (
					sigDualModeChanged (SCCMonitorDisplay*)
				),
				mMonitorArrange, SLOT (
					slotDualModeChanged ( SCCMonitorDisplay* )
				)
			);
		}
		//=====================================
		// connect monitor model popup slotOk
		//-------------------------------------
		if ( mMonitorArrange ) {
			SCCMonitorModel* mMonitorModel = display->getMonitorData();
			QObject::connect (
				mMonitorModel, SIGNAL (
					sigMonitorChanged  (SCCMonitorDisplay*)
				),
				mMonitorArrange, SLOT (
					slotMonitorChanged (SCCMonitorDisplay*)
				)
			);
		}
		mMonitorDisplay.append ( display );
	}
	//=====================================
	// finish creation of arrangement dialog
	//-------------------------------------
	if ( mMonitorArrange ) {
		mMonitorTab -> addTab (
			mMonitorArrange,mText["Arrange"]
		);
		mMonitorArrange -> init();
		mMonitorArrange -> import();
	}
}
//====================================
// init
//------------------------------------
void SCCMonitor::init ( void ) {
	//====================================
	// create desktop manipulator
	//------------------------------------
	SaXManipulateDesktop saxDesktop (
		mSection["Desktop"],mSection["Card"],mSection["Path"]
	);
	//====================================
	// check 3D box
	//------------------------------------
	bool has3DCapabilities = false;
	bool isMultiheaded     = false;
	for ( int n=0;n<mCard;n++) {
		if ( saxDesktop.selectDesktop (n) ) {
		if ( saxDesktop.is3DCard()) {
			has3DCapabilities = true;
			break;
		}
		}
	}
	int count = 0;
	QListIterator<SCCMonitorDisplay> it (mMonitorDisplay);
	for (; it.current(); ++it) {
		if (it.current()->isEnabled()) {
			count++;
		}
		if (count > 1) {
			isMultiheaded = true;
			break;
		}
	}
	mCheck3D -> setDisabled ( false );
	if ( (! has3DCapabilities) || (isMultiheaded) ) {
		mCheck3D -> setDisabled ( true  );
		mCheck3D -> setChecked  ( false );
	}
	//====================================
	// check monitor arrangement
	//------------------------------------
	if ( mMonitorArrange ) {
		mMonitorTab -> setTabEnabled ( mMonitorArrange,true );
		if (! isMultiheaded) {
			mMonitorTab -> setTabEnabled ( mMonitorArrange,false );
		}
	}
	//====================================
	// set init state
	//------------------------------------
	needInitData = false;
}
//====================================
// import
//------------------------------------
void SCCMonitor::import ( void ) {
	//====================================
	// create manipulators
	//------------------------------------
	SaXManipulateDesktop saxDesktop (
		mSection["Desktop"],mSection["Card"],mSection["Path"]
	);
	SaXManipulateLayout saxLayout (
		mSection["Layout"],mSection["Card"]
	);
	//====================================
	// handle 3D data import
	//------------------------------------
	m3DEnabled = false;
	if (saxDesktop.is3DEnabled()) {
		m3DEnabled = true;
	}
	//====================================
	// handle monitorDisplay's state
	//------------------------------------
	for (int dev=0;dev<mCard;dev++) {
	if (saxLayout.getXOrgLayout ( dev ).isEmpty()) {
		mMonitorDisplay.at(dev)->setEnabled ( false );
	}
	}
	//====================================
	// set widgets to imported data
	//------------------------------------
	mCheck3D -> setChecked ( m3DEnabled );

	//====================================
	// set arrangement if needed
	//------------------------------------
	if ( mMonitorArrange ) {
		mMonitorArrange -> setArrangement ( mCard,mMonitorDisplay );
	}
	//====================================
	// set import state
	//------------------------------------
	needImportData = false;
}
//====================================
// slotActivate
//------------------------------------
void SCCMonitor::slotActivate ( void ) {
	int count = 0;
	QListIterator<SCCMonitorDisplay> it (mMonitorDisplay);
	for (; it.current(); ++it) {
	if (it.current()->isEnabled()) {
		count++;
	}
	}
	if ( count == 0) {
		SCCMessage* mMessageBox = new SCCMessage (
			qApp->mainWidget(), mTextPtr, SaXMessage::OK, "PageFault",
			"MessageCaption", SaXMessage::Warning
		);
		SCCMonitorDisplay* current; 
		current = (SCCMonitorDisplay*)mMonitorTab->currentPage();
		mMessageBox -> showMessage();
		current -> setEnabled();
	}
	if ( mMonitorArrange ) {
		mMonitorArrange -> setCleanArrangement ( count,mMonitorDisplay );
	}
	init();
}
//====================================
// slotActivate3D
//------------------------------------
void SCCMonitor::slotActivate3D ( void ) {
	m3DEnabled = false;
	if (mCheck3D -> isChecked()) {
		m3DEnabled = true;
	}
}
//====================================
// exportData
//------------------------------------
void SCCMonitor::exportData ( void ) {
	int card = 0;
	//====================================
	// save particular monitor data
	//------------------------------------
	QListIterator<SCCMonitorDisplay> it (mMonitorDisplay);
	for (; it.current() ; ++it) {
		//====================================
		// remove current layout
		//------------------------------------
		QString screenID;
		QTextOStream (&screenID) << "Screen:Screen[" << card << "]";
		mSection["Layout"]->removeEntry( screenID );

		//====================================
		// check for monitor display's
		//------------------------------------
		SCCMonitorDisplay* display = (SCCMonitorDisplay*)it.current();
		if (display->isEnabled()) {
			//====================================
			// create manipulators...
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
			saxDesktop.selectDesktop ( card );
			saxCard.selectCard ( card );

			//====================================
			// save resolution list
			//------------------------------------
			QString modesKey;
			int color = display->getColorDepth();
			QTextOStream (&modesKey) << "Modes:" << color;
			mSection["Desktop"] -> removeEntry ( modesKey );
			QList<QString> resList = display->getResolution();
			QListIterator<QString> it ( resList );
			for (; it.current(); ++it) {
				QStringList tokens = QStringList::split ( "x",*it.current() );
				int xaxis = tokens.first().toInt();
				int yaxis = tokens.last().toInt();
				saxDesktop.addResolution (
					color,xaxis,yaxis
				);
			}
			//====================================
			// save color depth
			//------------------------------------
			saxDesktop.setColorDepth ( color );

			//====================================
			// save monitor settings
			//------------------------------------
			SCCMonitorModel* monitorData = display->getMonitorData();
			saxDesktop.setMonitorVendor (monitorData->getVendorName());
			saxDesktop.setMonitorName   (monitorData->getModelName());
			saxDesktop.setHsyncRange (
				(double)monitorData->getHSmin(),
				(double)monitorData->getHSmax()
			);
			saxDesktop.setVsyncRange (
				(double)monitorData->getVSmin(),
				(double)monitorData->getVSmax()
			);
			int sizeX = monitorData->getSizeX();
			int sizeY = monitorData->getSizeY();
			if ((sizeX > 0) && (sizeY > 0)) {
				saxDesktop.setDisplaySize ( sizeX,sizeY );
			}
			saxDesktop.disableDPMS();
			if (monitorData->isDPMSEnabled()) {
				saxDesktop.enableDPMS();
			}
			QString algorithm = saxDesktop.getModelineAlgorithm();
			if (algorithm != "XServerPool") {
				saxDesktop.calculateModelines ( true );
			}
			//====================================
			// save card options
			//------------------------------------
			SCCMonitorCard* cardData = display->getCardData();
			QDict<QString> optList = cardData->getOptions();
			mSection["Card"] -> removeEntry ("Option");
			mSection["Card"] -> removeEntry ("RawData");
			QDictIterator<QString> io (optList);
			for (; io.current(); ++io) {
				saxCard.addCardOption ( io.currentKey(),*io.current());
			}
			int rotateID = cardData->getRotate();
			mSection["Card"] -> removeEntry ("Rotate");
			saxCard.setRotate ( rotateID );

			//====================================
			// check for dual head mode
			//------------------------------------
			QString profile = saxDesktop.getDualHeadProfile();
			if (! profile.isEmpty()) {
				SaXImportProfile* pProfile = new SaXImportProfile ( profile );
				pProfile -> doImport();
				SaXImport* mImport = pProfile -> getImport ( SAX_CARD );
				if ( mImport ) {
					//====================================
					// pick up card profile data
					//------------------------------------
					QDict<QString> profileDriverOptions;
					SaXManipulateCard saxProfileCard ( mImport );
					profileDriverOptions = saxProfileCard.getOptions();
					QDictIterator<QString> it ( profileDriverOptions );

					//====================================
					// delete profile data keys
					//------------------------------------
					for (; it.current(); ++it) {
						printf ("%s\n",it.currentKey().ascii());
						// TODO
					}
					//====================================
					// setup profile data
					//------------------------------------
					if (display->isDualModeEnabled()) {
						it.toFirst();
						// TODO...
					}
				}
			}
		}
		card++;
	}
	//====================================
	// save arrangement
	//------------------------------------
	if ( mMonitorArrange ) {
		//====================================
		// create manipulators...
		//------------------------------------
		SaXManipulateLayout saxLayout (
			mSection["Layout"],mSection["Card"]
		);
		//====================================
		// save multihead state
		//------------------------------------
		int multiheadMode = mMonitorArrange->getMultiheadMode();
		switch (multiheadMode) {
			case 0:
				saxLayout.setXOrgMultiheadMode ( SAX_TRADITIONAL );
			break;
			case 1:
				saxLayout.setXOrgMultiheadMode ( SAX_CLONE );
			break;
			case 2:
				saxLayout.setXOrgMultiheadMode ( SAX_XINERAMA );
			break;
			default:
			break;
		};
		//====================================
		// save XOrg layout
		//------------------------------------
		QDict<QString> arrangeDict = mMonitorArrange->getArrangement();
		QDictIterator<QString> it (arrangeDict);
		for (; it.current(); ++it) {
			QString item (it.currentKey());
			item.replace (QRegExp("Screen:Screen\\["),"");
			item.replace (QRegExp("\\]"),"");
			int index = item.toInt();

			int count = 0;
			int left=-1,right=-1,above=-1,below=-1;
			QStringList tokens = QStringList::split (" ", *it.current());
			for (QStringList::Iterator it=tokens.begin();it!=tokens.end();++it){
				QString value (*it);
				int id = -1;
				if (value != "<none>") {
					value.replace (QRegExp("Screen\\["),"");
					value.replace (QRegExp("\\]"),"");
					id = value.toInt();
				}
				switch (count) {
					case 0: left  = id; break;
					case 1: right = id; break;
					case 2: above = id; break;
					case 3: below = id; break;
					default:
					break;
				}
				count++;
			}
			saxLayout.setXOrgLayout (
				index,left,right,above,below
			);
		}
	}
}
} // end namespace
