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
              : configure the monitor properties including
              : sync ranges DDC capabilities and DPMS rules
              :
              : --> signal action slots/callbacks
              :
STATUS        : Status: Up-to-date
**************/
#include "monitor.h"

//=====================================
// external globals
//-------------------------------------
extern int globalFrameWidth;

//=====================================
// XMonitor reset and switch to Intro
//-------------------------------------
void XMonitor::resetPage (int reload) {
	// ...
	// this function is called if the monitor dialog is finished or canceled
	// The changes are serialized and re-imported if the user want
	// to conclude the dialog. Otherwhise the original file is imported
	// and the changes will be lost.
	// ---
	QString update = "sys_DESKTOP";
	if (reload == PAGE_RELOAD) {
		if (! validatePage()) {
			return;
		}
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
		update = "sys_CARD";
		if (mFiles [update] -> sysSerialize()) {
			mFiles [update] -> isModified ( mFrame );
		}
		update = "sys_LAYOUT";
		if (mFiles [update] -> sysSerialize()) {
			mFiles [update] -> isModified ( mFrame );
		}
	}
	if (reload != PAGE_RELOAD) {
		XCard* cardDialog;
		cardDialog = (XCard*) mIntro -> retrieve (Card);
		cardDialog -> resetPage (PAGE_CANCEL);
	}
	mIntro -> importFile (update);
	slotIntro (mIndex);
	XTemplate::resetPage ();
	mStack -> raiseWidget (Intro);
}

//=====================================
// XMonitor validatePage...
//-------------------------------------
bool XMonitor::validatePage (void) {
	// log (L_INFO,"XMonitor::validatePage() called\n");
	// ...
	// this function is called if the finish button is
	// pressed to validate the current page setup
	// ---
	XWrapPointer< QDict<char> > mText (mTextPtr);
	bool valid = false;
    
	// ...
	// check the monitor number in monitorList
	// ---
	if (monitorList->count() >= 1) {
		valid = true;
	}

	// ...
	// give notify message if not valid
	// ---
	if (! valid) {
		XBox mb ( 
		mText["dialoginfo"], mText["monitorinvalid"],
		XBox::Warning,
		XBox::Ok,0,0,mFrame,
		globalFrameWidth
		);
		mb.setButtonText (
		XBox::Ok,mText["Ok"]
		);
		mb.exec();
		mFrame -> enterEvent ( 0 );
	}
	return (valid);
}


//=====================================
// XMonitor internal page called...
//-------------------------------------
void XMonitor::pageCalled (int index) {
	// ...
	// this function is called if the next page of the dialog
	// is called. Note if the dialog has only one page this function
	// will not be called because there is no next page in this case
	// ---
	switch (index) {
	case MonitorPage1: {
		break;
	}
	};
}

//=====================================
// XMonitor init page with data infos 
//-------------------------------------
void XMonitor::initPage (void) {
	// log (L_INFO,"XMonitor::initPage() called\n");
	// ...
	// this function is called after the page was created.
	// It must only be used to init the widgets contents
	// with the data available for selections and other stuff
	// like that
	// ---
	XWrapPointer< QDict<char> > mText (mTextPtr);
	QDict<XFile>* mFilePtr = mIntro->getFiles();
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);

	// insert monitor vendors...
	// -------------------------
	QList<char> vendors = mFiles["cdb_MONITORS"]->cdbGetVendorList();
	QListIterator<char> it (vendors);
	for (; it.current(); ++it) {
		mVendor->insertItem (it.current());
	}

	// include modeline algorithms...
	// ------------------------------
	mMode->insertItem ("CheckDesktopGeometry");
	mMode->insertItem ("UseFrameBufferTiming");
	mMode->insertItem ("XServerPool");

	// include traversal and ratio settings...
	// ---------------------------------------
	XApiData traversal = mFiles["std_MTRAVERSAL"]->getAPI();
	XApiData ratio = mFiles["std_MRATIO"]->getAPI();
	QList<char>* traversalList = traversal.getList();
	QList<char>* ratioList = ratio.getList();
	QListIterator<char> io (*traversalList);
	for (; io.current(); ++io) {
		QString info;
		QTextOStream (&info) << io.current() << " " << mText["zoll"];
		mTraversal -> insertItem (info);
	}
	QListIterator<char> ir (*ratioList);
	for (; ir.current(); ++ir) {
		mRatio -> insertItem (ir.current());
	}
}

