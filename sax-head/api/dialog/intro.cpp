/**************
FILE          : intro.c
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the GPL license
              :
DESCRIPTION   : xapi (X11-Application-Interface)
			  : intro.c is the introduction dialog for SaX2
              :
STATUS        : Status: Up-to-date
**************/
#include <qapplication.h>
#include <qwidget.h>
#include <qgroupbox.h>
#include <qbuttongroup.h>
#include <qmessagebox.h>
#include <qtextview.h>
#include <qtextbrowser.h>
#include <qfileinfo.h>
#include <qlayout.h>
#include <qsplitter.h>
#include <qlistview.h>
#include <qwhatsthis.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

#include "intro.h"
#include "../common/xlock.h"
#include "../common/qx.h"
#include "../xapi.h"

//=====================================
// external globals
//-------------------------------------
extern int globalFrameWidth;

//=====================================
// XStringList Constructor...
//-------------------------------------
XStringList::XStringList (const QString& text) {
	mSeperator = new QString (" ");
	mText      = new QString (text);
}

//=====================================
// XStringList setSeperator...
//-------------------------------------
void XStringList::setSeperator (const QString& sep) {
	mSeperator->sprintf("%s",sep.ascii());
}

//=====================================
// XStringList getList...
//-------------------------------------
QList<char> XStringList::getList (void) {
	if (! mText->isEmpty()) { 
	char* item = NULL; 
	char* itemlist = (char*) malloc (mText->length() + 1);
	sprintf (itemlist,"%s",mText->ascii());
	item = strtok (itemlist,mSeperator->ascii()); 
	while (1) {
		if (! item) { 
		break; 
		}
		QString* insert = new QString ( item );
		mList.append ( *insert );
		item = strtok (NULL,mSeperator->ascii());
	}
	}
	return (mList);
}

//=====================================
// XStringList setText...
//-------------------------------------
void XStringList::setText (const QString& text) {
	mText->sprintf("%s",text.ascii());
	mList.clear();
}

//=====================================
// XStringList clear...
//-------------------------------------
void XStringList::clear (void) {
	mList.clear();
}

//=====================================
// XQuery constructor...
//-------------------------------------
XQuery::XQuery (const QString& qresult, bool truncate) {
	QString* result;
	char* data = NULL;
	char* line = NULL;
	mScreen    = 1;

	data = (char*) malloc (qresult.length() + 1);
	sprintf (data,"%s",qresult.ascii());

	line = strtok (data,"\n");
	if (line) {
	result = new QString (line);
	if (truncate) {
		*result = result->right(result->length() - 2);
	}
	mValue.append (*result);
	while (1) {
		line = strtok (NULL,"\n");
		result = new QString (line);
		if (truncate) {
		*result = result->right(result->length() - 2);
		}
		if (! line) {
		break;
		}
		mValue.append (*result);
		mScreen++;
	}
	} 
}

//=====================================
// XQuery get value for card X...
//-------------------------------------
QString XQuery::getValue (int card) {
	QString result (mValue.at(card));
	return (result);
}

//=====================================
// XQuery get screen count...
//-------------------------------------
int XQuery::getScreen (void) {
	return (mScreen);
}

//=====================================
// XIntro Constructor...
//-------------------------------------
XIntro::XIntro (void) {
	mIndex = Intro;
}

//=====================================
// XIntro Constructor...
//-------------------------------------
XIntro::XIntro (XFrame* xf) : XTemplate (xf,this) {
	mIndex = Intro;
	mFrame = xf;
	addTo(xf);
}

//=====================================
// XIntro save a widget...
//-------------------------------------
void XIntro::store (void* widget,int index) {
	mStoreWidget.insert(index,widget);
}

//=====================================
// XIntro retrieve widget pointer...
//-------------------------------------
void* XIntro::retrieve (int index) {
	return (mStoreWidget[index]);
}

