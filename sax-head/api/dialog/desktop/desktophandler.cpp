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
              : configure colors and resolution for each desktop
              : bound to a graphics card
              :
              :
STATUS        : Status: Up-to-date
**************/
#include "desktop.h"

//=====================================
// external globals
//-------------------------------------
extern int globalFrameWidth;

//=====================================
// XDesktop reset and switch to Intro
//-------------------------------------
void XDesktop::resetPage (int reload) {
	// ...
	// this function is called if the desktop dialog is finished or canceled
	// The changes are serialized and re-imported if the user want
	// to conclude the dialog. Otherwhise the original file is imported
	// and the changes will be lost.
	// ---
	QString update = "sys_DESKTOP";
	QString liloCode;

	if (reload == PAGE_RELOAD) {
	// ...
	// check if we had to write the YCP data file with the
	// lilo code if the framebuffer driver was configured
	// ---
	unlink (LILOCODE);

	XData* data;
	QDict<char> card;
	bool writeLILOCode = false;
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);

	// ...
	// get graphics card dialog pointer
	// ---
	XCard* cardDialog;
	cardDialog = (XCard*) mIntro -> retrieve (Card);
	XWrapPointer<XData> workingDesktop (
		mFiles["sys_DESKTOP"]->getDevice ( cardDialog -> getSelected() )
	);

	// ...
	// if the current color depth is unknown ask for it
	// within the current used data record
	// ---
	#if 1
	if (&workingDesktop) {
	if (mCurrentColorDepth == -1) {
		QDict<char> desktop = workingDesktop.getData();
		mCurrentColorDepth = QString(desktop["ColorDepth"]).toInt();
		int col[5] = {4,8,15,16,24};
		for (int n=0;n<5;n++) {
			QString reskey; reskey.sprintf("Modes:%d",col[n]);
			XStringList res;
			res.setText (desktop[reskey]);
			res.setSeperator (",");
			switch (col[n]) {
				case 4:
					m4ConfigRes  = res.getList();
				break;
				case 8:
					m8ConfigRes  = res.getList();
				break;
				case 15:
					m15ConfigRes = res.getList();
				break;
				case 16:
					m16ConfigRes = res.getList();
				break;
				case 24:
					m24ConfigRes = res.getList();
				break;
			}
		}
	}
	}
	#endif

	// ...
	// Try to setup DisplaySize if not set
	// ---
	#if 1
	if (! workingDesktop["DisplaySize"]) {
		QString defaultColor (workingDesktop["ColorDepth"]);
		QString reskey; reskey.sprintf("Modes:%s",defaultColor.ascii());
		XStringList res (workingDesktop[reskey]);
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
		workingDesktop.setPair ("DisplaySize","360 270");
		log (L_INFO,"XDesktop::resetPage() set DisplaySize: %s\n",
			workingDesktop["DisplaySize"].ascii()
		);
		} else if (p > 786432) {
		//===========================================================
		// more than 1024 x 768 pixels assume 17'z monitor
		//-----------------------------------------------------------
		workingDesktop.setPair ("DisplaySize","320 240");
		log (L_INFO,"XDesktop::resetPage() set DisplaySize: %s\n",
			workingDesktop["DisplaySize"].ascii()
		);
		}
	}
	#endif

	// ...
	// Check if there really is no need to calculate
	// modelines. If someone add own resolutions this must be
	// corrected
	// ---
	if (mNewResAdded) {
	for (int n=0; n<mFiles["sys_DESKTOP"]->getDeviceCount(); n++) {
		XWrapPointer<XData> workingDesktop (
			mFiles["sys_DESKTOP"]->getDevice ( n )
		);
		workingDesktop.setPair (
			"CalcModelines","on"
		);
	}
	}

	// ...
	// walk through all cards and check if one is fbdev
	// based
	// ---
	QList<char> handleList;
	mFramebufferUsed = false;
	for (int n=0; n<=mFiles["sys_CARD"]->getDeviceCount(); n++) {
		data = mFiles["sys_CARD"] -> getDevice ( n );
		if (! data) {
			continue;
		}
		card = data -> getData();
		if (QString(card["Driver"]) == "fbdev") {
			mFramebufferUsed = true;
			XApiData fbresolution = mFiles["ext_MFBRESOLUTION"]->getAPI();
			mFBResPtr = fbresolution.getHash();
			int liloIndex = -1;
			switch (mCurrentColorDepth) {
			case 8:
				handleList = m8ConfigRes;
				liloIndex = 0;
			break;
			case 16:
				handleList = m16ConfigRes;
				liloIndex = 1;
			break;
			case 24:
				handleList = m24ConfigRes;
				liloIndex = 2;
			break;
			}
			// ...
			// Update resolution list for framebuffer usage
			// add only one resolution to the list
			// ---
			if (&workingDesktop) {
			if (handleList.count() >= 1) {
				QString reskey; reskey.sprintf("Modes:%d",mCurrentColorDepth);
				QString* mainResolution = new QString ( handleList.at(0) );
				workingDesktop.setPair (
					reskey,mainResolution->ascii()
				);
			}
			}
			// ...
			// Get lilo code and check if the current
			// resolution is valid for framebuffer usage
			// ---
			if (liloIndex >= 0) {
				QDictIterator<char> it (*mFBResPtr);
				for (; it.current(); ++it) {
				if (it.currentKey() == handleList.at(0)) {
					XStringList liloInfo (it.current());
					liloInfo.setSeperator (",");
					liloCode = liloInfo.getList().at (liloIndex);
				}
				}
				if (liloCode.toInt() > 0) {
					writeLILOCode = true;
				} else {
					setMessage ("invalidResolution",
						XBox::Warning, handleList.at(0)
					);
				}
			}
			// ...
			// break from loop after the first
			// fbdev based card was found
			// ---
			break;
		}
	}
	if (writeLILOCode) {
		writeLILO (liloCode.toInt(),handleList.at(0));
	}
	}

	// ...
	// update ISaX data files...
	// ---
	mStatus -> clear();
	if (reload == PAGE_RELOAD) {
		mFilePtr = mIntro->getFiles();
		XWrapFile < QDict<XFile> > mFiles (mFilePtr);
		if (mFiles [update] -> sysSerialize()) {;
			mFiles [update] -> isModified ( mFrame );
		}
    }
	mIntro -> importFile (update);
	slotIntro (mIndex);
	XTemplate::resetPage ();
	mStack -> raiseWidget (Intro);
}