//=====================================
// XMonitor virtual slots...
//-------------------------------------
bool XMonitor::slotRun (int index) {
	if (XTemplate::slotRun (index)) {
	// log(L_INFO,"XMonitor::slotRun() called: %d\n",index);
	// ...
	// this function is called if the monitor page is activated.
	// use this function to init the dialog with the current
	// setup of the monitor(s)
	// ---
	XWrapPointer< QDict<char> > mText (mTextPtr);
	mStatus -> message (mText["RunXMonitor"]);
	mFrame  -> nextButton() -> setText (mText["finish"]);

	// ...
	// get the mFiles pointer wrapper from the Intro
	// object which has read all the data files
	// ---
	QDict<XFile>* mFilePtr = mIntro->getFiles();
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);

	// ...
	// fill the monitorList listbox with the 
	// available monitors 
	// ---
	monitorList -> clear();
	QString topic;
	XData* sysData;
	QDict<char> monitorInfo;
	bool havePrimaryDisplaySize = false;
	int monitors;
	monitors = mFiles["sys_DESKTOP"]->getDeviceCount();
	for (int n=0; n<=monitors; n++) {
		sysData = mFiles["sys_DESKTOP"] -> getDevice(n);
		if (! sysData) {
			continue;
		}
		monitorInfo = sysData -> getData();
		if ( (n==0) && (monitorInfo["DisplaySize"]) ) {
			QString sizestr (monitorInfo["DisplaySize"]);
			XStringList dpi (sizestr);
			dpi.setSeperator (" ");
			QList<char> size = dpi.getList();
			if (size.count() == 2) {
				QString x (size.at(0));
				QString y (size.at(1));
				if ( (x.toInt() > 0) && (y.toInt() > 0) ) {
					havePrimaryDisplaySize = true;
				} 
			}
		}
		QString model (monitorInfo["VendorName"]);
		QString name  (monitorInfo["ModelName"]);
		if (model != name) {
			model += " "; model += name;
		}
		topic.sprintf ( "%s",model.ascii() );
		monitorList -> insertItem (
		QPixmap(PIXMONITOR),topic
		);
	}

	// ...
	// enable or disable the configure button
	// ---
	if (monitors > 0) {
		mSetup -> setDisabled (false);
		monitorList -> setCurrentItem ( 0 );
		monitorList -> setSelected    ( 0,true );
		mSelected = 0;
	} else {
		mSetup -> setDisabled (true);
	}
	// ...
	// check for valid DisplaySize on primary display
	// ---
	if (! havePrimaryDisplaySize) {
		XBox* mb = new XBox (
			mText["sizeinfo"],mText["sizetext"],
			XBox::Information, XBox::Ok, XBox::Cancel, 0,mFrame,
			globalFrameWidth
		);
		mb->setButtonText (
			XBox::Ok,mText["Ok"]
		);
		mb->setButtonText (
			XBox::Cancel, mText["Cancel"]
		);
		int choose = mb->exec();
		switch (choose) {
		case XBox::Ok: {
			slotSetup();
			mTop -> showPage (layer11);
		}
		}
		delete (mb);
	}
	}
	return (TRUE);
}

