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
              : configure tablets including pens and erasers
              : and other tablet stuff
              :
              : --> signal action slots/callbacks
              :
STATUS        : Status: Up-to-date
**************/
#include "tablet.h"

//=====================================
// external globals
//-------------------------------------
extern int globalFrameWidth;

//=====================================
// XTablet reset and switch to Intro
//-------------------------------------
void XTablet::resetPage (int reload) {
	// ...
	// this function is called if the tablet dialog is finished or canceled
	// The changes are serialized and re-imported if the user want
	// to conclude the dialog. Otherwhise the original file is imported
	// and the changes will be lost.
	// ---
	QString update = "sys_INPUT";

	if (reload == PAGE_RELOAD) {
		addTablet();
	}
	if (reload == PAGE_RELOAD) {
		XLayout* layoutDialog;
		layoutDialog = (XLayout*) mIntro -> retrieve (Layout);
		layoutDialog -> updateServerLayout();
	}
	mStatus -> clear();
	if (reload == PAGE_RELOAD) {
		mFilePtr = mIntro->getFiles();
		XWrapFile < QDict<XFile> > mFiles (mFilePtr);
		if (mFiles [update] -> sysSerialize()) {
			mFiles [update] -> isModified ( mFrame );
		}
		update = "sys_LAYOUT";
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
// XTablet internal page called...
//-------------------------------------
void XTablet::pageCalled (int index) {
	// ...
	// this function is called if the next page of the dialog
	// is called. Note if the dialog has only one page this function
	// will not be called because there is no next page in this case
	// ---
	switch (index) {
	case TabletPage1: {
		break;
	}
	};
}

//=====================================
// XTablet virtual slots...
//-------------------------------------
bool XTablet::slotRun (int index) {
	if (XTemplate::slotRun (index)) {
	log(L_INFO,"XTablet::slotRun() called: %d\n",index);
	// ...
	// this function is called if the tablet page is activated.
	// use this function to init the dialog with the current
	// setup of the tablet
	// ---
	XWrapPointer< QDict<char> > mText (mTextPtr);
	mStatus -> message (mText["RunXTablet"]);
	mFrame  -> nextButton() -> setText (mText["finish"]);

	// ...
	// get the mFiles pointer wrapper from the Intro
	// object which has read all the data files
	// ---
	QDict<XFile>* mFilePtr = mIntro->getFiles();
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);

	// ...
	// find the first tablet listed and try to select
	// it in the vendor/name listbox
	// ---
	QDict<char> workingTablet;
    XData* data = NULL;
	gotTablet = false;
	gotPen    = false;
	gotEraser = false;
	int count = -1;
	for (int n=0; n<mFiles["sys_INPUT"]->getDeviceCount(); n++) {
		count += 2;
		data = mFiles["sys_INPUT"] -> getDevice(count);
		if (! data) {
			continue;
		}
		workingTablet = data -> getData();
		QString inputFashion (workingTablet["InputFashion"]);
		if (inputFashion != "Tablet") {
			continue;
		}
		gotTablet = true;
		IDTablet  = count;
		break;
	}
	if (gotTablet) {
		count = -1;
		for (int n=0; n<mFiles["sys_INPUT"]->getDeviceCount(); n++) {
			count += 2;
			data = mFiles["sys_INPUT"] -> getDevice(count);
			if (! data) {
			continue;
			}
			QDict<char> input = data -> getData();
			if (QString(input["Driver"]) == workingTablet["Driver"]) {
			QString inputFashion (input["InputFashion"]);
			if (inputFashion == "Pen") {
				IDPen  = count;
				gotPen = true;
			}
			if (inputFashion == "Eraser") {
				IDEraser  = count;
				gotEraser = true;
			}
			}
		}
	}
	if (gotTablet) {
		// ...
		// a) select vendor and name in the listboxes
		// b) check availability of pen and eraser
		// c) setup pen and eraser checkboxes
		// d) enable remove button
		// ---
		mPen    -> setChecked (false);
		mEraser -> setChecked (false);
		mVendor -> clearSelection();
		mName   -> clear();
		// a)
		for (int i=0;i < (int)mVendor->count();i++) {
		if (mVendor -> text(i) == QString(workingTablet["Vendor"])) {
			mVendor -> setSelected (i,true);
			slotVendor (mVendor -> item(i));
			break;
		}
		}
		// a)
		for (int i=0;i < (int)mName->count();i++) {
		if (mName -> text(i) == QString(workingTablet["Name"])) {
			mName -> setSelected (i,true);
			break;
		}
		}
		// b)
		slotName (0);
		// c)
		if (gotPen) {
			mPen -> setChecked (true);
		}
		if (gotEraser) {
			mEraser -> setChecked (true);
		}
		// d)
		mRemove -> setDisabled (false);
	}
	}
	return (TRUE);
}

//=====================================
// XTablet init page with data infos 
//-------------------------------------
void XTablet::initPage (void) {
	// log (L_INFO,"XTablet::initPage() called\n");
	// ...
	// this function is called after the page was created.
	// It must only be used to init the widgets contents
	// with the data available for selections and other stuff
	// like that
	// ---
	QDict<XFile>* mFilePtr = mIntro->getFiles();
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);

	// insert touchscreen vendors...
	// -----------------------------
	QList<char> vendors = mFiles["cdb_TABLETS"]->cdbGetVendorList();
	QListIterator<char> it (vendors);
	for (; it.current(); ++it) {
		mVendor -> insertItem (it.current());
	}
}