//=====================================
// XDesktop internal page called...
//-------------------------------------
void XDesktop::pageCalled (int index) {
	// ...
	// this function is called if the next page of the dialog
	// is called. Note if the dialog has only one page this function
	// will not be called because there is no next page in this case
	// ---
	switch (index) {
	case DesktopPage1: {
		break;
	}
	};
}

//=====================================
// XDesktop init page with data infos 
//-------------------------------------
void XDesktop::initPage (void) {
    // log (L_INFO,"XDesktop::initPage() called\n");
    // ...
    // this function is called after the page was created.
    // It must only be used to init the widgets contents
    // with the data available for selections and other stuff
    // like that
    // ---
	QDict<XFile>* mFilePtr = mIntro->getFiles();
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);

	// insert available colors to combo box...
	// insert resolutions to "possible" list...
	// ----------------------------------------
	char depth[][5] = {"4","8","15","16","24"};
	XApiData resolution = mFiles["std_MRESOLUTION"]->getAPI();
	XApiData colors = mFiles["ext_MCOLORS"]->getAPI();
	mColorPtr = colors.getHash();
	QList<char>* rlist = resolution.getList();

	for (int i=0;i<5;i++) {
	mColor -> insertItem (mColorPtr->operator[](depth[i]));
	QListIterator<char> it (*rlist);
	for (; it.current(); ++it) {
		if (i==0) {
		(void) new XCheckListItem (
			mResolution,it.current(),QCheckListItem::CheckBox,mIntro
		);
		}
	}
	}
}