//=====================================
// XMonitor init function for toplevel
//-------------------------------------
void XMonitor::setupTop ( void ) {
	// log (L_INFO,"XMonitor::setupTop() called\n");
	// ...
	// this function is called if the setup toplevel window
	// is created to configure the selected monitor
	// ---
	// get the mFiles pointer wrapper from the Intro
	// object which has read all the data files
	// ---
	XWrapPointer< QDict<char> > mText (mTextPtr);
	QDict<XFile>* mFilePtr = mIntro->getFiles();
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);

	// ...
	// first get the selected monitor and try to select
	// the monitor in the CDB base list
	// ---
	XWrapPointer<XData> workingMonitor (
		mFiles["sys_DESKTOP"]->getDevice (mSelected)
	);
	// select vendor
	mVendor -> clearSelection();
	mName   -> clear();
	for (int i=0;i < (int)mVendor->count();i++) {
	if (mVendor -> text(i) == QString(workingMonitor["VendorName"])) {
		mVendor -> setSelected (i,true);
		slotVendor (mVendor -> item(i));
		break;
	}
	}
	// select name...
	for (int i=0;i < (int)mName->count();i++) {
	if (mName -> text(i) == QString(workingMonitor["ModelName"])) {
		mName -> setSelected (i,true);
		break;
	}
	}

	// ...
	// setup sync ranges, DPMS option
	// DisplaySize and CalcAlgorithm
	// ---
	// VertRefresh...
	QString vsyncstr (workingMonitor["VertRefresh"]);
	XStringList vsync (vsyncstr); 
	vsync.setSeperator ("-");
	QList<char> vrange = vsync.getList();
	QString vmin (vrange.at(0));
	QString vmax (vrange.at(1));
	mVSpinMin -> setValue (vmin.toInt());
	mVSpinMax -> setValue (vmax.toInt());

	// HorizSync...
	QString hsyncstr (workingMonitor["HorizSync"]);
	XStringList hsync (hsyncstr);
	hsync.setSeperator ("-");
	QList<char> hrange = hsync.getList();
	QString hmin (hrange.at(0));
	QString hmax (hrange.at(1));
	mHSpinMin -> setValue (hmin.toInt());
	mHSpinMax -> setValue (hmax.toInt());

	// CalcAlgorithm...
	int index = 0;
	if (workingMonitor["CalcAlgorithm"]) {
	QString mode (workingMonitor["CalcAlgorithm"]);
	if (mode == "CheckDesktopGeometry") {
		index = 0;
	}
	if (mode == "UseFrameBufferTiming") {
		index = 1;
	}
	if (mode == "XServerPool") {
		index = 2;
	}
	}
	mMode -> setCurrentItem (index);

	// DPMS...
	mDPMS -> setChecked (false);
	if (workingMonitor["MonitorOptions"]) {
		QString opt (workingMonitor["MonitorOptions"]);
		XStringList optstr (opt);
		optstr.setSeperator (",");
		QList<char> optlist = optstr.getList();
		QListIterator<char> it (optlist);
		for (; it.current(); ++it) {
		if (QString(it.current()).lower() == "dpms") {
			mDPMS -> setChecked (true);
			break;
		}
		}
	}
	
	// DisplaySize...
	mXspin -> setValue (0);
	mYspin -> setValue (0);
	if (workingMonitor["DisplaySize"]) {
		QString sizestr (workingMonitor["DisplaySize"]);
		XStringList dpi (sizestr);
		dpi.setSeperator (" ");
		QList<char> size = dpi.getList();
		if (size.count() == 2) {
			QString x (size.at(0)); mXspin -> setValue (x.toInt());
			QString y (size.at(1)); mYspin -> setValue (y.toInt());
			// set traversal and aspect/ratio
			double traversal = round (getTraversal (x.toInt(),y.toInt()));
			QPoint ratio = getRatio (x.toInt(),y.toInt());
			QString infoTraversal;
			QString infoRatio;
			QTextOStream (&infoTraversal) << traversal << " " << mText["zoll"];
			mTraversal -> setCurrentText (infoTraversal);
			QTextOStream (&infoRatio) << ratio.x() << "/" << ratio.y();
			mRatio -> setCurrentText (infoRatio);
		}
	}
	
	// ...
	// Save initial values for frequency and algorithm 
	// method. The values are used later to decide if
	// we need to recalculate Modelines
	// ---
	mInitialAlgorithm.sprintf("%s",
	mMode->currentText().ascii()
	);
	mInitialHsync.sprintf("%s-%s",
	mHSpinMin->text().ascii(),mHSpinMax->text().ascii()
	);
	mInitialVsync.sprintf("%s-%s",
	mVSpinMin->text().ascii(),mVSpinMax->text().ascii()
	);
}

