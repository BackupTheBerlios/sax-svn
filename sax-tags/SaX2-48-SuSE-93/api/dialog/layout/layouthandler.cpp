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
              : configure the server layout if a multihead
              : environment is available
              :
              : --> signal action slots/callbacks
              :
STATUS        : Status: Up-to-date
**************/
#include "layout.h"

//=====================================
// external globals
//-------------------------------------
extern int globalFrameWidth;

//=====================================
// XLayout reset and switch to Intro
//-------------------------------------
void XLayout::resetPage (int reload) {
	// ...
	// this function is called if the layout dialog is finished or canceled
	// The changes are serialized and re-imported if the user want
	// to conclude the dialog. Otherwhise the original file is imported
	// and the changes will be lost.
	// ---
	QString update = "sys_LAYOUT";
	// ...
	// set the current layout
	// ---
	if (reload == PAGE_RELOAD) {
		updateServerLayout();
	}
	// ...
	// serialize data if required and re-import
	// the data as changed or original stuff
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
// XLayout update server layout...
//-------------------------------------
void XLayout::updateServerLayout (void) {
	// ...
	// this function is manually called if the server
	// layout record should be updated. Update means in this
	// case the old record is deleted and a new one is created
	// based on the old data and the new data which has changed
	// ---
	// get the file pointer wrapper and the according
	// server layout record
	// ---
	QDict<XFile>* mFilePtr = mIntro->getFiles();
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);
	XWrapPointer<XData> serverLayout (
		mFiles["sys_LAYOUT"] -> getDevice(0)
	);
	if (! &serverLayout) {
		log (L_ERROR,
			"XLayout::could not load layout record\n"
		);
		return;
	}
	// ...
	// save the current Xinerama and Clone settings
	// used for recreating the server layout record
	// ---
	bool isXinerama = false;
	bool isClone    = false;
	if (QString(serverLayout["Xinerama"]) == "on") {
		isXinerama = true;
	}
	if (QString(serverLayout["Clone"]) == "on") {
		isClone = true;
	}
	// ...
	// clear the current layout settings and rebuild
	// the section from scratch, start with the
	// Screen entries
	// ---
	serverLayout.clear();
	QList<XLayoutLine> layout = mMatrix->getLayout();
	QListIterator<XLayoutLine> it (layout);
	for (; it.current(); ++it) {
		QString* key   = new QString();
		QString* value = new QString();
		XLayoutLine* l = it.current();
		int neighbour[4] = {l->mLeft,l->mRight,l->mTop,l->mBottom};
		key->sprintf("Screen:Screen[%d]",l->ID);
		for (int n=0;n<4;n++) {
		if (neighbour[n] == -1) {
			value->sprintf (
				"%s <none>",value->ascii()
			);
		} else {
			value->sprintf (
				"%s Screen[%d]",value->ascii(),neighbour[n]
			);
		}
		}
		*value = value->stripWhiteSpace();
		serverLayout.setPair (
			key->ascii(),value->ascii()
		);
	}
	// ...
	// set static layout elements
	// ---
	serverLayout.setPair ("Identifier","Layout[all]");
	serverLayout.setPair ("Keyboard","Keyboard[0]");
	serverLayout.setPair ("Clone","off");
	serverLayout.setPair ("Xinerama","off");
	// ...
	// search for the input devices and create the 
	// InputDevice string
	// ---
	int count = -1;
	QString* input = new QString();
	for (int n=0; n<mFiles["sys_INPUT"]->getDeviceCount(); n++) {
		count += 2;
		input->sprintf("%s,Mouse[%d]",input->ascii(),count);
	}
	input->replace(QRegExp("^,"),"");
	serverLayout.setPair ("InputDevice",input->ascii());
	// ...
	// last set Clone/Xinerama flags
	// ---
	if (isClone) {
		serverLayout.setPair ("Clone","on");
		serverLayout.setPair ("Xinerama","on");
	}
	if (isXinerama) {
		serverLayout.setPair ("Xinerama","on");
	}
}

//=====================================
// XLayout internal page called...
//-------------------------------------
void XLayout::pageCalled (int index) {
	// ...
	// this function is called if the next page of the dialog
	// is called. Note if the dialog has only one page this function
	// will not be called because there is no next page in this case
	// ---
	switch (index) {
	case LayoutPage1: {
		break;
	}
	};
}