//=====================================
// XDesktop virtual slots...
//-------------------------------------
bool XDesktop::slotRun (int index) {
	if (XTemplate::slotRun (index)) {
	// log(L_INFO,"XDesktop::slotRun() called: %d\n",index);
	// ...
	// this function is called if the desktop page is activated.
	// use this function to init the dialog with the current
	// setup of the desktop(s)
	// ---
	XWrapPointer< QDict<char> > mText (mTextPtr);
	mStatus -> message (mText["RunXDesktop"]);
	mFrame  -> nextButton() -> setText (mText["finish"]);

	// ...
	// get the mFiles pointer wrapper from the Intro
	// object which has read all the data files
	// ---
	QDict<XFile>* mFilePtr = mIntro->getFiles();
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);

	// ...
	// fill the mDesktopList listbox with the 
	// available desktops
	// ---
	mDesktopList -> clear();
	QString topic;
	XData* sysData;
	QDict<char> desktopInfo;
	int desktops;
	desktops = mFiles["sys_DESKTOP"]->getDeviceCount();
	for (int n=0; n<=desktops; n++) {
		topic = "";
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
		QTextOStream (&topic)
			<< mText["desktop"] << " " << primary.ascii() 
			<< " " << mText["with"] << " " << defaultColor.ascii()
			<< " bit";
		mDesktopList -> insertItem (
			QPixmap(PIXDESKTOP),topic
		);
	}

	// ...
	// enable or disable the configure button
	// ---
	if (desktops > 0) {
		mSetup -> setDisabled (false);
		mDesktopList -> setCurrentItem ( 0 );
		mDesktopList -> setSelected    ( 0,true );
		mSelected = 0;
	} else {
		mSetup -> setDisabled (true);
	}
	}
	return (TRUE);
}

//=====================================
// XDesktop init function for toplevel
//-------------------------------------
void XDesktop::setupTop ( int color ) {
	// log (L_INFO,"XDesktop::setupTop() called\n");
	// ...
	// this function is called if the setup toplevel window
	// is created to configure the desktop
	// ---
	// get the mFiles pointer wrapper from the Intro
	// object which has read all the data files
	// ---
	QList<char>* resolution = NULL;
	int colorDepth;

	// ...
	// get the file pointer wrapper
	// ---
	QDict<XFile>* mFilePtr = mIntro->getFiles();
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);

	// ...
	// get the selected desktop pointer
	// ---
	XWrapPointer<XData> workingDesktop (
		mFiles["sys_DESKTOP"] -> getDevice (mSelected)
	);

	// ...
	// get the default color depth currently used 
	// for this desktop
	// ---
	if (color == -1) {
		QDict<char> desktopInfo = workingDesktop.getData();
		colorDepth = QString(desktopInfo["ColorDepth"]).toInt();
	} else {
		colorDepth = color;
	}

	// ...
	// init resolution lists for the current color
	// depths 4,8,15,16 and 24 bit
	// ---
	mCurrentColorDepth = colorDepth;
	QDict<char> desktopInfo = workingDesktop.getData();
	int col[5] = {4,8,15,16,24};
	for (int n=0;n<5;n++) {
		QList<char>* current = NULL;
		QString reskey; reskey.sprintf("Modes:%d",col[n]);
		XStringList res;
		if (mClipBoard[reskey]) {
			res.setText (mClipBoard[reskey]);
		} else {
			res.setText (desktopInfo[reskey]);
		}
		res.setSeperator (",");
		switch (col[n]) {
		case 4:
			m4ConfigRes  = res.getList();
			current = &m4ConfigRes;
		break;
		case 8:
			m8ConfigRes  = res.getList();
			current = &m8ConfigRes;
		break;
		case 15:
			m15ConfigRes = res.getList();
			current = &m15ConfigRes;
		break;
		case 16:
			m16ConfigRes = res.getList();
			current = &m16ConfigRes;
		break;
		case 24:
			m24ConfigRes = res.getList();
			current = &m24ConfigRes;
		break;
		}
		// ...
		// init default color combo box and according 
		// resolution list pointer
		// ---
		if (colorDepth == col[n]) {
			mColor -> setCurrentItem (n);
			resolution = current;
		}
	}

	// ...
	// set the name of the resolution tab
	// ---
	XWrapPointer< QDict<char> > mText (mTextPtr);
	QString translation = mText["resolutiontab"];
	translation.replace ( QRegExp("\%1"),mColor->currentText() );
	mTop->changeTab (layer7,translation);

	// ...
	// setup the currently used resolutions
	// according to the used color depth
	// ---
	if (resolution) {
	QDict<QCheckListItem> resItem;
	QListViewItemIterator it (mResolution);
	for ( ; it.current(); ++it ) {
		QCheckListItem* item = (QCheckListItem*)it.current();
		resItem.insert(it.current()->text(0),item);
		item -> setOn (false);
		mResolution -> setSelected (item,false);
	}
	QList<char> addRes;
	QListIterator<char> io (*resolution);
	for (; io.current(); ++io) {
		if (resItem[io.current()]) {
			resItem[io.current()]->setOn(true);
		} else {
			addRes.append (io.current());
		}
	}
	if (addRes.count() > 0) {
		QListIterator<char> io (addRes);
		for (; io.current(); ++io) {
			XStringList strRes (io.current());
			strRes.setSeperator ("x");
			QList<char> xy = strRes.getList();
			mNewX -> setValue (QString(xy.at(0)).toInt());
			mNewY -> setValue (QString(xy.at(1)).toInt());
			slotAdd();
		}
	}
	slotUsed (NULL);
	}
}