//=====================================
// XMonitor select one desktop...
//-------------------------------------
void XMonitor::slotSelect ( QListBoxItem* item ) {
	// log (L_INFO,"XMonitor::slotSelect() called\n");
	// ...
	// this function is called if you select a
	// desktop from the main desktop list
	// ---
	if (monitorList->isSelected (item)) {
		mSelected = monitorList -> currentItem();
	}
}

//=====================================
// XMonitor double click on desktop...
//-------------------------------------
void XMonitor::slotDoubleSelect ( QListBoxItem* item ) {
	// log (L_INFO,"XMonitor::slotDoubleSelect() called\n");
	// ...
	// this function is called if a double click changes
	// desktop main selection
	// ---
	if (monitorList->isSelected (item)) {
		QString* text = new QString;
		QTextOStream (text)
			<< mSelected << ", ,This screen is selected";
		QString optd ("-d");
		QString optr ("-R");
		qx (XQUERY,STDNONE,4,"%s %s %s %s",
			optd.ascii(),DisplayString(x11Display()),optr.ascii(),text->ascii()
		);
	}
}

//=====================================
// XMonitor configure selection...
//-------------------------------------
void XMonitor::slotSetup (void) {
	// log (L_INFO,"XMonitor::slotSetup() called\n");
	// ...
	// this function is called if you click onto the
	// configure button to setup the selected 
	// desktop
	// ---
	if ( ! monitorList->isSelected (mSelected) ) {
		return;
	}
	setupTop ();
	mTop -> show();
	mTop -> enterEvent ( 0 );
}

//=====================================
// XMonitor remove Monitor/Card...
//-------------------------------------
void XMonitor::slotRemove (void) {
	// log (L_INFO,"XMonitor::slotRemove() called\n");
	// ...
	// this function is called if a montor resp. the
	// card it belongs to should be removed.
	// ---
	XWrapPointer< QDict<char> > mText (mTextPtr);
	if (monitorList->count() == 1) {
		XBox mb (
			mText["dialoginfo"], mText["cannotremovemonitor"],
			XBox::Critical,
			XBox::Ok,0,0,mFrame,
			globalFrameWidth
		);
		mb.setButtonText (
			XBox::Ok,mText["Ok"]
		);
		mb.exec();
		mFrame -> enterEvent ( 0 );
		return;
	}
	XCard* cardDialog;
	cardDialog = (XCard*) mIntro -> retrieve (Card);
	cardDialog -> mSelected = mSelected;
	cardDialog -> slotRemove ();
	monitorList-> removeItem (mSelected);
}

//=====================================
// XMonitor select monitor vendor...
//-------------------------------------
void XMonitor::slotVendor ( QListBoxItem* item ) {
	// log (L_INFO,"XMonitor::slotVendor()...\n");
	// ...
	// fill the name listbox with the devices
	// apropriate to the selected vendor
	// ---
	QDict<XFile>* mFilePtr = mIntro->getFiles();
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);
	if (! mVendor->isSelected (item)) {
		return;
	} 
	mName -> clear();
	QString selectedVendor = mVendor -> currentText();
	QList<char> names = mFiles["cdb_MONITORS"]->cdbGetNameList (selectedVendor);
	QListIterator<char> it (names);
	for (; it.current(); ++it) {
		mName -> insertItem (it.current());
	}
	mName -> sort();
}

