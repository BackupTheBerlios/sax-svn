/**************
FILE          : action.c
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the GPL license
              :
DESCRIPTION   : xapi (X11-Application-Interface)
              : configure the 3D/OpenGL subsystem
              :
              : --> signal action slots/callbacks
              :
STATUS        : Status: Up-to-date
**************/
#include "opengl.h"

//=====================================
// external globals
//-------------------------------------
extern int global3DActive; 

//=====================================
// XOpenGL reset and switch to Intro
//-------------------------------------
void XOpenGL::resetPage (int reload) {
	// ...
	// this function is called if the OpenGL dialog is finished or 
	// canceled The changes are serialized and re-imported if the user want
	// to conclude the dialog. Otherwhise the original file is imported
	// and the changes will be lost.
	// ---
	QString update = "sys_PATH";

	// ...
	// update the card toplevel window
	// if the 3D status has changed
	// ---
	if (mCheck3DStatusChanged) {
		XCard* cardDialog;
		cardDialog = (XCard*) mIntro -> retrieve (Card);
		cardDialog -> showSetupWindow (true);
		if (reload == PAGE_RELOAD) {
			cardDialog -> slotTopOk();
		} else {
			cardDialog -> slotTopCancel();
		}
	}

	// ...
	// export changes and reload them again
	// ---
	mStatus -> clear();
	if (reload == PAGE_RELOAD) {
		mFilePtr = mIntro->getFiles();
		XWrapFile < QDict<XFile> > mFiles (mFilePtr);
        if (mFiles [update] -> sysSerialize()) {
			mFiles [update] -> isModified ( mFrame );
		}
		update = "sys_CARD";
		if (mFiles [update] -> sysSerialize()) {
			mFiles [update] -> isModified ( mFrame );
		}
	}
	mIntro -> importFile (update);
	slotIntro (mIndex);
	XTemplate::resetPage ();
	mStack -> raiseWidget (Intro);
}

//=====================================
// XOpenGL internal page called...
//-------------------------------------
void XOpenGL::pageCalled (int index) {
	// ...
	// this function is called if the next page of the dialog
	// is called. Note if the dialog has only one page this function
	// will not be called because there is no next page in this case
	// ---
	switch (index) {
	case OpenGLPage1: {
		break;
	}
	};
}

//=====================================
// XOpenGL init page with data infos 
//-------------------------------------
void XOpenGL::initPage (void) {
    // log (L_INFO,"XOpenGL::initPage() called\n");
    // ...
    // this function is called after the page was created.
    // It must only be used to init the widgets contents
    // with the data available for selections and other stuff
    // like that
    // ---
}

//=====================================
// XOpenGL enable or disable 3D feature
//-------------------------------------
void XOpenGL::slot3D (void) {
	// log (L_INFO,"XOpenGL::slot3D() called\n");
	// ...
	// slot called to enable or disable 3D
	// we will call the slot functions implemented with
	// the card dialog because of changes made at the
	// card side
	// ---
	XCard* cardDialog;
	cardDialog = (XCard*) mIntro -> retrieve (Card);
	mCheck3DStatusChanged = true;

	// ...
	// call showSetupWindow with argument false
	// this will cause the following within the 
	// Card dialog:
	// ---
	// 1) prevent displaying toplevel windows
	// 2) prevent XTemplate::slotRun() call
	// 3) prevent setting up text for status bar
	// ---
	cardDialog -> showSetupWindow (false);

	cardDialog -> slotRun (Card);
	cardDialog -> slotSetup();
	if (mCheck3D -> isChecked()) {
		cardDialog -> check3D (true);
	} else {
		cardDialog -> check3D (false);
	}
	cardDialog -> slot3D();
	if (cardDialog->isChecked()) {
		mCheck3D -> setChecked (true);
	} else {
		mCheck3D -> setChecked (false);
	}
}

//=====================================
// XOpenGL virtual slots...
//-------------------------------------
bool XOpenGL::slotRun (int index) {
	// ...
	// this function is called if the OpenGL page is activated.
	// use this function to init the dialog with the current
	// OpenGL setup 
	// ---
	XCard* cardDialog;
	XWrapPointer< QDict<char> > mText (mTextPtr);
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);
	cardDialog = (XCard*) mIntro -> retrieve (
		Card
	);

	if (index == OpenGL) {
		// ===================================================
		// 3D setup algorithm...
		// ---------------------------------------------------
		// The initial state of the 3D checkbox is enabled and
		// not checked. Next is to find out if the currently selected
		// card is capable of using 3D. If the global3DActive variable 
		// is true we will set the 3D box as checked. global3DActive conforms
		// to the exit code of 3Ddiag-result and therefore indicates
		// the currently status of 3D used with this system.
		// ---
		// Now device between a single card or a multihead system.
		// If more than one card is used we will disable the checkbox
		// because 3D will not work in multihead systems. If the checkbox
		// is checked at this point we will show a message about the
		// fact and uncheck the checkbox then. If only a single card
		// is used we had to check if this card is capable of 3D.
		// ---
		mCheck3D -> setChecked  ( false );
		mCheck3D -> setDisabled ( false );
		// ...
		// handle 3D in multihead environments or check
		// for the 3D capability of a single card
		// ---
		int cards = 0;
		for (int n=0; n<=mFiles["sys_CARD"]->getDeviceCount(); n++) {
			XData* data = mFiles["sys_CARD"] -> getDevice(n);
			if (! data) {
				continue;
			}
			cards++;
		}
		if (cards > 1) {
			cardDialog -> setMessage ("no3DinMulti");
			return (false);
		}
		// ...
		// Is the currently used first ! card capable 
		// to use the 3D feature(s)
		// ---
		bool D3Capable = false;
		QDict<char>* spec = NULL;
		QDict<char> card;
		XData* data = mFiles["sys_CARD"] -> getDevice (0);
		if ( data ) {
			card = data -> getData();
		}
		XDb* pCDB = mFiles["cdb_CARDS"]->cdbGetSpec(
			card["Vendor"],card["Name"]
		);
		if (pCDB) { 
		spec = pCDB -> getHash();
		XWrapPointer< QDict<char> > selectedCard ( spec );
		QString flag (selectedCard["Flag"]);
		if (flag == "3D") {
			D3Capable = true;
		}
		}
		if (! D3Capable) {
			cardDialog -> setMessage ("no3Dcard");
			return (false);
		}
	}

	if (XTemplate::slotRun (index)) {
		// log(L_INFO,"XOpenGL::slotRun() called: %d\n",index);
		// ...
		// set checkbox to the currently used 
		// 3D state
		// ---
		mCheck3D -> setDisabled ( false  );
		mCheck3D -> setChecked  ( false );
		if (global3DActive) {
			mCheck3D -> setChecked  ( true );
		} 
		// ...
		// set status message and next button
		// ---
		mStatus -> message (mText["RunXOpenGL"]);
		mFrame  -> nextButton() -> setText (mText["finish"]);
	}
	return (TRUE);
}