//=====================================
// XDesktop select one desktop...
//-------------------------------------
void XDesktop::slotSelect ( QListBoxItem* item ) {
	// log (L_INFO,"XDesktop::slotSelect() called\n");
	// ...
	// this function is called if you select a
	// desktop from the main desktop list
	// ---
	if (mDesktopList->isSelected (item)) {
		mSelected = mDesktopList -> currentItem();
	}
}

//=====================================
// XDesktop double click on desktop...
//-------------------------------------
void XDesktop::slotDoubleSelect ( QListBoxItem* item ) {
	// log (L_INFO,"XDesktop::slotSelect() called\n");
	// ...
	// this function is called if a double click will
	// select an item from the main desktop list
	// ---
	if (mDesktopList->isSelected (item)) {
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
// XDesktop configure selection...
//-------------------------------------
void XDesktop::slotSetup (void) {
	// log (L_INFO,"XDesktop::slotSetup() called\n");
	// ...
	// this function is called if you click onto the
	// configure button to setup the selected 
	// desktop
	// ---
	setupTop ();
	mTop -> show();
	mTop -> enterEvent ( 0 );
}

//=====================================
// XDesktop select color depths...
//-------------------------------------
void XDesktop::slotColor (int color) {
	// log (L_INFO,"XDesktop::slotColor() called\n");
	// ...
	// if we want to setup colors and resolutions for
	// the fbdev driver we need to update the resolution
	// and color list according to the framebuffer 
	// properties
	// ---
	QDict<XFile>* mFilePtr = mIntro->getFiles();
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);
	XWrapPointer<XData> workingCard (
		mFiles["sys_CARD"] -> getDevice (mSelected)
	);
	if (QString(workingCard["Driver"]) == "fbdev") {
	if ((color == 0) || (color == 2)) {
		XWrapPointer< QDict<char> > mText (mTextPtr);
		XBox mb (
			mText["dialoginfo"],
			mText["colorinvalid"],
			XBox::Critical,
			XBox::Ok,0,0,mFrame,
			globalFrameWidth
		);
		mb.setButtonText (
			XBox::Ok,mText["Ok"]
		);
		mb.exec();
		mTop -> enterEvent ( 0 );
		setupTop (mCurrentColorDepth);
		return;
	}
	}
	// ...
	// save the previously made resolution changes to
	// the clipboard which is evaluated in slotTopOk()
	// ---
	QList<char> handleList;
	switch (mCurrentColorDepth) {
	case 4:
		handleList = m4ConfigRes;
	break;
	case 8:
		handleList = m8ConfigRes;
	break;
	case 15:
		handleList = m15ConfigRes;
	break;
	case 16:
		handleList = m16ConfigRes;
	break;
	case 24:
		handleList = m24ConfigRes;
	break;
	}
	QString* modes = new QString();
	QString* key   = new QString();
	QListIterator<char> it (handleList);
	key -> sprintf("Modes:%d",mCurrentColorDepth);
	for (; it.current(); ++it) {
		modes->sprintf("%s,%s",modes->ascii(),it.current());
	}
	if (modes -> at(0) == ',') {
		modes -> remove (0,1);
	}
	if (! modes->isNull()) {
		mClipBoard.insert (key->ascii(),modes->ascii());
	}
   
	// ...
	// set the name of the resolution tab
	// ---
	XWrapPointer< QDict<char> > mText (mTextPtr);
	QString translation = mText["resolutiontab"];
	translation.replace ( QRegExp("\%1"),mColor->currentText() );
	mTop->changeTab (layer7,translation);
 
	// ...
	// evaluate default color according to the mColor
	// index value and call setupTop()
	// ---
	int defaultColor = 16;
	switch (color) {
	case 0:
		defaultColor = 4;
	break;
	case 1:
		defaultColor = 8;
	break;
	case 2:
		defaultColor = 15;
	break;
	case 3:
		defaultColor = 16;
	break;
	case 4:
		defaultColor = 24;
	break;
	}
	setupTop (
		defaultColor
	);
	mTop -> showPage (layer7);
}

//=====================================
// XDesktop select used resolution...
//-------------------------------------
void XDesktop::slotUsed (QListViewItem* resolution) {
	// log (L_INFO,"XDesktop::slotUsed() called\n");
	// ...
	// this function is called if you select an item
	// from the resolution listbox
	// ---
	// if we want to setup colors and resolutions for
	// the fbdev driver we need to update the resolution
	// list according to the framebuffer properties
	// ---
	if (resolution) {
	QDict<XFile>* mFilePtr = mIntro->getFiles();
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);
	XWrapPointer<XData> workingCard (
		mFiles["sys_CARD"] -> getDevice (mSelected)
	);
	XApiData fbresolution = mFiles["ext_MFBRESOLUTION"]->getAPI();
	mFBResPtr = fbresolution.getHash();
	if (QString(workingCard["Driver"]) == "fbdev") {
		QListViewItem* selectedItem = mResolution->currentItem();
		QDictIterator<char> it (*mFBResPtr);
		bool frameBufferResolution = false;
		for (; it.current(); ++it) {
		if (it.currentKey() == selectedItem->text(0)) {
			frameBufferResolution = true;
			break;
		}
		}
		if (! frameBufferResolution) {
			XWrapPointer< QDict<char> > mText (mTextPtr);
			XBox mb ( 
				mText["dialoginfo"],
				mText["resinvalid"],
				XBox::Critical,
				XBox::Ok,0,0,mFrame,
				globalFrameWidth
			);
			mb.setButtonText (
				XBox::Ok,mText["Ok"]
			);
			mb.exec();
			mTop -> enterEvent ( 0 );
			QCheckListItem* item = (QCheckListItem*)selectedItem;
			if (item->isOn()) {
				item->setOn (false);
				mResolution -> clearSelection();
			}
			return;
		}
    }
	}
	// ...
	// ok, now we are able to handle the available
	// resolutions...
	// ---
	bool oneChecked = false;
	bool selectionReached = false;
	QCheckListItem* selected = NULL;
	if (resolution) {
		selected = (QCheckListItem*)resolution;
	}
	QCheckListItem* standard = NULL;
	QListViewItem*  stditem  = NULL;
	QListViewItemIterator it (mResolution);
	for ( ; it.current(); ++it ) {
		QCheckListItem* item = (QCheckListItem*)it.current();
		#if 1
		// select all the resolutions behind the selected
		// one which are not selected.
		// ---
		if ((selected) && (! mFramebufferUsed)) {
		if (item == selected) {
			selectionReached = true;
		}
		if ((selected->isOn()) && (selectionReached) && (! item->isOn())) {
			item->setOn (true);
		}
		}
		#endif
		if (item->isOn()) {
		if (! oneChecked) {
			mInfo -> setText (it.current()->text(0));
			mResolution -> setSelected (it.current(),true);
			mResolution -> ensureItemVisible (it.current());
			oneChecked = true;
		}
		}
		if (item->text(0) == "640x480") {
			standard = item;
			stditem  = it.current();
		}
	}
	// ...
	// if no item is checked on, we will enable the
	// 640x480 standard item because one item is needed
	// to be checked on
	// ---
	if (! oneChecked) {
		standard -> setOn (true);
		mResolution -> setSelected (stditem,true);
		mResolution -> ensureItemVisible (stditem);
		mInfo -> setText (stditem->text(0));
	}

	// ...
	// setup the correct QList which save the resolution
	// information per color depth
	// ---
	QList<char> currentList;
	QListViewItemIterator io (mResolution);
	for ( ; io.current(); ++io ) {
	QCheckListItem* item = (QCheckListItem*)io.current();
	if (item->isOn()) {
		currentList.append (io.current()->text(0));
	}   
	}
	switch (mColor->currentItem()) {
	case 0:
		m4ConfigRes  = currentList;
	break;
	case 1:
		m8ConfigRes  = currentList;
	break;
	case 2:
		m15ConfigRes = currentList;
	break;
	case 3:
		m16ConfigRes = currentList;
	break;
	case 4:
		m24ConfigRes = currentList;
	break;
	}
}