//=====================================
// XMonitor select monitor name...
//-------------------------------------
void XMonitor::slotName ( QListBoxItem *) {
	// log (L_INFO,"XMonitor::slotName()...\n");
	// ...
	// get the mFiles pointer wrapper from the Intro
	// object which has read all the data files
	// ---
	QDict<XFile>* mFilePtr = mIntro->getFiles();
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);

	// ...
	// get the selection data from the CDB
	// ---
	QDict<char>* spec = NULL;
	XDb* pCDB = mFiles["cdb_MONITORS"]->cdbGetSpec(
		mVendor->currentText(),mName->currentText()
	);
	if (pCDB) {
	// ...
	// init dialog values for DPMS, DisplaySize
	// and modeline algorithm
	// ---
	mDPMS  -> setChecked (false);
	mXspin -> setValue (0);
	mYspin -> setValue (0);

	// ...
	// get the CDB information to setup the dialog widgets
	// note: we will not set the monitor spec to the sys_DESKTOP
	// structure here... see slotTopOk()
	// ---
	spec = pCDB -> getHash();
	QDictIterator<char> it (*spec);
	for (; it.current(); ++it) {
		QString key (it.currentKey());
		QString* val = new QString;
		val->sprintf("%s",it.current());
		if (val->isEmpty()) {
			continue;
		}
		if (key == "Vsync")     {
			// VertRefresh...
			XStringList sync (*val);
			sync.setSeperator ("-");
			QList<char> range = sync.getList();
			QString min (range.at(0));
			QString max (range.at(1));
			mVSpinMin -> setValue (min.toInt());
			mVSpinMax -> setValue (max.toInt());
		}
		if (key == "Hsync")     {
			// HorizSync...
			XStringList sync (*val);
			sync.setSeperator ("-");
			QList<char> range = sync.getList();
			QString min (range.at(0));
			QString max (range.at(1));
			mHSpinMin -> setValue (min.toInt());
			mHSpinMax -> setValue (max.toInt());
		}
		if (key == "Option")    {
			// MonitorOptions...
			XStringList opt (*val);
			opt.setSeperator (",");
			QList<char> optlist = opt.getList();
			QListIterator<char> it (optlist);
			for (; it.current(); ++it) {
			if (QString(it.current()).lower() == "dpms") {
				mDPMS -> setChecked (true);
				break;
			}
			}
		}
		if (key == "Modeline")  {
			// SpecialModeline...
		}
		if (key == "DPI")       {
			// DisplaySize...
			XStringList dpi (*val);
			dpi.setSeperator (" ");
			QList<char> size = dpi.getList();
			QString x (size.at(0));
			QString y (size.at(1));
			mXspin -> setValue (x.toInt());
			mYspin -> setValue (y.toInt());
		}
	}
	}
	// ...
	// Try to set a DisplaySize value according to
	// the current resolution
	// ---
	#if 1
	if ((mXspin->value() == 0) || (mYspin->value() == 0)) {
		XData* sysData;
		QDict<char> desktopInfo;
		sysData = mFiles["sys_DESKTOP"] -> getDevice (mSelected);
		if (sysData) {
			desktopInfo = sysData -> getData();
			QString defaultColor (desktopInfo["ColorDepth"]);
			QString reskey; reskey.sprintf("Modes:%s",defaultColor.ascii());
			XStringList res (desktopInfo[reskey]);
			res.setSeperator (",");
			QString primary = res.getList().at(0);
			XStringList strRes (primary);
			strRes.setSeperator ("x");
			QList<char> xy = strRes.getList();
			int x = QString(xy.at(0)).toInt();
			int y = QString(xy.at(1)).toInt();
			long p = x * y;
			if (p >= 1920000) {
			//===========================================================
			// equal or more than 1600 x 1200 pixels, assume 19'z monitor
			//-----------------------------------------------------------
			mXspin -> setValue (360);
       	    mYspin -> setValue (270);

			} else if (p > 786432) {
			//===========================================================
			// more than 1024 x 768 pixels assume 17'z monitor
			//-----------------------------------------------------------
			mXspin -> setValue (320);
			mYspin -> setValue (240);
			}
		}
	}
	#endif
}

