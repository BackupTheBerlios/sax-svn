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
		//    save pen/eraser data to mToolData[...] hashes
		// d) setup main tablet options
		// e) setup port
		// f) setup mode
		// g) enable remove button
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
			mPenConfigure -> setDisabled (false);
			XWrapPointer<XData> workingPen (
				mFiles["sys_INPUT"] -> getDevice (IDPen)
			);
			if (workingPen["Mode"]) {
				QString* data = new QString (workingPen["Mode"]);
				mToolDataPen.insert ("Mode",*data);
			}
			if (workingPen["Option"]) {
				QString* data = new QString (workingPen["Option"]);
				mToolDataPen.insert ("Option",*data);
			}
			if (workingPen["RawOption"]) {
				XStringList options;
				options.setText (workingPen ["RawOption"]);
				options.setSeperator (",");
				QList<char> optraw = options.getList();
				QListIterator<char> ir (optraw);
				QStringList allopt = QStringList::split(",",
					workingPen["Option"]
				);
				if (! optraw.isEmpty()) {
					for (; ir.current(); ++ir) {
						options.setText (ir.current());
						options.setSeperator (" ");
						QList<char> keyval = options.getList();
						QString* key = new QString (keyval.at(0));
						QString* val = new QString (keyval.at(1));
						*key = key->stripWhiteSpace();
						*val = val->stripWhiteSpace();
						*key = key->mid (1,key->length() - 2);
						*val = val->mid (1,val->length() - 2);
						mToolDataPen.insert (*key,*val);
						allopt.append (*key);
					}
					QString* data = new QString (allopt.join(","));
					mToolDataPen.replace ("Option",*data);
				}
			}
		}
		if (gotEraser) {
			mEraser -> setChecked (true);
			mEraserConfigure -> setDisabled (false);
			XWrapPointer<XData> workingEraser (
				mFiles["sys_INPUT"] -> getDevice (IDEraser)
			);
			if (workingEraser["Mode"]) {
				QString* data = new QString (workingEraser["Mode"]);
				mToolDataEraser.insert ("Mode",*data);
			}
			if (workingEraser["Option"]) {
				QString* data = new QString (workingEraser["Option"]);
				mToolDataEraser.insert ("Option",*data);
			}
			if (workingEraser["RawOption"]) {
				XStringList options;
				options.setText (workingEraser ["RawOption"]);
				options.setSeperator (",");
				QList<char> optraw = options.getList();
				QListIterator<char> ir (optraw);
				QStringList allopt = QStringList::split(",",
					workingEraser["Option"]
				);
				if (! optraw.isEmpty()) {
					for (; ir.current(); ++ir) {
						options.setText (ir.current());
						options.setSeperator (" ");
						QList<char> keyval = options.getList();
						QString* key = new QString (keyval.at(0));
						QString* val = new QString (keyval.at(1));
						*key = key->stripWhiteSpace();
						*val = val->stripWhiteSpace();
						*key = key->mid (1,key->length() - 2);
						*val = val->mid (1,val->length() - 2);
						mToolDataEraser.insert (*key,*val);
						allopt.append (*key); 
					} 
					QString* data = new QString (allopt.join(","));
					mToolDataEraser.replace ("Option",*data);
				}
			}
		}
		// d)
		if ((workingTablet ["Option"]) || (workingTablet ["RawOption"])) {
			mTabletOptions -> clearSelection();
		}
		// d)
		if (workingTablet ["Option"]) {
			XStringList options;
			options.setText (workingTablet ["Option"]);
			options.setSeperator (",");
			QList<char> optl = options.getList();
			QListIterator<char> it (optl);
			if (! optl.isEmpty()) {
			for (; it.current(); ++it) {
				for (unsigned int id=0;id<mTabletOptions->count();id++) {
					QListBoxItem* item = mTabletOptions->item (id);
					if (QString(it.current()) == item->text()) {
						mTabletOptions -> setSelected ( id,true );
					}
				}
			}
			}
		}
		// d)
		if (workingTablet ["RawOption"]) {
			XStringList options;
			options.setText (workingTablet ["RawOption"]);
			options.setSeperator (",");
			QList<char> optraw = options.getList();
			QListIterator<char> ir (optraw);
			if (! optraw.isEmpty()) {
			for (; ir.current(); ++ir) {
				options.setText (ir.current());
				options.setSeperator (" ");
				QList<char> keyval = options.getList();
				QString* key = new QString (keyval.at(0));
				QString* val = new QString (keyval.at(1));
				*key = key->stripWhiteSpace();
				*val = val->stripWhiteSpace();
				*key = key->mid (1,key->length() - 2);
				*val = val->mid (1,val->length() - 2);
				// save raw options...
				mTabletDataRawOptions.insert (*key,*val);
				// select raw options...
				for (unsigned int id=0;id<mTabletOptions->count();id++) {
					QListBoxItem* item = mTabletOptions->item (id);
					if (*key == item->text()) {
						mTabletOptions -> setSelected ( id,true );
					}
				}
			}
			}
		}

		// e)
		if (workingTablet["Device"]) {
			QString device = workingTablet ["Device"];
			QRegExp identifier ("/dev/input/event");
			if (identifier.search (device) >= 0) {
				mPort -> setCurrentItem ( 0 );
				mPort -> setCurrentText (device);
			}
			if (device == "/dev/ttyS0") {
				mPort -> setCurrentItem ( 1 );
			}
			if (device == "/dev/ttyS1") {
				mPort -> setCurrentItem ( 2 );
			}
		}

		// f)
		if (workingTablet ["Mode"]) {
		if (QString(workingTablet ["Mode"])	== "Absolute") {
			mModeAbsolute -> setChecked ( true );
		} else {
			mModeRelative -> setChecked ( true );
		}
		}

		// g)
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
		QDict<char>* spec = pCDB -> getHash();
		XWrapPointer< QDict<char> > selectedTablet (spec);
		// ...
		// insert options for this tablet...
		// ---
		mTabletOptions -> clear();
		mToolTabletOptions -> clear();
		QStringList optlist = getOptions ( selectedTablet["Driver"] );
		mTabletOptions -> insertStringList (
			optlist
		);
		mToolTabletOptions -> clear();
		mToolTabletOptions -> insertStringList (
			optlist
		);
		// ...
		// setup pen/eraser state for this tablet
		// ---
		mPen    -> setDisabled ( true );
		mEraser -> setDisabled ( true );
		mPen    -> setChecked  ( false );
		mEraser -> setChecked  ( false );
		mPenConfigure    -> setDisabled ( true );
		mEraserConfigure -> setDisabled ( true );

		if (selectedTablet["StylusLink"]) {
			mPen -> setDisabled ( false );
		}
		if (selectedTablet["EraserLink"]) {
			mEraser -> setDisabled ( false );
		}
		// ...
		// clear tool data hashes
		// ---
		mTabletDataRawOptions.clear();
		mToolDataEraser.clear();
		mToolDataPen.clear();

		// ...
		// enable widgets
		// ---
		mPort -> setDisabled          ( false );
		mTabletOptions -> setDisabled ( false );
		mModeRelative -> setDisabled  ( false );
		mModeAbsolute -> setDisabled  ( false );
		// ...
		// select options [bool]
		// ---
		XStringList options;
		options.setText (selectedTablet ["Option"]);
		options.setSeperator (",");
		QList<char> optl = options.getList();
		QListIterator<char> it (optl);
		if (! optl.isEmpty()) {
		for (; it.current(); ++it) {
			int id = 0;
			QStringList::Iterator in;
			for (in = optlist.begin(); in != optlist.end(); ++in) {
				if (QString(it.current()) == QString (*in)) {
					mTabletOptions -> setSelected ( id,true );
				}
				id++;
			}
		}
		}
		// ...
		// select options [raw]
		// ---
		options.setText (selectedTablet ["RawOption"]);
		options.setSeperator (",");
		QList<char> optraw = options.getList();
		QListIterator<char> ir (optraw);
		if (! optraw.isEmpty()) {
		for (; ir.current(); ++ir) {
			options.setText (ir.current());
			options.setSeperator (" ");
			QList<char> keyval = options.getList();
			QString* key = new QString (keyval.at(0));
			QString* val = new QString (keyval.at(1));
			*key = key->stripWhiteSpace();
			*val = val->stripWhiteSpace();
			*key = key->mid (1,key->length() - 2);
			*val = val->mid (1,val->length() - 2);
			// save raw options...
			mTabletDataRawOptions.insert (*key,*val);
			// select raw options...
			int id = 0;
			QStringList::Iterator in;
			for (in = optlist.begin(); in != optlist.end(); ++in) {
				if (*key == QString (*in)) {
					mTabletOptions -> setSelected ( id,true );
				}
				id++;
			}
		}
		}
		// ...
		// select port (Device) node
		// ---
		QString device = selectedTablet ["Device"];
		QRegExp identifier ("/dev/input/event");
		if (identifier.search (device) >= 0) {
			mPort -> setCurrentItem ( 0 );
		}
		if (device == "/dev/ttyS0") {
			mPort -> setCurrentItem ( 1 );
		}
		if (device == "/dev/ttyS1") {
			mPort -> setCurrentItem ( 2 );
		}
		// ...
		// select mode
		// ---
		QString mode = selectedTablet ["Mode"];
		if (mode == "Absolute") {
			mModeAbsolute -> setChecked ( true );
		}
		if (mode == "Relative") {
			mModeRelative -> setChecked ( true );
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
		QDict<char>* pen = NULL;
		XData* newPen = NULL;
		XWrapPointer< QDict<char> > selectedTablet (spec);
		if (mPen->isChecked()) {
		pen = searchTool (
			mVendor->currentText(),selectedTablet["StylusLink"]
		);
		if (pen) {
			count += 2;
			newPen = mFiles["sys_INPUT"] -> addDevice (count);
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
		QDict<char>* eraser = NULL;
		XData* newEraser = NULL;
		if (mEraser->isChecked()) {
		eraser = searchTool (
			mVendor->currentText(),selectedTablet["EraserLink"]
		);
		if (eraser) {
			count += 2;
			newEraser = mFiles["sys_INPUT"] -> addDevice (count);
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
		// ...
		// save the pen user configuration
		// ---
		if (pen) {
			// options...
			if (mToolDataPen["Option"]) {
				QStringList options;
				QStringList rawoptions;
				QStringList allopt = QStringList::split(",",
					mToolDataPen["Option"]
				);
				QStringList::Iterator in;
				for (in = allopt.begin(); in != allopt.end(); ++in) {
				if (mToolDataPen[*in]) {
					QString line;
					line.sprintf ("\"%s\" \"%s\"",
						QString(*in).ascii(),mToolDataPen[*in]
					);
					rawoptions.append (line);
				} else {
					options.append (*in);
				}
				}
				if (! options.isEmpty()) {
					QString* opts = new QString (options.join(","));
					newPen -> setPair ("Option",*opts);
				} else {
					newPen -> setPair ("Option","");
				}
				if (! rawoptions.isEmpty()) {
					QString* opts = new QString (rawoptions.join(","));
					newPen -> setPair ("RawOption",*opts);
				} else {
					newPen -> setPair ("RawOption","");
				}
			}
			// mode...
			if (mToolDataPen["Mode"]) {
				newPen -> setPair ("Mode",mToolDataPen["Mode"]);
			}
		}
		// ...
		// save the eraser user configuration
		// ---
		if (eraser) {
			// options...
			if (mToolDataEraser["Option"]) {
				QStringList options;
				QStringList rawoptions;
				QStringList allopt = QStringList::split(",",
					mToolDataEraser["Option"]
				);
				QStringList::Iterator in;
				for (in = allopt.begin(); in != allopt.end(); ++in) {
				if (mToolDataEraser[*in]) { 
					QString line;
					line.sprintf ("\"%s\" \"%s\"",
						QString(*in).ascii(),mToolDataEraser[*in]
					);
					rawoptions.append (line);
				} else { 
					options.append (*in); 
				}
				} 
				if (! options.isEmpty()) { 
					QString* opts = new QString (options.join(",")); 
					newEraser -> setPair ("Option",*opts); 
				} else {
					newEraser -> setPair ("Option","");
				}
				if (! rawoptions.isEmpty()) {
					QString* opts = new QString (rawoptions.join(","));
					newEraser -> setPair ("RawOption",*opts);
				} else {
					newEraser -> setPair ("RawOption","");
				}
			}
			// mode...
			if (mToolDataEraser["Mode"]) { 
				newEraser -> setPair ("Mode",mToolDataEraser["Mode"]);
			}
		}
		// ...
		// save the main tablet user configuration
		// this includes the port the options and
		// the tablet mode
		// ---
		// options...
		QStringList options;
		QStringList rawoptions;
		for (unsigned int i=0;i<mTabletOptions->count();i++) {
			QListBoxItem* item = mTabletOptions->item (i);
			if (item -> isSelected()) {
			if (mTabletDataRawOptions[item->text()]) {
				QString line;
				line.sprintf ("\"%s\" \"%s\"",
					item->text().ascii(),mTabletDataRawOptions[item->text()]
				);
				rawoptions.append (line);
			} else {
				options.append (item->text());
			}
			}
		}
		if (! options.isEmpty()) {
			QString* opts = new QString (options.join(","));
			workingTablet.setPair ("Option",*opts);
		} else {
			workingTablet.setPair ("Option","");
		}
		if (! rawoptions.isEmpty()) {
			QString* opts = new QString (rawoptions.join(","));
			workingTablet.setPair ("RawOption",*opts);
		} else {
			workingTablet.setPair ("RawOption","");
		}
		// port...
		QString* tabletport = new QString (
			qx (GETTABLETPORT,STDOUT)
		);
		switch (mPort->currentItem()) {
			case 0:
				workingTablet.setPair ("Device",*tabletport);
				if (pen)    { newPen -> setPair ("Device",*tabletport);    }
				if (eraser) { newEraser -> setPair ("Device",*tabletport); }
			break;
			case 1:
				workingTablet.setPair ("Device","/dev/ttyS0");
				if (pen)    { newPen -> setPair ("Device","/dev/ttyS0");    }
				if (eraser) { newEraser -> setPair ("Device","/dev/ttyS0"); }
			break;
			case 2:
				workingTablet.setPair ("Device","/dev/ttyS1");
				if (pen)    { newPen -> setPair ("Device","/dev/ttyS1");    }
				if (eraser) { newEraser -> setPair ("Device","/dev/ttyS1"); }
			break;
		}
		// has somebody entered a device path manually...
		QString* portText = new QString (mPort->currentText());
		if (*portText[0] == '/') {
			workingTablet.setPair ("Device",*portText);
		}
		// mode...
		if (mModeRelative -> isOn()) {
			workingTablet.setPair ("Mode","Relative");
		} else {
			workingTablet.setPair ("Mode","Absolute");
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

//=====================================
// XTablet slotTopOk...
//-------------------------------------
void XTablet::slotTopOk (void) {
	// log (L_INFO,"XTablet::slotTopOk() called\n");
	// ...
	// this function is called if the configuration dialog
	// for either the pen or the eraser is finished with
	// the [OK] button
	// ---
	QDict<char> data;
	if (mToolModeRelative -> isChecked()) {
		data.insert ("Mode","Relative");
	}
	if (mToolModeAbsolute -> isChecked()) {
		data.insert ("Mode","Absolute");
	}
	QStringList options;
	for (unsigned int i=0;i<mToolTabletOptions->count();i++) {
	if (mToolTabletOptions->isSelected (i)) {
		QListBoxItem* item = mToolTabletOptions->item (i);
		options.append (item->text());
	}
	}
	QString* allopts = new QString (options.join (","));
	data.insert ("Option",*allopts);

	// save raw options data...
	if (! mToolTabletDataRawOptions.isEmpty()) {
		QDictIterator<char> it (mToolTabletDataRawOptions);
		for (; it.current(); ++it) {
			QString* key = new QString (it.currentKey());
			QString* val = new QString (it.current());
			for (unsigned int i=0;i<mToolTabletOptions->count();i++) {
				QListBoxItem* item = mToolTabletOptions->item (i);
				if (*key == item->text()) {
				if (item -> isSelected()) {
					data.insert (*key,*val);
				}
				}
			}
		}
	}

	if ( mTopToolStatus == "eraser" ) {
		mToolDataEraser = data;
	}
	if ( mTopToolStatus == "pen") {
		mToolDataPen = data;
	}
}

//=====================================
// XTablet slotTopCancel...
//-------------------------------------
void XTablet::slotTopCancel (void) {
	// log (L_INFO,"XTablet::slotTopCancel() called\n");
	// ...
	// this function is called if the configuration dialog
	// for either the pen or the eraser is fisnished with
	// the [Cancel] button
	// ---
}

//=====================================
// XTablet configure eraser...
//-------------------------------------
void XTablet::slotEraser (void) {
	// log (L_INFO,"XTablet::slotEraser() called\n");
	// ...
	// this function is called if the property button
	// for the eraser tool has been pressed to setup
	// special eraser settings. Save eraser data to
	// mToolDataEraser
	// ---
	mTop->show();
	mTopToolStatus = "eraser";
	mToolTabletOptions -> clearSelection();
	mToolTabletDataRawOptions.clear();
    mToolTabletDataRawOptions = mToolDataEraser;
	setupTop ( mTopToolStatus );
}

//=====================================
// XTablet configure pen...
//-------------------------------------
void XTablet::slotPen (void) {
	// log (L_INFO,"XTablet::slotPen() called\n");
	// ...
	// this function is called if the property button
	// for the pen tool has been pressed to setup
	// special pen settings. Save pen data to
	// mToolDataPen
	// ---
	mTop->show();
	mTopToolStatus = "pen";
	mToolTabletOptions -> clearSelection();
	mToolTabletDataRawOptions.clear();
	mToolTabletDataRawOptions = mToolDataPen;
	setupTop ( mTopToolStatus );
}

//=====================================
// XTablet setup tab dialog...
//-------------------------------------
void XTablet::setupTop (const QString& which) {
	// log (L_INFO,"XTablet::slotPen() called\n");
	// ...
	// this function will read the mToolData... hash
	// and setup the tool property dialog according to
	// this information. If one of the data hashes is
	// empty the information about the pen/eraser is
	// retrieved from the predefined CDB data
	// ---
	// 1) get the CDB information record
	// ---
	QDict<char> data;
	QDict<XFile>* mFilePtr = mIntro->getFiles();
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);
	XDb* pCDB = mFiles["cdb_TABLETS"]->cdbGetSpec(
		mVendor->currentText(),mName->currentText()
	);
	if ( ! pCDB) {
		return;
	}
	QDict<char>* spec = pCDB -> getHash();
	XWrapPointer< QDict<char> > selectedTablet (spec);
	// ...
	// 2) check if tool hashes contain data
	// ---
	if ((which == "eraser") && (mToolDataEraser.isEmpty())) {
		QDict<char>* eraser = searchTool (
			mVendor->currentText(),selectedTablet["EraserLink"]
		);
		if (eraser) {
			XWrapPointer< QDict<char> > selectedEraser (eraser);
			if (selectedEraser["Mode"]) {
				QString* mode = new QString (selectedEraser["Mode"]);
				mToolDataEraser.replace ("Mode",*mode);
			}
			if (selectedEraser["Option"]) {
				QString* opts = new QString (selectedEraser["Option"]);
				mToolDataEraser.replace ("Option",*opts);
			}
			if (selectedEraser["RawOption"]) {
				XStringList options;
				options.setText (selectedEraser ["RawOption"]);
				options.setSeperator (",");
				QList<char> optraw = options.getList();
				QListIterator<char> ir (optraw);
				if (! optraw.isEmpty()) {
					QStringList optlist;
					if (selectedEraser["Option"]) {
						optlist = QStringList::split(",",
							selectedEraser["Option"]
						);
					}
					for (; ir.current(); ++ir) {
						options.setText (ir.current());
						options.setSeperator (" ");
						QList<char> keyval = options.getList();
						QString* key = new QString (keyval.at(0));
						QString* val = new QString (keyval.at(1));
						*key = key->stripWhiteSpace();
						*val = val->stripWhiteSpace();
						*key = key->mid (1,key->length() - 2);
						*val = val->mid (1,val->length() - 2);
						// save raw options...
						mToolTabletDataRawOptions.insert (*key,*val);
						mToolDataEraser.insert (*key,*val);
						// append base option to selectedEraser["Option"]...
						optlist.append (*key);
					}
					QString* opts = new QString (optlist.join(","));
					mToolDataEraser.replace ("Option",*opts);
				}
			}
		}
	}
	if ((which == "pen") && (mToolDataPen.isEmpty())) {
		QDict<char>* pen = searchTool (
			mVendor->currentText(),selectedTablet["StylusLink"]
		);
		if (pen) {
			XWrapPointer< QDict<char> > selectedPen (pen);
			if (selectedPen["Mode"]) {
				QString* mode = new QString (selectedPen["Mode"]);
				mToolDataPen.replace ("Mode",*mode);
			}
			if (selectedPen["Option"]) {
				QString* opts = new QString (selectedPen["Option"]);
				mToolDataPen.replace ("Option",*opts);
			}
			if (selectedPen["RawOption"]) {
				XStringList options;
				options.setText (selectedPen ["RawOption"]);
				options.setSeperator (",");
				QList<char> optraw = options.getList();
				QListIterator<char> ir (optraw);
				if (! optraw.isEmpty()) {
					QStringList optlist;
					if (selectedPen["Option"]) {
						optlist = QStringList::split(",",
							selectedPen["Option"]
						);
					}
					for (; ir.current(); ++ir) {
						options.setText (ir.current());
						options.setSeperator (" ");
						QList<char> keyval = options.getList();
						QString* key = new QString (keyval.at(0));
						QString* val = new QString (keyval.at(1));
						*key = key->stripWhiteSpace();
						*val = val->stripWhiteSpace();
						*key = key->mid (1,key->length() - 2);
						*val = val->mid (1,val->length() - 2);
						// save raw options...
						mToolTabletDataRawOptions.insert (*key,*val);
						mToolDataPen.insert (*key,*val);
						// append base option to selectedPen["Option"]...
						optlist.append (*key);
					}
					QString* opts = new QString (optlist.join(","));
					mToolDataPen.replace ("Option",*opts);
				}
			}
		}
	}
	// ...
	// 3) decide for one tool hash and setup dialog data
	// ---
	if (which == "eraser") {
		data = mToolDataEraser;
	}
	if (which == "pen") {
		data = mToolDataPen;
	}
	if (data["Mode"]) {
		if (QString(data["Mode"]) == "Absolute") {
			mToolModeAbsolute -> setChecked ( true );
		} else {
			mToolModeRelative -> setChecked ( true );
		}
	}
	if (data["Option"]) {
		XStringList options;
		options.setText (data["Option"]);
		options.setSeperator (",");
		QList<char> optl = options.getList();
		QListIterator<char> it (optl);
		if (! optl.isEmpty()) {
		for (; it.current(); ++it) {
			for (unsigned int id=0;id < mToolTabletOptions->count();id++) {
				QListBoxItem* item = mToolTabletOptions -> item (id);
				if (QString(it.current()) == item->text()) {
					mToolTabletOptions -> setSelected ( id,true );
					break;
				}
			}
		}
		}
	}
}

//=====================================
// XTablet pen checkbox handling...
//-------------------------------------
void XTablet::slotPenCheck (void) {
	// log (L_INFO,"XTablet::slotPenCheck() called\n");
	// ...
	// called every time the pen checkbox is clicked
	// enable/disable Property button according to
	// selection status
	// ---
	if (mPen -> isChecked()) {
		mPenConfigure -> setDisabled ( false );
	} else {
		mPenConfigure -> setDisabled ( true );
	}
}

//=====================================
// XTablet eraser checkbox handling...
//-------------------------------------
void XTablet::slotEraserCheck (void) {
	// log (L_INFO,"XTablet::slotEraserCheck() called\n");
	// ...
	// called every time the pen checkbox is clicked
	// enable/disable Property button according to
	// selection status
	// ---
	if (mEraser -> isChecked()) {
		mEraserConfigure -> setDisabled ( false );
	} else {
		mEraserConfigure -> setDisabled ( true );
	}
}

//=====================================
// XTablet handle options...
//-------------------------------------
void XTablet::slotOption (QListBoxItem* item) {
	// log (L_INFO,"XTablet::slotOption() called\n");
	// ...
	// this function is called if the option listbox
	// was selected. It checks the type of the option
	// selected and opens a dialog if the option isn't
	// of type <bool>
	// ---
	if (! item -> isSelected()) {
		return;
	}
	int index = mTabletOptions->currentItem();
	if (mKeyOptions[item->text()]) {
		if (! setTypedMessage (
			"optionfor",XBox::Information,
			item->text(),mKeyOptions[item->text()],
			mTabletDataRawOptions[item->text()],
			XBOX_OPT_STRING
		)) {
			mTabletOptions -> setSelected ( index,false );
		} else {
			QString* input = new QString (mTypeString);
			mTabletDataRawOptions.replace (item->text(),*input);
		}
	} else
	if (mRawOptions[item->text()]) {
		if (! setTypedMessage (
			"optionfor",XBox::Information,
			item->text(),mKeyOptions[item->text()],
			mTabletDataRawOptions[item->text()],
			XBOX_OPT_ANY
		)) {
			mTabletOptions -> setSelected ( index,false );
		} else {
			QString* input = new QString (mTypeString);
			mTabletDataRawOptions.replace (item->text(),*input);
		}
	} else {
		return;
	}
}

//=====================================
// XTablet handle tool options...
//-------------------------------------
void XTablet::slotToolOption (QListBoxItem* item) {
	// log (L_INFO,"XTablet::slotToolOption() called\n");
	// ...
	// this function is called if the tool option listbox
	// was selected. It checks the type of the option
	// selected and opens a dialog if the option isn't
	// of type <bool>
	// ---
	if (! item -> isSelected()) {
		return;
	}
	int index = mToolTabletOptions->currentItem();
	if (mKeyOptions[item->text()]) {
		if (! setTypedMessage (
			"optionfor",XBox::Information,
			item->text(),mKeyOptions[item->text()],
			mToolTabletDataRawOptions[item->text()],
			XBOX_OPT_STRING
		)) {
			mToolTabletOptions -> setSelected ( index,false );
		} else {
			QString* input = new QString (mTypeString);
			mToolTabletDataRawOptions.replace (item->text(),*input);
		}
	} else
	if (mRawOptions[item->text()]) {
		if (! setTypedMessage (
			"optionfor",XBox::Information,
			item->text(),mKeyOptions[item->text()],
			mToolTabletDataRawOptions[item->text()],
			XBOX_OPT_ANY
		)) {
			mToolTabletOptions -> setSelected ( index,false );
		} else {
			QString* input = new QString (mTypeString);
			mToolTabletDataRawOptions.replace (item->text(),*input);
		}
	} else {
		return;
	}
}

//=====================================
// XTablet get options per driver...
//-------------------------------------
QStringList XTablet::getOptions ( const QString& driver) {
	// log (L_INFO,"XTablet::getOptions() called\n");
	// ...
	// retrieve options for the given driver name
	// save the string%... options to mKeyOptions and the
	// any%... options to mRawOptions with empty (null) value
	// ---
	mKeyOptions.clear();
	mRawOptions.clear();

	QDict<XFile>* mFilePtr = mIntro->getFiles();
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);
	XApiData tabletOptions = mFiles["ext_TMODULES"]->getAPI();
	QString options ( tabletOptions[driver] );
	QStringList* result = new QStringList ();
	XStringList cur;
	if (! options.isEmpty()) {
		cur.setText (options);
		cur.setSeperator (" ");
		QList<char> strlist = cur.getList();
		QListIterator<char> it (strlist);
		for (; it.current(); ++it) {
			XStringList opt;
			opt.setText (it.current());
			opt.setSeperator (":");
			QList<char> optlist = opt.getList();
			opt.setText (optlist.at(1));
			opt.setSeperator ("%");
			QList<char> optvallist = opt.getList();
			if (QString(optvallist.at(0)) == "bool") {
				result -> append (optlist.at(0));
			}
			if (QString(optvallist.at(0)) == "string") {
				result -> append (optlist.at(0));
				mKeyOptions.insert (optlist.at(0),optvallist.at(1));
			}
			if (QString(optlist.at(1)) == "any") {
				result -> append (optlist.at(0));
				mRawOptions.insert (optlist.at(0),"null");
			}
		}
	}
	return ( *result );
}

//=====================================
// XTablet set OK/Cancel typed box...
//-------------------------------------
int XTablet::setTypedMessage (
	const QString& textKey,XBox::Icon icon,
	const QString& option,const QString& value,
	const QString& current,int type
) {
	XWrapPointer< QDict<char> > mText (mTextPtr);
	QString text (mText[textKey]);
	text.replace (
		QRegExp("\%1"),option
	);
	XBox* mb = new XBox (
		mText["hint"],text,
		icon,XBox::Ok, XBox::Cancel,0,mFrame,
		globalFrameWidth,type
	);
	switch (type) {
	case XBOX_OPT_STRING:
		if (! value.isEmpty()) {
			QStringList items = QStringList::split( ",", value );
			mb->setComboBoxText (items);
			if (! current.isEmpty()) {
				mb->setComboBoxCurrent (current);
			}
		}
	break;
	case XBOX_OPT_ANY:
		if (! current.isEmpty()) {
			mb->setLineEditText (current);
		}
	break;
	}
	mb->setFixedWidth (400);
	mb->setButtonText (
		XBox::Ok,mText["Ok"]
	);
	mb->setButtonText (
		XBox::Cancel, mText["Cancel"]
	);
	int choose = mb->exec();
	switch (choose) {
	case XBox::Cancel: {
		delete (mb); return (0);
	}
	}
	switch (type) {
	case XBOX_OPT_ANY:
		mTypeString = mb->getLineEditText();
	break;
	case XBOX_OPT_STRING:
		mTypeString = mb->getComboBoxText();
	break;
	case XBOX_OPT_INT:
		mTypeValue = mb->getSpinBoxCount();
	break;
	}
	if (mTop->isVisible()) {
		mTop -> enterEvent ( 0 );
	} else {
		mFrame -> enterEvent ( 0 );
	}
	delete (mb);
	return (1);
}