//=====================================
// XLayout init page with data infos 
//-------------------------------------
void XLayout::initPage (void) {
	// log (L_INFO,"XLayout::initPage() called\n");
	// ...
	// this function is called after the page was created.
	// It must only be used to init the widgets contents
	// with the data available for selections and other stuff
	// like that
	// ---
}  

//=====================================
// XLayout virtual slots...
//-------------------------------------
bool XLayout::slotRun (int index) {
	// ...
	// this function is called if the layout page is activated.
	// use this function to init the dialog with the current
	// setup of the server layout
	// ---
	XWrapPointer< QDict<char> > mText (mTextPtr);
	if (index == Layout) {
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
	// log(L_INFO,"XLayout::slotRun() called: %d\n",index);
	// ...
	// add widgets to layout matrix according to
	// the currently used serverLayout
	// ---
	setupLayout();

	// ...
	// update status widget and prepare 
	// finish button
	// ---
	mStatus -> message (mText["RunXLayout"]);
	mFrame  -> nextButton() -> setText (mText["finish"]);
	}
	return (TRUE);
}

//=====================================
// XLayout setup layout structure...
//-------------------------------------
void XLayout::setupLayout (void) {
	// ...
	// this function will create the currently used
	// serverlayout and build a plan of the layout
	// to the matrix
	// ---
	// create text pointer wrapper object and
	// clear the matrix field
	// ---
	XWrapPointer< QDict<char> > mText (mTextPtr);
	mMatrix -> clear();

	// ...
	// get the mFiles pointer wrapper and read the data
	// record holding the server layout specifications
	// There is only one server layout section
	// ---
	QDict<XFile>* mFilePtr = mIntro->getFiles();
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);
	XWrapPointer<XData> sysData (
		mFiles["sys_LAYOUT"] -> getDevice(0)
	);
	if (! &sysData) {
		log (L_WARN,
		"XLayout::could not get layout record\n"
		);
		return;
	}
	int cards = mFiles["sys_CARD"] -> getDeviceCount();
	for (int count=0;count < cards; count++) {
	QString key;
	key.sprintf ("Screen:Screen[%d]",count);
	if ( sysData[key] ) {
		XStringList relation ( sysData[key] );
		QList<char> neighbours = relation.getList();
		QListIterator<char> at (neighbours);
		int indexList [4];
		int index = 0;
		for (; at.current(); ++at) {
			QString item (at.current());
			if (item.contains ("none")) {
				indexList[ index ] = -1; index++;
				continue;
			}
			item.replace (QRegExp("Screen\\["),"");
			item.replace (QRegExp("\\]"),"");
			indexList[ index ] = item.toInt();
			index++;
		}
		XData* cardPointer = mFiles["sys_CARD"] -> getDevice(count);
		XData* crtdPointer = mFiles["sys_DESKTOP"] -> getDevice(count);
		if ((! cardPointer) || (! crtdPointer)) {
			log (L_WARN,
			"XLayout::could not get CARD/DESKTOP record for screen %d\n",count
			);
			return;
		}
		QDict<char> card = cardPointer -> getData();
		QDict<char> monitor = crtdPointer -> getData();
		QString monitorInfo;
		if (QString(monitor["VendorName"]) == monitor["ModelName"]) {
			monitorInfo = monitor["VendorName"];
		} else {
			monitorInfo = monitor["VendorName"];
			monitorInfo += " ";
			monitorInfo += monitor["ModelName"];
		}
		QString toolTip;
		QTextOStream (&toolTip)
			<< "<nobr><b>" << monitorInfo << "</b> " << mText["connectedto"] 
			<< " <u>" << card["Vendor"] << " " << card["Name"] 
			<< "</u></nobr>";
		mMatrix->setWidget(
			count,indexList[0],indexList[1],indexList[2],indexList[3],
			toolTip
		);
	}
	}
	mMatrix->setLayout();
}

//=====================================
// XLayout got press event on figure...
//-------------------------------------
void XLayout::gotFigure (int id) {
	// ...
	// every time the user clickes on one of the
	// figures building the layout, we will show a message
	// on the screen which belongs to the figure clicked
	// ---
	QString optd ("-d");
	QString optr ("-R");
	QString* text = new QString;
	QTextOStream (text) << id << ", ,Screen,(," << id << ",)";
	qx (XQUERY,STDNONE,4,"%s %s %s %s",
		optd.ascii(),DisplayString(x11Display()),optr.ascii(),text->ascii()
	);
}

//=====================================
// XLayout return matrix pointer...
//-------------------------------------
XPlot* XLayout::getMatrix (void) {
	return (mMatrix);
}