//=====================================
// XIntro add widget to mainframe...
//-------------------------------------
void XIntro::addTo (XFrame* xf) {
	mFrame = xf;

	// get connected to main frame....
    // ---------------------------------	
	getConnected(xf);

	// Read all the needed data files...
	// -----------------------------------
	QDict<char> init = xf->getInit();
	QDict<char> text = xf->getText();
	QDictIterator<char> it (init);

	XCommand sysCard       ( GETCARD       , ISAXCARD       );
	XCommand sysDesktop    ( GETDESKTOP    , ISAXDESKTOP    );
	XCommand sysLayout     ( GETLAYOUT     , ISAXLAYOUT     );
	XCommand sysKeyboard   ( GETKBD        , ISAXKEYBOARD   );
	XCommand sysPath       ( GETPATH       , ISAXPATH       );
	XCommand sysInput      ( GETINPUT      , ISAXINPUT      );
	XCommand sysExtensions ( GETEXTENSIONS , ISAXEXTENSIONS );

	QFileInfo configStatus (XCONFIG);
	QFileInfo scanStatus   (HWCONFIG);

	// ...
	// if option -u is passed to xapi this will indicate
	// that SaX2 has started its own X-Server based on the
	// hw data scanned from SaX2/hwinfo. In this case we
	// need to pass the option -b to the isax call to be
	// sure the hw information is used instead of reading
	// the information from the existing configuration 
	// file... 
	// --------
	if (xf -> useHwData) {
		sysCard.addOption		("-b");
		sysDesktop.addOption	("-b");
		sysLayout.addOption		("-b");
		sysKeyboard.addOption	("-b");
		sysPath.addOption		("-b");
		sysInput.addOption		("-b");
		sysExtensions.addOption ("-b");
	}
	// ...
	// Ok let`s call the ISaX to create the configuration
	// information database
	// ----------------------
	if (! xf->fastSetup) {
		sysCard.runCommand();
		sysDesktop.runCommand();
		sysLayout.runCommand();
		sysKeyboard.runCommand();
		sysPath.runCommand();
		sysInput.runCommand();
		sysExtensions.runCommand();
	}

	for (; it.current(); ++it) {
	QString key = it.currentKey();
	QString kid = key.left(3);
	if (kid == "cdb") {
		XFile *import = new XFile(it.current(),key);
		import->enableProgressDialog(xf->getProgressDialog(),xf);
		import->cdbRead();
		mFiles.insert(key,import);
	} 
	if ((kid == "std") || (kid == "ext")) {
		XFile *import = new XFile(it.current(),key);
		import->apiRead();
		mFiles.insert(key,import);
	}
	if (kid == "sys") {
		QFileInfo sysFile (it.current());
		if (! sysFile.exists()) {
			sysCard.runCommand();
			sysDesktop.runCommand();
			sysLayout.runCommand();
			sysKeyboard.runCommand();
			sysPath.runCommand();
			sysInput.runCommand();
			sysExtensions.runCommand();
		}
		while(getproc(it.current())) {
			XStringList baseList (it.current());
			baseList.setSeperator ("/");
			QList<char> baseName = baseList.getList();
			QString* locked = new QString();
			QTextOStream (locked)
				<< QString::fromLocal8Bit(text["file"]) 
				<< ": " << baseName.getLast() << " "
				<< QString::fromLocal8Bit(text["lock"]) 
				<< " " << QString::fromLocal8Bit(text["wait"]) << "...";
			xf->getProgressDialog()->setLabelText (*locked);
			qApp->processEvents();
			usleep(500);
		}
		XFile *import = new XFile(it.current(),key);
		import->enableProgressDialog(xf->getProgressDialog(),xf);
		import->sysRead();
		mFiles.insert(key,import);
	}
	}
	xf->getProgressDialog()->cancel();

	// start the dialog builder function....
	// ---------------------------------------
	dialogCreate();
}

//=====================================
// XIntro dialog building member...
//-------------------------------------
void XIntro::dialogCreate (void) {
	// Show a introduction page about the current
	// configuration status...
	// -------------------------
	QDict<char> text = mFrame->getText();
	QDict<char> init = mFrame->getInit();

	// Get X11 Version...
	// -------------------
	QString msg  = QString::fromLocal8Bit(text["intro_header"]);
	QString xver = qx(XVERSION,STDOUT);
	XQuery qVer (xver,FALSE);
	msg += qVer.getValue(0).ascii();
	msg += "<br><br>";
	mHeaderText = new QString (msg);

	// Create widgets for this dialog
	// using a frame with a VBox Layout in it
	// ----------------------------------------
	mDialog = new QFrame ( mStack );
	QBrush*     paper  = new QBrush;
	QBoxLayout* layout = new QVBoxLayout ( mDialog );

	mTextView = new XTextBrowser ( mDialog );
	paper   -> setPixmap( QPixmap( init["paper"] ) );
	mTextView -> setHScrollBarMode (QScrollView::AlwaysOff);
	mTextView -> setPaper(*paper);
	mTextView -> setText(msg);

	// connect mime factory
	// --------------------
	QObject::connect (
		mTextView, SIGNAL (linkSelected (const QString&) ),
		this     , SLOT   (slotMimeType (const QString&) )
	);

	// add widgets to the layout and stack...
	// ----------------------------------------
	layout->addWidget(mTextView);
	mStack->addWidget(mDialog,mIndex);

	// this special dialog is started manually now
	// --------------------------------------------
	mFrame->buttonBar2()  -> hide();
	mFrame->startButton() -> setDisabled (TRUE);
	slotRun (mIndex);
}