//=====================================
// XMonitor utility disk requested...
//-------------------------------------
void XMonitor::slotDisk ( void ) {
	// log (L_INFO,"XMonitor::slotDisk()...\n");
	// ...
	// this function will ask for the Microsoft Windows
	// .inf disc provided from the monitor manufacturer.
	// We are trying to read the contents of the disk and
	// include the data to the selection list
	// ---
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);
	XWrapPointer < QDict<char> > mText (mTextPtr);

	// ...
	// ask for the monitor manufacturer disk
	// ---
	XBox* mb = new XBox (
		mText["diskinfo"],mText["disktext"],
		XBox::Information, XBox::Ok, XBox::Cancel, 0,mFrame, 
		globalFrameWidth
	);
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
		delete (mb); return;
	}
	}
	qApp->setOverrideCursor (
		Qt::forbiddenCursor
	);
	QString diskData = qx (GETINF,STDOUT);
	qApp->restoreOverrideCursor();
	delete (mb);
	if (! diskData.isEmpty()) {
		QString vendor;
		QString name;

		XStringList lines (diskData);
		lines.setSeperator ("\n");
		QList<char> monitorModels = lines.getList();
		QListIterator<char> it (monitorModels);
		for (; it.current(); ++it) {
			XStringList items (it.current());
			items.setSeperator (",");
			QList<char> monitorData = items.getList();

			QDict<char> property;
			property.insert ("Vsync",monitorData.at(2));
			property.insert ("Hsync",monitorData.at(3));
			
			mFiles["cdb_MONITORS"]->cdbAddXDbSection (
				monitorData.at(0),monitorData.at(1),property
			);
			vendor = monitorData.at(0);
			name   = monitorData.at(1);
		}
		mVendor -> clear();
		mName   -> clear();
		QList<char> vendors = mFiles["cdb_MONITORS"]->cdbGetVendorList();
		QListIterator<char> iv (vendors);
		for (; iv.current(); ++iv) {
			mVendor->insertItem (iv.current());
		}
		// select vendor
		for (int i=0;i < (int)mVendor->count();i++) {
		if (mVendor -> text(i) == vendor) {
			mVendor -> setSelected (i,true);
			mVendor -> ensureCurrentVisible ();
			slotVendor (mVendor -> item(i));
			break;
		}
		}
		// select name...
		for (int i=0;i < (int)mName->count();i++) {
		if (mName -> text(i) == name) {
			mName -> setSelected (i,true);
			mName -> ensureCurrentVisible ();
			break;
		}
		}
	} else {
		// ...
		// Ups, could not read any data from the disk
		// ---
		XBox mb (
			mText["diskinfo"],mText["diskerror"],
			XBox::Critical, XBox::Ok, 0,0,mFrame, globalFrameWidth
		);
		mb.setButtonText (
			XBox::Ok,mText["Ok"]
		);
		mb.exec();
		mTop -> enterEvent ( 0 );
	}
}

//=====================================
// XMonitor hsync value changed...
//-------------------------------------
void XMonitor::slotHsyncMin ( int ) {
	// log (L_INFO,"XMonitor::slotHsyncMin()...\n");
}

//=====================================
// XMonitor vsync value changed...
//-------------------------------------
void XMonitor::slotVsyncMin ( int ) {
	// log (L_INFO,"XMonitor::slotVsyncMin()...\n");
}

//=====================================
// XMonitor hsync value changed...
//-------------------------------------
void XMonitor::slotHsyncMax ( int ) {
	// log (L_INFO,"XMonitor::slotHsyncMax()...\n");
}

//=====================================
// XMonitor vsync value changed...
//-------------------------------------
void XMonitor::slotVsyncMax ( int ) {
	// log (L_INFO,"XMonitor::slotVsyncMax()...\n");
}

//=====================================
// XMonitor modeline algorithm set...
//-------------------------------------
void XMonitor::slotAlgorithm ( int ) {
	// log (L_INFO,"XMonitor::slotAlgorithm()...\n");
}

//=====================================
// XMonitor DisplaySize X changed...
//-------------------------------------
void XMonitor::slotXSize ( int ) {
	// log (L_INFO,"XMonitor::slotXSize()...\n");
}

//=====================================
// XMonitor DisplaySize Y changed...
//-------------------------------------
void XMonitor::slotYSize ( int ) {
	// log (L_INFO,"XMonitor::slotYSize()...\n");
}

