/**************
FILE          : template.c
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the GPL license
              :
DESCRIPTION   : xapi (X11-Application-Interface)
              : XTemplate virtual class used to create
              : connections for XFrame
              :
STATUS        : Status: Up-to-date
**************/
#include <qapplication.h>
#include <qwidget.h>

#include "template.h"
#include "xapi.h"

//=====================================
// external globals
//-------------------------------------
extern int globalFrameWidth;
extern XIntro* introPointer;

//=====================================
// XTemplate Constructor...
//-------------------------------------
XTemplate::XTemplate (XFrame* xf,XIntro* intro) {
	mVisible  = FALSE;
	mLastPage = 0;
	mPage     = 0;
	mIndex    = 0;
	mIntro    = intro;
	getConnected (xf);
}

//=====================================
// XTemplate Constructor...
//-------------------------------------
XTemplate::XTemplate (void) {
	mFrame    = NULL;
	mIntro    = NULL;
	mStack    = NULL;
	mVisible  = FALSE;
	mLastPage = 0;
	mPage     = 0;
	mIndex    = 0;
}

//=====================================
// create connections to base view...
//-------------------------------------
void XTemplate::getConnected (XFrame* xf) {
	mFrame = xf;
	mStack = mFrame -> getStack();
	mInit  = mFrame -> getInit();
	mTextPtr = mFrame -> getTextPointer();
	QObject::connect(
		mFrame , SIGNAL (sigNext  ()),
		this   , SLOT   (slotNext ())
	);
    QObject::connect(
		mFrame , SIGNAL (sigBack  ()),
		this   , SLOT   (slotBack ())
	);
    QObject::connect(
		mFrame , SIGNAL (sigRun   (int)),
		this   , SLOT   (slotRun  (int))
	);
	QObject::connect(
		mFrame , SIGNAL (sigCheck  (int)),
		this   , SLOT   (slotCheck (int))
	);
	QObject::connect(
		mFrame , SIGNAL (sigIntro (int)),
		this   , SLOT   (slotIntro(int))
	);
	QObject::connect(
		this   , SIGNAL (sigExit  ()),
		mFrame , SLOT   (slotExit ())
	);
	QObject::connect(
		mFrame , SIGNAL (sigAbort (int)),
		this   , SLOT   (slotAbort(int))
	);
}

//=====================================
// standard handling for virtual f()...
//-------------------------------------
// 
//=====================================
// XTemplate resetPage...
//-------------------------------------
void XTemplate::resetPage (int reload) {
	mPage    = mIndex;
	mVisible = FALSE;
	XWrapPointer< QDict<char> > mText (mTextPtr);
	if (reload == PAGE_RELOAD) {
		mFrame  -> nextButton() -> setText (mText["Next"]);
		mFrame  -> nextButton() -> setDisabled (TRUE);
		mFrame  -> backButton() -> setDisabled (TRUE);
		mFrame  -> buttonBar2() -> hide();
		mFrame  -> buttonBar1() -> show();
	}
	mStatus -> message (mText["dohint"]);
	mFrame  -> showListView();
	mFrame  -> finishButton() -> setFocus();
}

//=====================================
// XTemplate savePage...
//-------------------------------------
int XTemplate::savePage (void) {
	return (PAGE_RELOAD);
}

//=====================================
// XTemplate pageCalled...
//-------------------------------------
void XTemplate::pageCalled (int index) {
	log(L_INFO,"XTemplate::pageCalled -> %d...\n",index);
}

//=====================================
// XTemplate slotNext...
//-------------------------------------
void XTemplate::slotNext (void) {
	XWrapPointer< QDict<char> > mText (mTextPtr);
	if (mVisible) {
	// if finish is pressed; finish this page...
	// ------------------------------------------
	if (mFrame -> nextButton() -> text() == mText["finish"]) {
	int reload = savePage();
	if (reload != PAGE_CANCEL) {
		resetPage ( reload );
	}
	return;
    }
	// if first page is visible set mPage index
	// -----------------------------------------
	if (mPage == mIndex) {
		mPage = mIndex * 10;
	} else {
		mPage++;
	}
	// if this is the last page set finish text...
	// --------------------------------------------
	if (mPage == mLastPage) {
		mFrame -> nextButton() -> setText (mText["finish"]);
	}
	mFrame -> backButton() -> setDisabled (FALSE);
	mStack -> raiseWidget (mPage);
	pageCalled (mPage);
	}
}

//=====================================
// XTemplate slotBack...
//-------------------------------------
void XTemplate::slotBack (void) {
	if (mVisible) {
	// if we are on the finish page, set next text...
	// -----------------------------------------------
	XWrapPointer< QDict<char> > mText (mTextPtr);
	if (mFrame -> nextButton() -> text() == mText["finish"]) {
		mFrame -> nextButton() -> setText (mText["Next"]);
	}
	// if first page is reached set mPage index
	// ------------------------------------------
	if (mPage == (mIndex * 10)) {
		mFrame -> backButton() -> setDisabled (TRUE);
		mPage = mIndex;
	} else {
		mPage--;
	}
	mStack -> raiseWidget (mPage);
	pageCalled (mPage);
	}
}

//=====================================
// XTemplate slotRun...
//-------------------------------------
bool XTemplate::slotRun (int index) {
	if (mVisible) {
		mVisible = FALSE;
		if (index == mIndex) {
			resetPage (PAGE_NOLOAD);
		}
	}
	if (index == mIndex) {
		mVisible = TRUE;
		mFrame  -> buttonBar1()   -> hide();
		mFrame  -> buttonBar2()   -> show();
		mFrame  -> nextButton()   -> setDefault  (TRUE);
		mFrame  -> nextButton()   -> setDisabled (FALSE);
		mFrame  -> backButton()   -> setDisabled (TRUE);
		mFrame  -> hideListView();
		mStack  -> raiseWidget(mIndex);
		return (TRUE);
	}
	return (FALSE);
}

//=====================================
// XTemplate slotIntro...
//-------------------------------------
void XTemplate::slotIntro (int index) {
	if (index == mIndex) {
	printf("XTemplate::slotIntro() with index: %d\n",index);
	}
}

//=====================================
// XTemplate slot check detected...
//-------------------------------------
void XTemplate::slotCheck (int index) {
	if (index == mIndex) {
		introPointer -> checkDetected();
	}
}

//=====================================
// XTemplate slotExit...
//-------------------------------------
void XTemplate::slotExit (void) {
	sigExit();
}

//=====================================
// XTemplate slotAbort...
//-------------------------------------
void XTemplate::slotAbort (int index) {
	if (index == mIndex) {
		mVisible = FALSE;
		if (index == mIndex) {
			resetPage (PAGE_NOLOAD);
			mFrame -> finishButton() -> setFocus();
		}
	}
}
