/**************
FILE          : action.c
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the XFree86 license
              :
DESCRIPTION   : xapi (X11-Application-Interface)
              : setup the picture geometry of your desktop(s)
              : this will affect the modeline timings which are
              : saved to the configuration if the setup is finished
              :
              : --> signal action slots/callbacks
              :
STATUS        : Status: Up-to-date
**************/
#include "geometry.h"

//===============================================
// predfined function prototypes
//-----------------------------------------------
void removeXFineCache (void);

//===============================================
// XDisplayGeometry reset and switch to Intro
//-----------------------------------------------
void XDisplayGeometry::resetPage (int reload) {
	// ...
	// this function is called if the geometry dialog is finished or canceled
	// The changes are serialized and re-imported if the user want
	// to conclude the dialog. Otherwhise the original file is imported
	// and the changes will be lost.
	// ---
	QString update = "sys_DESKTOP";

	mStatus -> clear();
	if (reload == PAGE_RELOAD) {
		mFilePtr = mIntro->getFiles();
		XWrapFile < QDict<XFile> > mFiles (mFilePtr);
        if (mFiles [update] -> sysSerialize()) {
			mFiles [update] -> isModified ( mFrame );
		}
	}
	mIntro -> importFile (update);
	slotIntro (mIndex);
	XTemplate::resetPage ();
	mStack -> raiseWidget (Intro);
}

//===============================================
// XDisplayGeometry internal page called...
//-----------------------------------------------
void XDisplayGeometry::pageCalled (int index) {
	// ...
	// this function is called if the next page of the dialog
	// is called. Note if the dialog has only one page this function
	// will not be called because there is no next page in this case
	// ---
	switch (index) {
	case GeometryPage1: {
		break;
	}
	};
}

//===============================================
// XDisplayGeometry virtual slots...
//-----------------------------------------------
bool XDisplayGeometry::slotRun (int index) {
	// ...
	// this page does not need to offer a dialog because
	// we will call an external program called XFine here
	// ---
	if (index == Geometry) {
		// log(L_INFO,"XDisplayGeometry::call XFine...\n");
		// ...
		// next is calling the external program called xfine
		// the return code of the program which is written to
		// stdout will tell us how to proceed
		// ---
		//  1: xfine was successful, but the user canceled.
		//	2: xfine was successful. The user selected "Save settings"
		// ---
		bool modelineChanged = false;
		// ...
		// get the mFiles pointer wrapper from the Intro
		// object which has read all the data files
		// ---
		QDict<XFile>* mFilePtr = mIntro->getFiles();
		XWrapFile < QDict<XFile> > mFiles (mFilePtr);
		// ...
		// call xfine now and handle the return value...
		// ---
		removeXFineCache();
		XRunXFine xfineThread;
		xfineThread.start();
		mFrame -> disableInteraction();
		while (xfineThread.running()) {
			qApp->processEvents();
			usleep (50000);
		}
		mFrame -> enableInteraction();
		int code = xfineThread.getReturnCode();
		QString message (xfineThread.getMessage());
		switch (code) {
		case 0:
			log (L_ERROR,
				"XDisplayGeometry::%s\n",message.ascii()
			);
		break;
		case 1:
			modelineChanged = false;
		break;
		case 2:
			modelineChanged = true;
		break;
		}
		// ...
		// if the xfine call has build the modeline cache
		// holding the modeline changes we had to set the
		// ImportXFineCache variable for all desktops
		// ---
		if (modelineChanged) {
			XData* sys = NULL;
			XFile* map = mFiles["sys_DESKTOP"];
			for (int n=0; n < map->getDeviceCount(); n++) {
			sys = map -> getDevice (n);
			if (! sys) {
				continue;
			}
			sys -> setPair ("ImportXFineCache","yes");
			}
			resetPage (
				PAGE_RELOAD
			);
		}
	}

	#if 0
	if (XTemplate::slotRun (index)) {
	log(L_INFO,"XDisplayGeometry::slotRun() called: %d\n",index);
	// ...
	// this function is called if the geometry page is activated.
	// use this function to init the dialog with the current
	// setup of the geometry
	// ---
	mStatus -> message (mText["RunXGeometry"]);
	}
	#endif

	return (TRUE);
}