//=====================================
// XMonitor slotTopOk...
//-------------------------------------
void XMonitor::slotTopOk (void) {
	// log (L_INFO,"XMonitor::slotTopOk() called\n");
	// ...
	// this function is called if you click onto the OK 
	// button in the setup toplevel window
	// ---
    // get the mFiles pointer wrapper from the Intro
    // object which has read all the data files
    // ---
	mFrame -> enterEvent ( 0 );
    QDict<XFile>* mFilePtr = mIntro->getFiles();
    XWrapFile < QDict<XFile> > mFiles (mFilePtr);

	// ...
	// get the workingMonitor pointer wrapper. get the device 
	// according to the selection made in monitorList
    // ---
	XWrapPointer<XData> workingMonitor (
		mFiles["sys_DESKTOP"]->getDevice (mSelected)
	);

	// ...
	// set the specs which comes with the CDB monitor
	// selection and could not be modified via 
	// the expert dialog:
	// [ SpecialModeline ]
	// [ ModelName       ]
	// [ VendorName      ]
	// ---
	QDict<char>* spec = NULL;
	XDb* pCDB = mFiles["cdb_MONITORS"]->cdbGetSpec(
		mVendor->currentText(),mName->currentText()
	);
	if (pCDB) {
		// ...
		// first check if the selected monitor name contains
		// a resolution string. If yes this resolution will be
		// used for the workingMonitor
		// ---
		QRegExp resExp ("[0-9]+x[0-9]+");
		resExp.setCaseSensitive (false);
		int rpos = resExp.search (mName->currentText(),0);
		if (rpos >= 0) {
			QString res = mName->currentText().lower();
			int npos = 0;
			for (int n=rpos;n<(int)res.length();n++) {
			if ((res.at(n) == 'x') || (res.at(n).isDigit())) {
				npos++;
			} else {
				break;
			}
			}
			QString* substr = new QString (
				res.mid (rpos,npos)
			);
			QString reskey;
			QDict<char> desktopInfo = workingMonitor.getData();
			reskey.sprintf("Modes:%d",
				QString(desktopInfo["ColorDepth"]).toInt()
			);
			workingMonitor.setPair (
				reskey,substr->ascii()
			);
		}
		// ...
		// next check for all the configuration information
		// according to the selected monitor
		// ---
		spec = pCDB -> getHash();
		QDictIterator<char> it (*spec);
		for (; it.current(); ++it) {
		QString key (it.currentKey());
		QString* val = new QString;
		val->sprintf("%s",it.current());
		if (val->isEmpty()) {
			continue;
		}
		if (key == "Modeline") {
			workingMonitor.setPair ("SpecialModeline",val->ascii());
		}
		}
		QString* vendor = new QString ( mVendor->currentText() );
		QString* name   = new QString ( mName->currentText() );
		workingMonitor.setPair (
			"ModelName" ,name->ascii()
		);
		workingMonitor.setPair (
			"VendorName",vendor->ascii()
		);
	}

	// ...
	// save the current sync range settings
	// this will affect the following desktop keys:
	// [ HorizSync   ]
	// [ VertRefresh ]
	// ---
	QString* hsync = new QString();
	QString* vsync = new QString();
	hsync -> sprintf (
		"%s-%s",mHSpinMin->text().ascii(),mHSpinMax->text().ascii()
	);
	vsync -> sprintf (
		"%s-%s",mVSpinMin->text().ascii(),mVSpinMax->text().ascii()
	);
	workingMonitor.setPair("HorizSync"  ,hsync->ascii());
	workingMonitor.setPair("VertRefresh",vsync->ascii());

	// ...
	// check if DPMS is enabled for this monitor
	// this will affect the [ MonitorOptions ] key
	// ---
	workingMonitor.setPair("MonitorOptions","");
	if (mDPMS->isChecked()) {
		workingMonitor.setPair("MonitorOptions","DPMS");
	}

	// ...
	// set current DisplaySize for this monitor
	// this will affect the [ DisplaySize ] key
	// ---
	workingMonitor.setPair("DisplaySize","");
	if (
	 (QString(mXspin->text()).toInt() != 0) &&
	 (QString(mYspin->text()).toInt() != 0)
	) {
		QString* size = new QString();
		size -> sprintf ("%s %s",mXspin->text().ascii(),mYspin->text().ascii());
		workingMonitor.setPair("DisplaySize",size->ascii());
	}

	// ...
	// set current algorithm method used to calculate 
	// modelines for this monitor
	// ---
	workingMonitor.setPair (
		"CalcAlgorithm",mMode->currentText()
	);	

	// ...
	// check if we need to calcualte modelines. This check is
	// based on the following changes to the dialog:
	// 1) if frequencies were changed
	// 2) if the calc algorithm method has changed
	// 3) if the primary resolution has changed. This is made
	//    in the handler source at the desktop tree
	// ---
	bool recalc = false;
	workingMonitor.setPair("CalcModelines","no");
	if (mInitialAlgorithm != workingMonitor["CalcAlgorithm"]) {
		recalc = true;
	} else {
	if (mInitialHsync != workingMonitor["HorizSync"]) {
		recalc = true;
	} else {
	if (mInitialVsync != workingMonitor["VertRefresh"]) {
		recalc = true;
	}
	}
	}
	if (recalc) {
		workingMonitor.setPair("CalcModelines","on");
		workingMonitor.setPair("Modelines","");
	} 

	// ...
	// If the user selected XServerPool as algorithm method
	// we will disable the calculation of modelines 
	// ---
	if (mMode->currentText() == "XServerPool") {
		workingMonitor.setPair("CalcModelines","no");
	}

	// ...
	// update the name of the monitor in the
	// monitorList
	// ---
	QString topic;
	topic.sprintf ("%s %s",
		workingMonitor["VendorName"].ascii(),workingMonitor["ModelName"].ascii()
	);
	monitorList -> changeItem (
		QPixmap(PIXMONITOR),topic,mSelected
	);
}