//=====================================
// XTablet slotVendor...
//-------------------------------------
void XTablet::slotVendor (QListBoxItem* item) {
	// log (L_INFO,"XTablet::slotVendor() called\n");
	// ...
	// this function is called if you select a tablet vendor
	// from the tablet vendor listbox in the setup toplevel window
	// ---
	QDict<XFile>* mFilePtr = mIntro->getFiles();
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);
	if (! mVendor->isSelected (item)) {
		return;
	}
	mName -> clear();
	QString selectedVendor = mVendor -> currentText();
	QList<char> names = mFiles["cdb_TABLETS"]->cdbGetNameList (selectedVendor);
	QListIterator<char> it (names);
	for (; it.current(); ++it) {
		mName -> insertItem (it.current());
	}
	mName -> sort();
}   
    
//=====================================
// XTablet slotName...
//-------------------------------------
void XTablet::slotName (QListBoxItem*) {
	// log (L_INFO,"XTablet::slotName() called\n");
	// ...
	// this function is called if you select a tablet name
	// from the tablet name listbox in the setup toplevel window
	// ---
	// get the mFiles pointer wrapper
	// ---
	QDict<XFile>* mFilePtr = mIntro->getFiles();
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);
    
	// ...
	// get the CDB pointer for the current vendor
	// name selection of the tablet listboxes
	// and look for the stylus and eraser links
	// ---
	XDb* pCDB = mFiles["cdb_TABLETS"]->cdbGetSpec(
		mVendor->currentText(),mName->currentText()
	);
	if (pCDB) {
		mPen    -> setDisabled ( true );
		mEraser -> setDisabled ( true );
		mPen    -> setChecked  ( false );
		mEraser -> setChecked  ( false );
		QDict<char>* spec = pCDB -> getHash();
		XWrapPointer< QDict<char> > selectedTablet (spec);
		if (selectedTablet["StylusLink"]) {
			mPen -> setDisabled ( false );
		}
		if (selectedTablet["EraserLink"]) {
			mEraser -> setDisabled ( false );
		}
	}
}

//=====================================
// XTablet remove tablet if exist...
//-------------------------------------
void XTablet::slotRemove (void) {
	// log (L_INFO,"XTablet::slotRemove() called\n");
	// ...
	// if a tablet / pen / eraser exist we will remove
	// the data from the system record now
	// ---
	QDict<XFile>* mFilePtr = mIntro->getFiles();
    XWrapFile < QDict<XFile> > mFiles (mFilePtr);
	// ...
	// ask me if I really want to do that
	// ---
	if (! deleteOnly) {
		XWrapPointer< QDict<char> > mText (mTextPtr);
		XBox mb (
			mText["rmtablet"],
			mText["rmtabletquestion"],
			XBox::Warning,
			XBox::Yes,
			XBox::No,
			0,mFrame,
			globalFrameWidth
		);
		mb.setButtonText (
			XBox::Yes,mText["Yes"]
		);
		mb.setButtonText (
			XBox::No, mText["No"]
		);
		switch (mb.exec()) {
		case XBox::No: {
			return;
		}
		}
		mFrame -> enterEvent ( 0 );
	}
	if (gotTablet) {
		mFiles["sys_INPUT"] -> deleteDevice (IDTablet);
	}
	if (gotPen) {
		mFiles["sys_INPUT"] -> deleteDevice (IDPen);
	}
	if (gotEraser) {
		mFiles["sys_INPUT"] -> deleteDevice (IDEraser);
	}
	// ...
	// finish the dialog now and reset the page if
	// not only want to delete the tablet
	// ---
	if (! deleteOnly) {
		mRemove -> setDisabled ( true );
		mVendor -> clearSelection();
        mName   -> clear();
		resetPage ( PAGE_RELOAD );
	}
}

