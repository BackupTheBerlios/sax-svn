/**************
FILE          : mouse.cpp
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
              : - mouse.h: SaXGUI::SCCMouse header definitions
              : - mouse.cpp: configure mouse system
              : ----
              :
STATUS        : Status: Development
**************/
#include "mouse.h"

namespace SaXGUI {
//====================================
// Constructor
//------------------------------------
SCCMouse::SCCMouse (
	QWidgetStack* stack,QDict<QString>* text,
	QDict<SaXImport> section, QWidget* parent
) : SCCDialog ( stack,text,section,parent ) {
	//=====================================
	// get translation pointer
	//-------------------------------------
	SCCWrapPointer< QDict<QString> > mText (mTextPtr);

	//=====================================
	// check for mouse devices
	//-------------------------------------
	mMouse = 0;
	SaXManipulateMice saxMouse ( mSection["Pointers"] );
	saxMouse.selectPointer (SAX_CORE_POINTER);
	for (int i=SAX_CORE_POINTER;i<mSection["Pointers"]->getCount();i+=2) {
		if (saxMouse.selectPointer (i)) {
		if (saxMouse.isMouse()) {
			mMouse++;
		}
		}
	}
	//=====================================
	// create mouse dialog
	//-------------------------------------
	mMouseTab = new QTabWidget ( mDialogFrame );

	//=====================================
	// add widgets to the layout
	//-------------------------------------
	mDialogLayout -> addWidget ( mMouseTab );

	//=====================================
	// create basic mouse display tabs
	//-------------------------------------
	int mouseID = SAX_CORE_POINTER;
	for (int n=0;n<mMouse;n++) {
		QString displayName;
		QTextOStream (&displayName) << mText["Mouse"] << " " << (n + 1);
		SCCMouseDisplay* mouse = new SCCMouseDisplay (
			text,section,mouseID,this
		);
		mMouseTab -> addTab ( mouse, displayName );
		mouse -> init();
		mouse -> import();
		//=====================================
		// connect mouse activate checkbox
		//-------------------------------------
		QObject::connect (
			mouse , SIGNAL (sigActivate  ( void )),
			this  , SLOT   (slotActivate ( void ))
		);
		mMouseDisplay.append ( mouse );
		mouseID += 2;
	}
}
//====================================
// init
//------------------------------------
void SCCMouse::init ( void ) {
	//====================================
	// set init state
	//------------------------------------
	needInitData = false;
}
//====================================
// import
//------------------------------------
void SCCMouse::import ( void ) {
	//====================================
	// create manipulators
	//------------------------------------
	SaXManipulateLayout saxLayout (
		mSection["Layout"],mSection["Card"]
	);
	//====================================
	// handle mouseDisplay's state
	//------------------------------------	
	int mouseCT = 0;
	int mouseID = SAX_CORE_POINTER;
	SaXManipulateMice saxMouse ( mSection["Pointers"] );
	QList<QString> inputLayout = saxLayout.getInputLayout();
	for (int i=mouseID;i<mSection["Pointers"]->getCount();i+=2) {
		if (saxMouse.selectPointer (mouseID)) {
		if (saxMouse.isMouse()) {
			bool foundID = false;
			QListIterator<QString> it (inputLayout);
			for (; it.current();++it) {
			if (mouseID == it.current()->toInt()) {
				foundID = true;
				break;
			}
			}
			if (! foundID) {
				mMouseDisplay.at(mouseCT)->setEnabled ( false );
			}
		}
		}
		mouseID += 2;
		mouseCT++;
	}
	//====================================
	// set import state
	//------------------------------------
	needImportData = false;
}
//====================================
// slotActivate
//------------------------------------
void SCCMouse::slotActivate ( void ) {
	int count = 0;
	QListIterator<SCCMouseDisplay> it (mMouseDisplay);
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
		SCCMouseDisplay* current; 
		current = (SCCMouseDisplay*)mMouseTab->currentPage();
		mMessageBox -> showMessage();
		current -> setEnabled();
	}
	init();
}
//====================================
// exportData
//------------------------------------
bool SCCMouse::exportData ( void ) {
	int mouseID = 0;
	QListIterator<SCCMouseDisplay> it (mMouseDisplay);
	for (; it.current() ; ++it) {
		//====================================
		// check for mouse display's
		//------------------------------------
		SCCMouseDisplay* display = (SCCMouseDisplay*)it.current();
		//if (display->isEnabled()) {
		//====================================
		// create manipulators...
		//------------------------------------
		SaXManipulateMice saxMouse (
			mSection["Pointers"]
		);
		SaXManipulateLayout saxLayout (
			mSection["Layout"],mSection["Card"]
		);
		//====================================
		// select pointer device
		//------------------------------------
		mouseID = display->getDisplay();
		saxMouse.selectPointer ( mouseID );

		//====================================
		// save mouse selection data
		//------------------------------------
		SCCMouseModel* modelData = display->getModelData();
		QString vendor = modelData -> getVendorName(); 
		QString model  = modelData -> getModelName();
		if (! model.isEmpty() ) {
			QDict<QString> mData = mSection["Pointers"]->getCurrentTable();
			QDictIterator<QString> it (mData);
			for (; it.current() ; ++it) {
				QString key = it.currentKey();
				QString val = *it.current();
				if (key == "Identifier") {
					continue;
				}
				mSection["Pointers"]->removeEntry ( key );
			}
			saxMouse.setMouse ( vendor,model );
		}
		//====================================
		// save 3-Button emulation data
		//------------------------------------
		saxMouse.disable3ButtonEmulation();
		if (display->isButtonEmulationEnabled()) {
			saxMouse.enable3ButtonEmulation();
		}
		//====================================
		// save mouse wheel data
		//------------------------------------
		saxMouse.disableWheel();
		if (display->isWheelEnabled()) {
			saxMouse.enableWheel();
		}
		//====================================
		// save mouse X axis invertation data
		//------------------------------------
		saxMouse.disableXAxisInvertation();
		if (display->isXInverted()) {
			saxMouse.enableXAxisInvertation();
		}
		//====================================
		// save mouse Y axis invertation data
		//------------------------------------
		saxMouse.disableYAxisInvertation();
		if (display->isYInverted()) {
			saxMouse.enableYAxisInvertation();
		}
		//====================================
		// save mouse wheel emulation data
		//------------------------------------
		saxMouse.disableWheelEmulation();
		if (display->isWheelEmulationEnabled()) {
			saxMouse.enableWheelEmulation (display->getWheelButton());
		}
		//====================================
		// save input layout
		//------------------------------------
		saxLayout.removeInputLayout (mouseID);
		if (display->isEnabled()) {
			saxLayout.addInputLayout (mouseID);
		}
		//}
	}
	return true;
}
} // end namespace