//=====================================
// XDesktop add button clicked...
//-------------------------------------
void XDesktop::slotAdd (void) {
	// log (L_INFO,"XDesktop::slotAdd() called\n");
	// ...
	// this function is called if the add button
	// is clicked to incorp your own resolution
	// definition
	// ---
	QString newRes;
	newRes.sprintf (
		"%sx%s",mNewX->text().ascii(),mNewY->text().ascii()
	);
	QList<char> rtext;
	QList<char> state;
	QListViewItemIterator it (mResolution);
	for ( ; it.current(); ++it ) {
	QCheckListItem* item = (QCheckListItem*)it.current();
	QString* addRes = new QString (it.current()->text(0));
	rtext.append (addRes->ascii());
	if (item->isOn()) {
		state.append ("1");
	} else {
		state.append ("0");
	}
	}

	bool added = false;
	mResolution -> clear();
	int newCount = mNewX->value() * mNewY->value();
	for (int i=rtext.count()-1; i >= 0; i--) {
		XStringList strRes (rtext.at(i));
		strRes.setSeperator ("x");
		QList<char> xy = strRes.getList();
		int curCount = QString(xy.at(0)).toInt() * QString(xy.at(1)).toInt();
		// ...
		// include new resolution to correct position
		// ---
		if ((newCount < curCount) && (! added)) {
		added = true;
		QCheckListItem* item = new XCheckListItem (
			mResolution,newRes,QCheckListItem::CheckBox,mIntro
		);
		item -> setOn (true);
		}
		// ...
		// include standard resolutions existing before
		// cleaning the list and set the checked ones too
		// ---
		QCheckListItem* item = new XCheckListItem (
			mResolution,rtext.at(i),QCheckListItem::CheckBox,mIntro
		);
		if (QString(state.at(i)) == "1") {
			item -> setOn (true);
		}
	}
	if (! added) {
		QCheckListItem* item = new XCheckListItem (
			mResolution,newRes,QCheckListItem::CheckBox,mIntro
		);
		item -> setOn (true);
		mNewResAdded = true;
	}
	slotUsed (NULL);
	mAdd -> setDisabled (true);
	mTop -> showPage (layer7);
}