//=====================================
// XTablet add tablet...
//-------------------------------------
void XTablet::addTablet (void) {
	// log (L_INFO,"XTablet::addTablet() called\n");
    // ...
	// this function will add the needed InputDevice sections
	// to the isax record. First task is to find the index number 
	// used for the tablet and the eventually needed tools 
	// (pen,eraser...)
	// ---
	// get the mFiles pointer wrapper
	// ---
	QDict<XFile>* mFilePtr = mIntro->getFiles();
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);

	// ...
	// get the CDB pointer for the current vendor
	// name selection of the tablet listboxes
	// ---
	XDb* pCDB = mFiles["cdb_TABLETS"]->cdbGetSpec(
		mVendor->currentText(),mName->currentText()
	);
	if (pCDB) {
		// ...
		// delete an eventually existing tablet
		// ---
		deleteOnly = true;
		slotRemove();
		deleteOnly = false;
		// ...
		// find index number for the new device and
		// add a new one, set new tablet ID and tell
		// gotTablet that we have a new tablet
		// ---
		int count = 1;
		while (mFiles["sys_INPUT"]->getDevice (count)) {
			count += 2;
		}
		XWrapPointer<XData> workingTablet (
			mFiles["sys_INPUT"] -> addDevice (count)
		);
		IDTablet  = count;
		gotTablet = true;
		// ...
		// set base elements for the new tablet
		// ---
		QString* ident  = new QString;
		QString* name   = new QString (mName->currentText());
		QString* vendor = new QString (mVendor->currentText());
		ident -> sprintf ("Mouse[%d]",count);
		workingTablet.setPair ("InputFashion","Tablet");
		workingTablet.setPair ("Identifier",ident->ascii());
		workingTablet.setPair ("Name"  , name->ascii());
		workingTablet.setPair ("Vendor", vendor->ascii());
		// ...
		// get CDB information and fill the
		// new workingTablet record
		// ---
		QDict<char>* spec = pCDB -> getHash();
		QDictIterator<char> it (*spec);
		for (; it.current(); ++it) {
		QString* key = new QString (it.currentKey());
		QString* val = new QString;
		val->sprintf("%s",it.current());
		if (val->isEmpty()) {
			continue;
		}
		if ((*key == "StylusLink") || (*key == "EraserLink")) {
			continue;
		}
		workingTablet.setPair (
			key->ascii(),val->ascii()
		);
		}
		// ...
		// add pen and eraser if available and selected
		// use the searchTool function to get the record
		// ...
		XWrapPointer< QDict<char> > selectedTablet (spec);
		if (mPen->isChecked()) {
		QDict<char>* pen = searchTool (
			mVendor->currentText(),selectedTablet["StylusLink"]
		);
		if (pen) {
			count += 2;
			XData* newPen = mFiles["sys_INPUT"] -> addDevice (count);
			QString* ident  = new QString;
			QString* name   = new QString (selectedTablet["StylusLink"]);
			QString* vendor = new QString (mVendor->currentText());
			ident  -> sprintf ("Mouse[%d]",count);
			newPen -> setPair ("InputFashion","Pen");
			newPen -> setPair ("Identifier",ident->ascii());
			newPen -> setPair ("Name"  , name->ascii());
			newPen -> setPair ("Vendor", vendor->ascii());
			QDictIterator<char> it (*pen);
			for (; it.current(); ++it) {
			QString* key = new QString (it.currentKey());
			QString* val = new QString;
			val->sprintf("%s",it.current());
			if (val->isEmpty()) {
				continue;
			}
			newPen -> setPair (
				key->ascii(),val->ascii()
			);
			}
			IDPen  = count;
			gotPen = true;
		}
		}
		if (mEraser->isChecked()) {
		QDict<char>* eraser = searchTool (
			mVendor->currentText(),selectedTablet["EraserLink"]
		);
		if (eraser) {
			count += 2;
			XData* newEraser = mFiles["sys_INPUT"] -> addDevice (count);
			QString* ident  = new QString;
			QString* name   = new QString (selectedTablet["EraserLink"]);
			QString* vendor = new QString (mVendor->currentText());
			ident  -> sprintf ("Mouse[%d]",count);
			newEraser -> setPair ("InputFashion","Eraser");
			newEraser -> setPair ("Identifier",ident->ascii());
			newEraser -> setPair ("Name"  , name->ascii());
			newEraser -> setPair ("Vendor", vendor->ascii());
			QDictIterator<char> it (*eraser);
			for (; it.current(); ++it) {
			QString* key = new QString (it.currentKey());
			QString* val = new QString;
			val->sprintf("%s",it.current());
			if (val->isEmpty()) {
				continue;
			}
			newEraser -> setPair (
				key->ascii(),val->ascii()
			);  
			}
			IDEraser  = count;
			gotEraser = true;
		}
		}
	}
}

//=====================================
// XTablet ...
//-------------------------------------
QDict<char>* XTablet::searchTool (
	const QString & vendorName, const QString & linkName
) {
	// log (L_INFO,"XTablet::searchTool() called\n");
	// ...
	// this function is used to get the specification hash for
	// the requested tool link. The parameter linkName is part
	// of the selectedTablet record which stores the tool link
	// either as StylusLink or EraserLink key
	// ---
	// get the mFiles pointer wrapper
    // ---
    QDict<XFile>* mFilePtr = mIntro->getFiles();
    XWrapFile < QDict<XFile> > mFiles (mFilePtr);

	QDict<char>* spec = NULL;
	XDb* pCDB = mFiles["cdb_PENS"]->cdbGetSpec(
		vendorName,linkName
	);
	if (pCDB) {
		spec = pCDB -> getHash();
	}
	return (spec);
}