//=====================================
// XIntro import sys file...
//-------------------------------------
void XIntro::importFile (QString & filekey) {
	QString kid  = filekey.left (3);

	if (kid == "sys") {
	QDict<char> init = mFrame -> getInit();
    QDict<char> text = mFrame -> getText();
	
	QString file = init [filekey.ascii()];
	while(getproc((char*)file.ascii())) {
		QString locked;
		locked.sprintf( "%s %s %s %s...",
			text["file"],file.ascii(),
			text["lock"],text["wait"]
		);
		mFrame->getProgressDialog()->setLabelText(locked);
		qApp->processEvents();
		usleep(500);
	}
	XFile *import = new XFile (file.ascii(),filekey);
	import -> enableProgressDialog(
		mFrame->getProgressDialog(),mFrame
	);
	import -> sysRead();
	if (mFiles.find (filekey)) {
	mFiles.replace (
		filekey,import
	);
	} else {
	mFiles.insert (
		filekey,import
	);
	}
	mFrame->getProgressDialog()->cancel();
	}
}

//=====================================
// XIntro get files dict...
//-------------------------------------
QDict<XFile>* XIntro::getFiles (void) {
	return(&mFiles);
}

//=====================================
// XIntro get text view pointer...
//-------------------------------------
XTextBrowser* XIntro::getTextView (void) {
	return (mTextView);
}

//=====================================
// XIntro get header text pointer...
//-------------------------------------
QString* XIntro::getHeaderText (void) {
	return(mHeaderText);
}

//=====================================
// XIntro are we active...
//-------------------------------------
bool XIntro::isVisible (void) {
	if (mStack->visibleWidget() == mDialog) {
		return(TRUE);
	}
	return (FALSE);
}