//=====================================
// XMonitor slotTopCancel...
//-------------------------------------
void XMonitor::slotTopCancel (void) {
	// log (L_INFO,"XMonitor::slotTopCancel() called\n");
	// ...
	// this function is called if you click onto the Cancel 
	// button in the setup toplevel window
	// ---
	mFrame -> enterEvent ( 0 );
}

//=====================================
// XMonitor slotTraversal...
//-------------------------------------
void XMonitor::slotTraversal (int) {
	// log (L_INFO,"XMonitor::slotTraversal() called\n");
	// ...
	// this function is called if you select a traversal 
	// from the combobox located in the properties dialog
	// ---
	QString selected = mTraversal -> currentText();
	QStringList selectList = QStringList::split ( " ", selected );
	double traversal = selectList.first().toDouble();
	selected = mRatio -> currentText();
	selectList = QStringList::split ( "/", selected );
	int aspect = selectList.first().toInt();
	int ratio  = selectList.last().toInt();
	QPoint n = translateTraversal (traversal,aspect,ratio);
	mXspin -> setValue (n.x());
	mYspin -> setValue (n.y());
}

//=====================================
// XMonitor slotRatio...
//-------------------------------------
void XMonitor::slotRatio (int) {
	// log (L_INFO,"XMonitor::slotRatio() called\n");
	// ...
	// this function is called if you select an aspect/ ratio 
	// from the combobox located in the properties dialog
	// ---
	slotTraversal (0);
}

//=====================================
// XMonitor translateTraversal...
//-------------------------------------
QPoint XMonitor::translateTraversal (double traversal,int aspect, int ratio) {
	// log (L_INFO,"XMonitor::translateTraversal() called\n");
	// ...
	// calculate x and y sizes in mm refering to traversal and
	// aspect / ratio
	// ---
	QPoint result;
	traversal = traversal * 25.4;
	double ar = (double)aspect / (double)ratio;
	double y = sqrt ( (traversal * traversal) / (ar * ar + 1.0) );
	double x = ar * y;
	result.setX ( (int)(round(x)) );
	result.setY ( (int)(round(y)) );
	return result;
}

//=====================================
// XMonitor getTraversal...
//-------------------------------------
double XMonitor::getTraversal (int x,int y) {
	// log (L_INFO,"XMonitor::getTraversal() called\n");
	// ...
	// calculate the length of the traversal refering to
	// the length of x and y
	// ---
	return (sqrt (x*x + y*y) / 25.4);
}

//=====================================
// XMonitor getRatio...
//-------------------------------------
QPoint XMonitor::getRatio (int x,int y) {
	// log (L_INFO,"XMonitor::getRatio() called\n");
	// ...
	// calculate the aspect/ratio refering to
	// the length of x and y
	// ---
	QPoint result;
	result.setX (4);
	result.setY (3);
	double ar = round ((double)x/(double)y);
	if ( ar > 1.0 ) {
		result.setX (16);
		result.setY (10);
	}
	return result;
}
