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
              : configure the multihead mode (clone,traditional,xinerama)
              : if the environment is multiheaded of course
              :
              : --> signal action slots/callbacks
              :
STATUS        : Status: Up-to-date
**************/
#include "multihead.h"

//=====================================
// external globals
//-------------------------------------
extern int globalFrameWidth;

//=====================================
// XMultihead reset and switch to Intro
//-------------------------------------
void XMultihead::resetPage (int reload) {
	// ...
	// this function is called if the multihead dialog is finished or 
	// canceled The changes are serialized and re-imported if the user want
	// to conclude the dialog. Otherwhise the original file is imported
	// and the changes will be lost.
	// ---
	QString update = "sys_LAYOUT";

	// ...
	// set the current selected multihead mode
	// if the page was close with the finish button
	// ---
	if (reload == PAGE_RELOAD) {
		updateModus();
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
	}
	mIntro -> importFile (update);
	slotIntro (mIndex);
	XTemplate::resetPage ();
	mStack -> raiseWidget (Intro);
}

//=====================================
// XMultihead internal page called...
//-------------------------------------
void XMultihead::pageCalled (int index) {
	// ...
	// this function is called if the next page of the dialog
	// is called. Note if the dialog has only one page this function
	// will not be called because there is no next page in this case
	// ---
	switch (index) {
	case MultiheadPage1: {
		break;
	}
	};
}

//=====================================
// XMultihead init page with data infos 
//-------------------------------------
void XMultihead::initPage (void) {
    // log (L_INFO,"XMultihead::initPage() called\n");
    // ...
    // this function is called after the page was created.
    // It must only be used to init the widgets contents
    // with the data available for selections and other stuff
    // like that
    // ---
}

//=====================================
// XMultihead virtual slots...
//-------------------------------------
bool XMultihead::slotRun (int index) {
	// ...
	// this function is called if the multihead page is activated.
	// use this function to init the dialog with the current
	// multihead setup 
	// ---
	XWrapPointer< QDict<char> > mText (mTextPtr);
	if (index == Modus) {
	if (! mAvailable) { 
		XBox mb (
			mText["dialoginfo"],
			mText["multihead_impossible"],
			XBox::Information,
			XBox::Ok,
			0,0,mFrame,
			globalFrameWidth
		);
		mb.setButtonText (
			XBox::Ok,mText["Ok"] 
		);
		mb.exec();
		mFrame -> enterEvent ( 0 );
		return (false);
	}
	}
	if (XTemplate::slotRun (index)) {
	// log(L_INFO,"XMultihead::slotRun() called: %d\n",index);
	// ...
	// get the mFiles pointer wrapper and read the data
	// record holding the server layout specifications
	// There is only one server layout section
	// ---
	QDict<XFile>* mFilePtr = mIntro->getFiles();
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);
	XWrapPointer<XData> serverLayout (
		mFiles["sys_LAYOUT"] -> getDevice(0)
	);

	// ...
	// setup currently used multihead modus
	// ---
	int modus = TRADITIONAL;
	if (serverLayout["Xinerama"]) {
	if (QString(serverLayout["Xinerama"]) == "on") {
		modus = XINERAMA;
	}
	}
	if (serverLayout["Clone"]) {
	if (QString(serverLayout["Clone"]) == "on") {
		modus = CLONE;
	}
	}
	switch (modus) {
	case CLONE:
		mClone -> setChecked (true);
	break;
	case XINERAMA:
		mXinerama -> setChecked (true);
	break;
	default:
		mTraditional -> setChecked (true);
	break;	
	}

	// ...
	// update status line and finish button
	// ---
	mStatus -> message (mText["RunXMultihead"]);
	mFrame  -> nextButton() -> setText (mText["finish"]);
	}
	return (TRUE);
}

//=====================================
// XMultihead set selected modus
//-------------------------------------
void XMultihead::updateModus (void) {
	// ...
	// this function is manually called via resetPage()
	// to include the currently selected multihead modus
	// to the data record
	// ---
	// get sysLayout data pointer within pointer
	// wrapper class
	// ---
	QDict<XFile>* mFilePtr = mIntro->getFiles();
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);
	XWrapPointer<XData> serverLayout (
		mFiles["sys_LAYOUT"] -> getDevice(0)
	);
	// ...
	// initially disable all modes, than overwrite
	// with current setting
	// ---
	serverLayout.setPair ("Xinerama","off");
	serverLayout.setPair ("Clone","off");
	if (mClone -> isChecked()) {
		serverLayout.setPair ("Clone","on");
		serverLayout.setPair ("Xinerama","on");
	}
	if (mXinerama -> isChecked()) {
		serverLayout.setPair ("Xinerama","on");
	}
}