//=====================================
// XDesktop own res X Value changed...
//-------------------------------------
void XDesktop::slotXChanged (int x) {
	// log (L_INFO,"XDesktop::slotXChanged() called\n");
	mNewY -> setValue ( (int)(0.75 * x));
	if (addOk()) {
		mAdd->setDisabled (false);
	} else {
		mAdd->setDisabled (true);
	}
}

//=====================================
// XDesktop own res Y Value changed...
//-------------------------------------
void XDesktop::slotYChanged (int) {
	// log (L_INFO,"XDesktop::slotYChanged() called\n");
	if (addOk()) {
		mAdd->setDisabled (false);
	} else {
		mAdd->setDisabled (true);
	}
}

//=====================================
// XDesktop check new resolution...
//-------------------------------------
bool XDesktop::addOk (void) {
	// log (L_INFO,"XDesktop::addOk() called\n");
	// ...
	// this function will check if the resolution
	// build via mNewX and mNewY is valid to be
	// added into the current resolution listview
	// ---
	if (QString(mNewX->text()).toInt() < 320) {
	return (false);
	}
	if (QString(mNewY->text()).toInt() < 200) {
	return (false);
	}
	QString newRes;
	newRes.sprintf (
		"%sx%s",mNewX->text().ascii(),mNewY->text().ascii()
	);
	QListViewItemIterator it (mResolution);
	for ( ; it.current(); ++it ) {
	if (it.current()->text(0) == newRes) {
		return (false);
	}
	}
	return (true);
}

