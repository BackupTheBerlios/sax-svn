/**************
FILE          : virtualhandler.c
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the GPL license
              :
DESCRIPTION   : xapi (X11-Application-Interface)
              : setup virtual resolution for every screen
              :
              : --> signal action slots/callbacks
              :
STATUS        : Status: Up-to-date
**************/
#include "virtual.h"

//=====================================
// XVirtual reset and switch to Intro
//-------------------------------------
void XVirtual::resetPage (int reload) {
	// ...
	// this function is called if the XVirtual dialog is finished or 
	// canceled The changes are serialized and re-imported if the user want
	// to conclude the dialog. Otherwhise the original file is imported
	// and the changes will be lost.
	// ---
	QString update = "sys_DESKTOP";

	if (reload == PAGE_RELOAD) {
	XData* sysData;
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);
	for (int n=0; n<mFiles["sys_DESKTOP"]->getDeviceCount(); n++) {
		sysData = mFiles["sys_DESKTOP"] -> getDevice(n);
		if (! sysData) {
			continue;
		}
		int depth = 8;
		while (1) {
			QString reskey;
			reskey.sprintf("Virtual:%d",depth);
			if (! mArea.at(n)->hasChanged()) {
			if (mArea.at(n)->virtualEqualsReal()) {
				sysData -> setPair ( reskey,"" );
			}
			} else {
				QPoint p = mArea.at(n)->getVirtualResolution();
				QString* vres = new QString();
				vres->sprintf("%d %d",p.x(),p.y());
				sysData -> setPair ( reskey,vres->ascii() );
			}
			depth += 8;
			if (depth == 40) {
				depth = 15;
			}
			if (depth == 23) {
				break;
			}
		}
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
	}
	mIntro -> importFile (update);
	slotIntro (mIndex);
	XTemplate::resetPage ();
	mStack -> raiseWidget (Intro);
}

//=====================================
// XVirtual internal page called...
//-------------------------------------
void XVirtual::pageCalled (int index) {
	// ...
	// this function is called if the next page of the dialog
	// is called. Note if the dialog has only one page this function
	// will not be called because there is no next page in this case
	// ---
	switch (index) {
	case VirtualPage1: {
		break;
	}
	};
}

//=====================================
// XVirtual init page with data infos 
//-------------------------------------
void XVirtual::initPage (void) {
    // log (L_INFO,"XVirtual::initPage() called\n");
    // ...
    // this function is called after the page was created.
    // It must only be used to init the widgets contents
    // with the data available for selections and other stuff
    // like that
    // ---
	XData* sysData;
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);
	for (int n=0; n<mFiles["sys_DESKTOP"]->getDeviceCount(); n++) {
		sysData = mFiles["sys_DESKTOP"] -> getDevice(n);
		if (! sysData) {
			continue;
		}
		QDict<char> desktopInfo = sysData -> getData();
		QString defaultColor (desktopInfo["ColorDepth"]);
		QString reskey; reskey.sprintf("Virtual:%s",defaultColor.ascii());
		if (desktopInfo[reskey]) {
			XStringList res (desktopInfo[reskey]);
			res.setSeperator (" ");
			QString vres;
			vres.sprintf("%sx%s",
				res.getList().at(0),res.getList().at(1)
			);
			mArea.at(n) -> setVirtual (vres);
		}
	}
}

//=====================================
// XVirtual enable or disable 3D feature
//-------------------------------------
void XVirtual::slotDesktop (int index) {
	// log (L_INFO,"XVirtual::slot3D() called\n");
	// ...
	// Slot called if another desktop is selected to setup
	// the virtual resolution for the currently selected
	// one
	// ---
	mVirtualStack -> raiseWidget (index);
}

//=====================================
// XVirtual virtual slots...
//-------------------------------------
bool XVirtual::slotRun (int index) {
	if (XTemplate::slotRun (index)) {
	// log(L_INFO,"XVirtual::slotRun() called: %d\n",index);
	// ...
	// this function is called if the XVirtual page is activated.
	// use this function to init the dialog with the current
	// XVirtual setup 
	// ---
	XWrapPointer< QDict<char> > mText (mTextPtr);
	mStatus -> message (mText["RunXVirtual"]);
	mFrame  -> nextButton() -> setText (mText["finish"]);

	// ...
	// get the mFiles pointer wrapper from the Intro
	// object which has read all the data files
	// ---
	QDict<XFile>* mFilePtr = mIntro->getFiles();
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);

	// ...
	// Clean up widget stack and delete its
	// objects
	// ---
	int count = 0;
	while (1) {
		QWidget* w = NULL;
		w = mVirtualStack -> widget (count);
		if (! w) {
			break;
		}
		mVirtualStack -> removeWidget (w);
		delete (w);
		count++;
	}
	mDesktop -> clear();
	// ...
	// Create new widget stack
	// ---
	XData* sysData;
	QDict<char> desktopInfo;
	int desktops;
	desktops = mFiles["sys_DESKTOP"]->getDeviceCount();
	for (int n=0; n<=desktops; n++) {
		sysData = mFiles["sys_DESKTOP"] -> getDevice(n);
		if (! sysData) {
			continue;
		}
		desktopInfo = sysData -> getData();
		QString defaultColor (desktopInfo["ColorDepth"]);
        QString reskey; reskey.sprintf("Modes:%s",defaultColor.ascii());
		XStringList res (desktopInfo[reskey]);
		res.setSeperator (",");
		QString primary = res.getList().at(0);
		XStringList geo (primary);
		geo.setSeperator ("x");
		QString initX = geo.getList().at(0);
		QString initY = geo.getList().at(1);
		//======================================== 
		// Fill combo box with desktop selections
		//----------------------------------------
		QString topic;
		QString translation = mText["virtualselect"];
		translation.replace (
			QRegExp("\%1"),primary
		);
		QTextOStream (&topic) 
			<< mText["desktop"] << " ( " << n << " ) : " << translation;
		mDesktop -> insertItem (topic);
		//========================================
		// create VirtualArea widgets within 
		// widget stack object
		//----------------------------------------
		QFrame* vt = new QFrame ( mDialog );
		vt -> setFixedWidth  ( mVirtualStack->width() );
		vt -> setFixedHeight ( mVirtualStack->height() );
		XVirtualArea* area = new XVirtualArea ( 
			initX.toInt() , initY.toInt(),
			vt
		);
		mArea.insert (n,area);
		mVirtualStack->addWidget ( vt,n );
	}
	// ...
	// raise first widget
	// ---
	mVirtualStack -> raiseWidget (0);
	mDesktop -> setCurrentItem (0);
	initPage ();
	}
	return (TRUE);
}