//=====================================
// XFrame check for unknown hardware
//-------------------------------------
void XIntro::checkDetected (void) {
	// ...
	// this function is always called if a configuration is
	// finished and exactly one time at program startup
	// we will check if the basic hardware was detected
	// this includes the following items:
	// ---
	// Monitor and Card
	// ---
	// log (L_INFO,"XIntro::checkDetected()...\n");
	// ---
	XData* data = NULL;
	XWrapPointer< QDict<char> > mText (
		mTextPtr
	);

	//=================================== 
	// Monitor data check
	//----------------------------------- 
	for (int n=0; n<mFiles["sys_DESKTOP"]->getDeviceCount(); n++) {
	data = mFiles["sys_DESKTOP"] -> getDevice(n);
	if (! data) {
		continue;
	}
	if (n > 0) {
		continue;
	}
	QString cardNr;
	cardNr.sprintf("%d",n);
	QDict<char> desktop = data -> getData();
	QString model (desktop["VendorName"]);
	QString optn;
	optn.sprintf("%d",n);
	QString ddcID = qx(GETDDC,STDOUT,1,"%s",optn.ascii());
	if ((model=="DDC-Probed") || (model=="Unknown")) {
		QString translation;
		if (ddcID == "undefined") {
			translation = mText["monitorunknown"];
			translation.replace ( QRegExp("\%1"),cardNr );
		} else {
			translation = mText["monitorCDBunknown"];
			translation.replace ( QRegExp("\%2"),ddcID );
			translation.replace ( QRegExp("\%1"),cardNr );
		}
		XBox mb (
			mText["monitorinfo"],
			translation,
			XBox::Warning,
			XBox::Yes,
			XBox::No, 0,mFrame,
			globalFrameWidth
		);
		mb.setButtonText (
			XBox::Yes,mText["Yes"]
		);
		mb.setButtonText (
			XBox::No, mText["No"]
		);
		switch (mb.exec()) {
		case XBox::Yes: {
			mFrame -> runDialog (Monitor);
		}
		}
		break;
	} else
	if ((model=="--> VESA") && (ddcID != "undefined")) {
		QString translation;
		translation = mText["monitorCDBunknown"];
		translation.replace ( QRegExp("\%2"),ddcID );
		translation.replace ( QRegExp("\%1"),cardNr );
		XBox mb (
			mText["monitorinfo"],
			translation,
			XBox::Warning,
			XBox::Yes,
			XBox::No, 0,mFrame,
			globalFrameWidth
		);
		mb.setButtonText (
			XBox::Yes,mText["Yes"]
		);
		mb.setButtonText (
			XBox::No, mText["No"]
		);
		switch (mb.exec()) {
		case XBox::Yes: {
			mFrame -> runDialog (Monitor);
		}
		}
		break;
	}
	}

	//===================================
	// Card data check
	//-----------------------------------
	bool multiCard = false;
	for (int n=0; n<=mFiles["sys_CARD"]->getDeviceCount(); n++) {
	data = mFiles["sys_CARD"] -> getDevice(n);
	if (! data) {
		continue;
	}
	QDict<char> card = data -> getData();
	QString model (card["Vendor"]);
	if (model == "unknown") {
		XBox mb (
			mText["cardinfo"],
			mText["cardunknown"],
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
		case XBox::Yes: {
			mFrame -> runDialog (Card);
		}
		}
		mFrame -> enterEvent ( 0 );
		break;
	}
	if (n >= 1) {
		multiCard = true;
	}
	}

	//======================================
	// more than one card; disable randr
	//--------------------------------------
	#if 0
	if (multiCard) {
		QString update;
		log(L_INFO,"disable RandR Extension in multihead mode\n");
		XData* systemServerFlags = mFiles["sys_PATH"]->getDevice (0);
		systemServerFlags -> setPair (
			"SpecialFlags","RandR,off"
		);
		update = "sys_PATH";
		if (mFiles [update] -> sysSerialize()) {
			mFiles [update] -> isModified ( mFrame );
		}
		importFile (update);
	}
	#endif
}

//=====================================
// XIntro virtual slots...
//-------------------------------------
void XIntro::slotNext (void) {
	if (isVisible()) {
	log(L_INFO,"got next...\n");
	}
}

void XIntro::slotBack (void) {
	if (isVisible()) {
	log(L_INFO,"got back...\n");
	}
}

bool XIntro::slotRun (int index) {
	if (index == mIndex) {
	log(L_INFO,"got run... %d -> Intro Page\n",index);
	mStack->raiseWidget(mIndex);
	}
	return (TRUE);
}

//=====================================
// XIntro dialog intro for main branch
//-------------------------------------
void XIntro::slotIntro (int index) {
	if (index == mIndex) {
		XWrapPointer< QDict<char> > mText (
			mTextPtr
		);
		// ...
		// get the configuration components available
		// within this major configuration topic
		// ---
		QString* header = new QString();
		QTextOStream headerStream (header);
		QListViewItem* current = NULL;
		QListView* mView = mFrame->getComponentView();
		current = mView -> currentItem();
		if (current) {
		headerStream
			<< *mHeaderText << mText["subdialog"] << " <b><u>" 
			<< current->text(0) << "<br></u></b>" << mText["subdialogheader"];
		current = current->firstChild();
		headerStream << "<ul>";
		while (current) {
			headerStream << "<li><a href=\"" << current->text(0) <<
			"\">" << current->text(0)  << "</a></li>";
			current = current->nextSibling();
		}
		}
		headerStream << "</ul>";
		// ...
		// set header text, with configuration
		// information
		// ---
		mTextView -> setText (*header);
	}
}

//=====================================
// XIntro link callback function
//-------------------------------------
void XIntro::slotMimeType ( const QString& name  ) {
	QListViewItem* selected = NULL;
	QListView* mView = mFrame->getComponentView();
	selected = mView -> currentItem();
	if (selected) {
	selected = selected->firstChild();
	while (selected) {
		if (selected ->text(0) == name) {
			mView -> setSelected (selected, true);
			mFrame -> runDialog (selected);
			return;
		}
		selected = selected -> nextSibling();
	}
	}
}