//=====================================
// XDesktop slotTopOk...
//-------------------------------------
void XDesktop::slotTopOk (void) {
	// log (L_INFO,"XDesktop::slotTopOk() called\n");
	// ...
	// this function is called if you click onto the OK 
	// button in the setup toplevel window
	// ---
	// get the mFiles pointer wrapper from the Intro
	// object which has read all the data files
	// ---
	mFrame -> enterEvent ( 0 );
	XWrapPointer< QDict<char> > mText (mTextPtr);
	QDict<XFile>* mFilePtr = mIntro->getFiles();
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);

	// ...
	// get the selected desktop pointer
	// ---
	XWrapPointer<XData> workingDesktop (
		mFiles["sys_DESKTOP"] -> getDevice (mSelected)
	);

	// ...
	// update the default color depth
	// ---
	QString* defaultColor = new QString();
	switch (mColor->currentItem()) {
	case 0:
		defaultColor -> sprintf("4");
	break;
	case 1:
		defaultColor -> sprintf("8");
	break;
	case 2:
		defaultColor -> sprintf("15");
	break;
	case 3:
		defaultColor -> sprintf("16");
	break;
	case 4:
		defaultColor -> sprintf("24");
	break;
	}
	workingDesktop.setPair (
		"ColorDepth",defaultColor->ascii()
	);

	// ...
	// update the Modes entries per color depth
	// ---
	QList<char> defaultRes;
	int col[5] = {4,8,15,16,24};
	for (int i=0;i<5;i++) {
		QList<char> handleList;
		switch (col[i]) {
		case 4:
			handleList = m4ConfigRes;
		break;
		case 8:
			handleList = m8ConfigRes;
		break;
		case 15:
			handleList = m15ConfigRes;
		break;
		case 16:
			handleList = m16ConfigRes;
		break;
		case 24:
			handleList = m24ConfigRes;
		break;
		}
		if (col[i] == defaultColor->toInt()) {
			defaultRes = handleList;
		}

		QString* modes = new QString();
		QString* key   = new QString();
		QListIterator<char> it (handleList);
		key -> sprintf("Modes:%d",col[i]);
		for (; it.current(); ++it) {
			modes->sprintf("%s,%s",modes->ascii(),it.current());
		}
		if (modes -> at(0) == ',') {
			modes -> remove (0,1);
		}
		if (! modes->isNull()) {
		workingDesktop.setPair (
			key->ascii(),modes->ascii()
		);
		}
	}

	// ...
	// have a look at the clipboard and set the
	// Modes resolution information too
	// ---
	QDictIterator<char> it (mClipBoard);
	for (; it.current(); ++it) {
	workingDesktop.setPair (
		it.currentKey().ascii(),it.current()
	);
	}
	mClipBoard.clear();

	// ...
	// check the startup resolution with the currently
	// used frequency setup. If the modeline calculated does
	// not fit the hsync range show a warning
	// ----
	QString optx ("-x");
	QString opty ("-y");
	QString optr ("-r");
	XStringList sync;
	sync.setText (workingDesktop["VertRefresh"]);
	sync.setSeperator ("-");
	QString maxVsync = sync.getList().at(1);
	QString minVsync = sync.getList().at(0);
	sync.setText (workingDesktop["HorizSync"]);
	QString maxHsync = sync.getList().at(1);
	XStringList res;
	res.setText (defaultRes.at(0));
	res.setSeperator ("x");
	QList<char> resList = res.getList();
	QString vsync;
	bool fitSyncRange = false;
	for (int i=minVsync.toInt();i<=maxVsync.toInt();i+=2) {
		vsync.sprintf("%d",i);
		QString modeline = qx (
			XMODE,STDOUT,6,"%s %s %s %s %s %s",
			optx.ascii(),resList.at(0),opty.ascii(),resList.at(1),
			optr.ascii(),vsync.ascii()
		);
		XStringList mode;
		mode.setText (modeline);
		mode.setSeperator ("\n");
		QString mhsync = mode.getList().at(0);
		QString mvsync = mode.getList().at(1);
		log ( L_INFO,
			"XDesktop::checking: have[ %d khz %d hz ] / got[ %d Khz %d hz ]\n",
			maxHsync.toInt(),minVsync.toInt(),mhsync.toInt(),mvsync.toInt()
		);
		if (
			(mhsync.toInt() <= maxHsync.toInt()) && 
			(mvsync.toInt() >= minVsync.toInt())
		) {
			fitSyncRange = true;
			log ( L_INFO, "XDesktop::fit sync range: %s Khz %s Hz\n",
				mhsync.ascii(),mvsync.ascii()
			);
			break;
		}
	}
	if (! fitSyncRange) {
		setMessage ("invalidSyncRangeForResolution",
			XBox::Warning, defaultRes.at(0)
		);
	}

	// ...
	// update the main selection string within the
	// mDesktopList listbox
	// ---
	QString topic;
	QTextOStream (&topic)
		<< mText["desktop"] << " " << defaultRes.at(0) 
		<< " " << mText["with"] << " " << defaultColor->ascii() 
		<< " bit";
	mDesktopList -> changeItem (
		QPixmap(PIXDESKTOP),topic,mSelected
	);
}

//=====================================
// XDesktop slotTopCancel...
//-------------------------------------
void XDesktop::slotTopCancel (void) {
	// log (L_INFO,"XDesktop::slotTopCancel() called\n");
	// ...
	// this function is called if you click onto the Cancel 
	// button in the setup toplevel window
	// ---
	// Reset list entries which hold the resolution(s)
	// used for each available color depth
	// ---
	mFrame -> enterEvent ( 0 );
	mClipBoard.clear();
}

//=====================================
// XDesktop write lilocode.ycp...
//-------------------------------------
void XDesktop::writeLILO (int code, const QString& resolution) {
	QFile* mHandle = new QFile (LILOCODE);
	if (! mHandle -> open(IO_WriteOnly)) {
		log (L_ERROR,
			"XDesktop::writeLILO() could not create LILO file: %s -> %s\n",
			LILOCODE,strerror(errno)
		);
		return;
	}
	QString line;
	line.sprintf ("[\n  \"%d\",\"%s\"\n]",code,resolution.ascii());
	mHandle -> writeBlock (
		line.ascii(),line.length()
	);
	mHandle -> close();
}

//=====================================
// XDesktop set OK message box...
//-------------------------------------
void XDesktop::setMessage (
	const QString& textKey,XBox::Icon icon,const QString& addon
) {
	XWrapPointer< QDict<char> > mText (mTextPtr);
	QString text;
	QTextOStream (&text)
		<< mText[textKey] << " " << addon;
	XBox mb (
		mText["hint"],text,
		icon,XBox::Ok, 0,0,mFrame,
		globalFrameWidth
	);
	mb.setButtonText (
		XBox::Ok, mText["Ok"]
	);
	mb.exec();
	if (mTop->isVisible()) {
		mTop -> enterEvent ( 0 );
	} else {
		mFrame -> enterEvent ( 0 );
	}
}

