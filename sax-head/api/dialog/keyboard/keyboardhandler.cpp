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
              : configure the primary keyboard properties and
              : handle the font settings here too
              :
              : --> signal action slots/callbacks
              :
STATUS        : Status: Up-to-date
**************/
#include "keyboard.h"
#include <X11/XKBlib.h>

//=====================================
// XKeyboard reset and switch to Intro
//-------------------------------------
void XKeyboard::resetPage (int reload) {
	// ...
	// this function is called if the keyboard dialog is finished or canceled
	// The changes are serialized and re-imported if the user want
	// to conclude the dialog. Otherwhise the original file is imported
	// and the changes will be lost.
	// ---
	QString update = "sys_KEYBOARD";

	//=======================================
	// save primary and secondary keyboard(s)
	//---------------------------------------
	if (reload == PAGE_RELOAD) {
		QString* XkbLayout  = NULL;
		QString* XkbModel   = NULL;
		QString* XkbVariant = NULL;
		XWrapFile < QDict<XFile> > mFiles (mFilePtr);
		XWrapPointer<XData> workingKeyboard (
			mFiles["sys_KEYBOARD"]->getDevice (0)
		);
		// 1) primary XKB model
		QDictIterator<char> itModel (mModelHash);
		for (; itModel.current(); ++itModel) {
		if (QString::fromLocal8Bit(itModel.current()) == mType->currentText()) {
			XkbModel = new QString (itModel.currentKey());
		}
		}
		// 2) primary XKB layout
		QDictIterator<char> itLayout (mLayoutHash);
		for (; itLayout.current(); ++itLayout) {
		if (
			QString::fromLocal8Bit(itLayout.current()) == mLayout->currentText()
		) {
			XkbLayout = new QString (itLayout.currentKey());
		}
		}
		// 3) variant for primary XKB layout
		XkbVariant = new QString();
		if ((mVariant->currentText()) && (mVariant->currentText() != "basic")) {
			*XkbVariant = mVariant->currentText();
		}
		// 4) additional layout and variant
		QListViewItemIterator itAdd (mAddView);
		for ( ; itAdd.current(); ++itAdd ) {
			QCheckListItem* item = (QCheckListItem*)itAdd.current();
			if (item->isOn()) {
				QString layout  = itAdd.current()->text(2);
				QString variant = "!";
				XkbLayout->sprintf("%s,%s",
					XkbLayout->ascii(),layout.ascii()
				);
				if (itAdd.current()->text(3)) {
					variant = itAdd.current()->text(3);
				}
				XkbVariant->sprintf("%s,%s",
					XkbVariant->ascii(),variant.ascii()
				);
			}
		}
		if (XkbLayout) {
		workingKeyboard.setPair ("XkbLayout",
			XkbLayout->ascii()
		);
		}
		if (XkbModel) {
		workingKeyboard.setPair ("XkbModel",
			XkbModel->ascii()
		);
		}
		if ((XkbVariant) && (! XkbVariant->isEmpty())) {
		workingKeyboard.setPair ("XkbVariant",
			XkbVariant->ascii()
		);
		}
	}

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
// XKeyboard internal page called...
//-------------------------------------
void XKeyboard::pageCalled (int index) {
	// ...
	// this function is called if the next page of the dialog
	// is called. Note if the dialog has only one page this function
	// will not be called because there is no next page in this case
	// ---
	switch (index) {
	case KeyboardPage1: {
		break;
	}
	};
}

//=====================================
// XKeyboard init page with infos...
//-------------------------------------
void XKeyboard::initPage (void) {
	// log (L_INFO,"XKeyboard::initPage() called\n");
    // ...
    // this function is called after the page was created.
    // It must only be used to init the widgets contents
    // with the data available for selections and other stuff
    // like that
    // ---
	XWrapPointer< QDict<char> > mText (mTextPtr);

	mLayoutHash = mRules.getLayouts();
	mModelHash  = mRules.getModels();
	mOptionHash = mRules.getOptions();
	//================================================
	// include available layouts
	//------------------------------------------------
	QDictIterator<char> itLayout (mLayoutHash);
	QListBox* layoutBox = new QListBox();
	for (; itLayout.current(); ++itLayout) {
		layoutBox -> insertItem ( QString::fromLocal8Bit (itLayout.current()) );
		QCheckListItem* item = new QCheckListItem (
			mAddView,"",QCheckListItem::CheckBox
		);
		item->setText ( 1, QString::fromLocal8Bit (itLayout.current()) );
		item->setText ( 2, itLayout.currentKey() );
	}
	layoutBox -> sort ( true );
	mLayout -> setListBox ( layoutBox );
	
	//================================================
	// include available models
	//------------------------------------------------
	QDictIterator<char> itModel (mModelHash);
	QListBox* typeBox = new QListBox();
	for (; itModel.current(); ++itModel) {
		typeBox -> insertItem ( QString::fromLocal8Bit (itModel.current()) );
    }
	typeBox -> sort ( true );
	mType -> setListBox ( typeBox );
	mAddView -> setSorting (1);
	//================================================
	// include available options
	//------------------------------------------------
	QString* xkbDefault = new QString (
		mText["Default"]
	);
	mXkbOption[0]->insertItem (*xkbDefault);
	mXkbOption[1]->insertItem (*xkbDefault);
	QDictIterator<char> itOption (mOptionHash);
	for (; itOption.current(); ++itOption) {
		XStringList completeOption (itOption.currentKey());
		completeOption.setSeperator (":");
		QString optkey = completeOption.getList().getFirst();
		if (optkey == "grp") {
		mXkbOption[0] -> insertItem (
			QString::fromLocal8Bit (itOption.current())
		);
		}
		if (optkey == "ctrl") {
		mXkbOption[1] -> insertItem (
			QString::fromLocal8Bit (itOption.current()) 
		);
		}
	}
	for (int i=2;i<6;i++) {
		mXkbOption[i]->insertItem (*xkbDefault);
		mXkbOption[i]->insertItem ("Meta");
		mXkbOption[i]->insertItem ("Compose");
		mXkbOption[i]->insertItem ("ModeShift");
		mXkbOption[i]->insertItem ("ModeLock");
		mXkbOption[i]->insertItem ("ScrollLock");
		mXkbOption[i]->insertItem ("Control");
	}
	//================================================
	// set defaults
	//------------------------------------------------
	mCurrentDelay = XKBDDELAY_DEFAULT;
	mCurrentKrate = XKBDRATE_DEFAULT;
}

//=====================================
// XKeyboard virtual slots...
//-------------------------------------
bool XKeyboard::slotRun (int index) {
	if (XTemplate::slotRun (index)) {
	// log(L_INFO,"XKeyboard::slotRun() called: %d\n",index);
	// ...
	// this function is called if the keyboard page is activated.
	// use this function to init the dialog with the current
	// setup of the keyboard
	// ---
	XWrapPointer< QDict<char> > mText (mTextPtr);
	mStatus -> message (mText["RunXKeyboard"]);
	mFrame  -> nextButton() -> setText (mText["finish"]);
	// ...
	// get the mFiles pointer wrapper from the Intro
	// object which has read all the data files. Than
	// lookup the keyboard XData object
	// ---
	QDict<XFile>* mFilePtr = mIntro->getFiles();
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);
	XData* sysData = mFiles["sys_KEYBOARD"] -> getDevice(0);
	if (! sysData) {
		return (FALSE);
	}
	QDict<char> keyboardInfo = sysData -> getData();
	QString XKBLayouts  = keyboardInfo["XkbLayout"];
	QString XKBVariants = keyboardInfo["XkbVariant"];
	QString XKBModel    = keyboardInfo["XkbModel"];

	//=====================================
	// clean QListView data fields
	//-------------------------------------
	mAddView -> clearSelection();
	QListViewItemIterator itAdd (mAddView);
	for ( ; itAdd.current(); ++itAdd ) {
		QCheckListItem* item = (QCheckListItem*)itAdd.current();
		item -> setOn   ( false );
		item -> setText ( 3 , "" );
	}

	//=====================================
	// select base keyboard model
	//-------------------------------------
	QDictIterator<char> itModel (mModelHash);
	for (; itModel.current(); ++itModel) {
	if (itModel.currentKey() == XKBModel) {
		mType -> setCurrentText (QString::fromLocal8Bit (itModel.current()));
	}
	}
	
	//=====================================
	// get layout/variant lists
	//-------------------------------------
	XStringList completeLayout (XKBLayouts);
	completeLayout.setSeperator (",");
	QList<char> layoutList = completeLayout.getList();
	QString baseLayout = layoutList.getFirst();
	layoutList.removeFirst();

	XStringList completeVariant (XKBVariants);
	completeVariant.setSeperator (",");
	QList<char> variantList = completeVariant.getList();
	QString baseVariant = variantList.getFirst();
	variantList.removeFirst();
	int varCount = 0;

	//=====================================
	// select base/secondary layout(s)
	//-------------------------------------
	// 1)
	QDictIterator<char> itLayout (mLayoutHash);
	for (; itLayout.current(); ++itLayout) {
	if (itLayout.currentKey() == baseLayout) {
		mLayout -> setCurrentText (QString::fromLocal8Bit (itLayout.current()));
	}
	}
	// 2)
	QListIterator<char> it (layoutList);
	for (; it.current(); ++it) {
	QListViewItemIterator itAdd (mAddView);
	for ( ; itAdd.current(); ++itAdd ) {
		QCheckListItem* item = (QCheckListItem*)itAdd.current();
		QString layout = itAdd.current()->text(2);
		if (layout == it.current()) {
			item -> setOn (true);
			if (QString(variantList.at(varCount)) != "!") {
				item -> setText ( 3 , variantList.at(varCount) );
			}
			mAddView -> setSelected (itAdd.current(), true);
			mAddView -> ensureItemVisible (item);
			varCount++;
		}
	}
	}
	updateVariants();

	//=====================================
	// select base variant
	//-------------------------------------
	for (int n=0;n<mVariant->count();n++) {
		QString item = mVariant->text (n);
		if (item == baseVariant) {
			mVariant -> setCurrentText (baseVariant);
		}
	}
	}
	return (TRUE);
}

//=====================================
// XKeyboard select basic layout...
//-------------------------------------
void XKeyboard::slotLayout ( int ) {
	// log(L_INFO,"XKeyboard::slotLayout() called\n");
	// ...
	// every time the primary language layout gets changed
	// this function is called
	// ---
	updateVariants();
	validateLayout();
	apply();
}

//=====================================
// XKeyboard select hardware type...
//-------------------------------------
void XKeyboard::slotType ( int ) {
	// log(L_INFO,"XKeyboard::slotType() called\n");
	// ...
	// every time the keyboard type gets changed this
	// function is called
    // ---
	apply();
}

//=====================================
// XKeyboard::slotAddLayout 
//-------------------------------------
void XKeyboard::slotAddLayout ( QListViewItem* ) {
	// log(L_INFO,"XKeyboard::slotAddLayout() called\n");
	// ...
	// every time the user changed the selection in the addOn
	// layout list this function is called to update the
	// available list of Variants according to the selected
	// layout
	// ---
	updateVariants();
	apply();
}

//=====================================
// XKeyboard::additional Variant(s)
//-------------------------------------
void XKeyboard::slotAddVariant ( int ) {
	// log(L_INFO,"XKeyboard::slotAddVariant() called\n");
	// ...
	// if the variant type in the addon listview has
	// changed this dialog is called
	// ---
	QListViewItem* item = mAddView -> selectedItem();
	item -> setText (3,"");
	if (mAddVariant->currentText() != "basic") {
		item -> setText (3, mAddVariant->currentText());
		apply();
	}
}

//=====================================
// XKeyboard::base Variant
//-------------------------------------
void XKeyboard::slotVariant ( int ) {
	// log(L_INFO,"XKeyboard::slotVariant() called\n");
	// ...
	// if the base variant has changed this function
	// is called
	// ---
	apply();
}

//=====================================
// XKeyboard::Layout/Model check
//-------------------------------------
void XKeyboard::validateLayout ( void ) {
	// log(L_INFO,"XKeyboard::validateLayout() called\n");
	// ...
	// this function check if the currently used combination
	// of layout and model is valid according to the Keyboard.map
	// if not we will provide a warning
	// ---
	QString XkbModel;
	QString XkbLayout;
	QDictIterator<char> itModel (mModelHash);
	for (; itModel.current(); ++itModel) {
	if (QString::fromLocal8Bit(itModel.current()) == mType->currentText()) {
		XkbModel = itModel.currentKey();
	}
	}
	// 2) primary XKB layout
	QDictIterator<char> itLayout (mLayoutHash);
	for (; itLayout.current(); ++itLayout) {
	if (QString::fromLocal8Bit(itLayout.current()) == mLayout->currentText()) {
		XkbLayout = itLayout.currentKey();
	}
	}
	QString isValid = qx ( VALIDATELAYOUT,STDOUT,2,"%s %s",
		XkbModel.ascii(),XkbLayout.ascii()
	);
	if (! isValid.toInt()) {
		setMessage ("noValidLayout");
	}
}

//=====================================
// XKeyboard::update all variant lists
//-------------------------------------
void XKeyboard::updateVariants ( void ) {
	// log(L_INFO,"XKeyboard::updateVariants() called\n");
	// ...
	// this function will update the variant lists for the
	// primary variant combobox and the additional variant
	// combobox. Currently set variants will be resetted
	// after the list update
	// ---
	// 1) Additional Variants...
	// ---
	mAddVariant -> clear();
	QListViewItem* item = mAddView -> selectedItem();
	if (item) {
		QStringList list = mRules.getVariants (item->text (2));
		if (! list.empty()) {
			mAddVariant -> insertStringList ( list );
		} else {
			mAddVariant -> insertItem ("basic");
		}
		mAddVariant->setCurrentText ("basic");
		if ((item->text(3)) && (! item->text(3).isEmpty())) {
			mAddVariant -> setCurrentText (item->text(3));
		}
	}
	// 2) Primary Variant...
	// ---
	int curItem = mVariant -> currentItem();
	QString curText = mVariant -> currentText();
	mVariant -> clear();
	QDictIterator<char> it (mRules.getLayouts());
	bool emptyVariantList = true;
	for (; it.current(); ++it) {
	if (QString::fromLocal8Bit (it.current()) == mLayout->currentText()) {
		QStringList list = mRules.getVariants (it.currentKey());
		if (! list.empty()) {
			mVariant -> insertStringList ( list );
			emptyVariantList = false;
		} else {
			mVariant -> insertItem ("basic");
		}
		break;
	}
	}
	mVariant->setCurrentText ("basic");
	if (! emptyVariantList) {
	if (mVariant->text (curItem) == curText) {
		mVariant->setCurrentItem (curItem);
	}
	}
}

//=====================================
// XKeyboard open option dialog
//-------------------------------------
void XKeyboard::slotOption ( void ) {
	// log(L_INFO,"XKeyboard::slotOption() called\n");
	// ...
	// This slot is called if you press the options
	// button to enter the keyboard options dialog
	// ---
	setupTop ();
	mTop -> show();
	mTop -> enterEvent ( 0 );
}

//=====================================
// XKeyboard get index of item text
//-------------------------------------
int XKeyboard::getItem ( QComboBox* widget, const QString& item ) {
	QListBox* list = widget -> listBox();
	for (int i=0;i<widget->count();i++) {
	if (list->item(i)->text() == item ) {
		return (i);
	}
	}
	return (0);
}

//=====================================
// XKeyboard init function for toplevel
//-------------------------------------
void XKeyboard::setupTop ( void ) {
	// log (L_INFO,"XKeyboard::setupTop() called\n");
	// ...
	// this function is called if the setup toplevel window
	// is created to configure options for the selected
	// keyboard type
	// ---
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);
	XWrapPointer<XData> workingKeyboard (
		mFiles["sys_KEYBOARD"]->getDevice (0)
	);
	// ...
	// lookup AutoRepeat value to init slider
	// values within the options dialog
	// ---
	if (workingKeyboard["AutoRepeat"]) {
		XStringList valList;
		valList.setText (workingKeyboard["AutoRepeat"]);
		valList.setSeperator (" ");
		QList<char> kbd = valList.getList();
		mCurrentDelay = atoi(kbd.at(0));
		mCurrentKrate = atoi(kbd.at(1));
		mDelay  -> setValue (
			mCurrentDelay
		);
		mRepeat -> setValue (
			mCurrentKrate
		);
	}
	// ...
	// init all XKB values in the key-mapping dialog
	// overwrite the default setup with it
	// ---
	for (int i=0;i<6;i++) {
		mXkbOption[i]->setCurrentItem (0);
	}
	if (workingKeyboard["XkbOptions"]) {
		XStringList optList;
		optList.setText (workingKeyboard["XkbOptions"]);
		optList.setSeperator (",");
		QList<char> opt = optList.getList();
		QListIterator<char> it (opt);
		for (; it.current(); ++it) {
			QDictIterator<char> itOption (mOptionHash);
			for (; itOption.current(); ++itOption) {
			if (itOption.currentKey() == QString(it.current())) {
				QString optionText = QString::fromLocal8Bit (
					itOption.current()
				);
				int index0 = getItem ( mXkbOption[0],optionText );
				int index1 = getItem ( mXkbOption[1],optionText );
				if (index0 != 0) {
					mXkbOption[0] -> setCurrentItem ( index0 );
				}
				if (index1 != 0) {
					mXkbOption[1] -> setCurrentItem ( index1 );
				}
			}
			}
		}
	}
	if (workingKeyboard["LeftAlt"]) {
		mXkbOption[2]->setCurrentItem (
		getItem (mXkbOption[2],workingKeyboard["LeftAlt"])
		);
	}
	if (workingKeyboard["RightAlt"]) {
		mXkbOption[3]->setCurrentItem (
		getItem (mXkbOption[3],workingKeyboard["RightAlt"])
		);
	}
	if (workingKeyboard["ScrollLock"]) {
		mXkbOption[4]->setCurrentItem (
		getItem (mXkbOption[3],workingKeyboard["ScrollLock"])
		);
	}
	if (workingKeyboard["RightCtl"]) {
		mXkbOption[5]->setCurrentItem (
		getItem (mXkbOption[3],workingKeyboard["RightCtl"])
		);
	}
}

//=====================================
// XKeyboard cancel pressed for mTop
//-------------------------------------
void XKeyboard::slotTopCancel ( void ) {
	// log (L_INFO,"XKeyboard::slotTopCancel() called\n");
	// ...
	// this function is called if you click onto the Cancel 
	// button in the setup toplevel window
	// ---
	mFrame -> enterEvent ( 0 );
	// ...
	// reset the symbol rate/delay 
	// using 'xset r' command
	// ---
	QString optr ("r");
	QString rate ("rate");
	QString delay; delay.sprintf ("%d",mCurrentDelay);
	QString krate; krate.sprintf ("%d",mCurrentKrate);
	qx ( XSET,STDNONE,4,"%s %s %s %s",
		optr.ascii(),rate.ascii(),
		delay.ascii(),krate.ascii()
	);
}

//=====================================
// XKeyboard Ok pressed for mTop
//-------------------------------------
void XKeyboard::slotTopOk ( void ) {
	// log (L_INFO,"XKeyboard::slotTopOk() called\n");
	// ...
	// this function is called if you click onto the OK 
	// button in the setup toplevel window
	// ---
	XWrapPointer< QDict<char> > mText (mTextPtr);

	mFrame -> enterEvent ( 0 );
	QString* XkbOptions = NULL;
	QString* ScrollLock = NULL;
	QString* LeftAlt    = NULL;
	QString* RightAlt   = NULL;
	QString* RightCtl   = NULL;
	for (int i=0;i<6;i++) {
		QString selection = mXkbOption[i]->currentText();
		QDictIterator<char> it (mOptionHash);
		for (; it.current(); ++it) {
		if (QString::fromLocal8Bit (it.current()) == selection) {
			selection = it.currentKey();
			break;
		}
		}
		if (selection == mText["Default"]) {
			selection = "";
		}
		switch (i) {
		case 0:
			if (! XkbOptions) {
				XkbOptions = new QString (selection);
			} else {
				XkbOptions -> sprintf ("%s,%s",
					XkbOptions->ascii(),selection.ascii()
				);
			}
		break;
		case 1:
			if (! XkbOptions) {
				XkbOptions = new QString (selection);
			} else {
				XkbOptions -> sprintf ("%s,%s",
					XkbOptions->ascii(),selection.ascii()
				);
			}
		break;
		case 2:
			LeftAlt = new QString (selection);
		break;
		case 3:
			RightAlt = new QString (selection);
		break;
		case 4:
			ScrollLock = new QString (selection);
		break;
		case 5:
			RightCtl = new QString (selection);
		break;
		}
		if (XkbOptions) {
		if (XkbOptions -> at(XkbOptions->length()-1) == ',') {
			XkbOptions -> remove (XkbOptions->length()-1,1);
		}
		}
	}
	// ...
	// save the values to the working keyboard if
	// they are available and valid
	// ---
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);
	XWrapPointer<XData> workingKeyboard (
		mFiles["sys_KEYBOARD"]->getDevice (0)
	);
	if (XkbOptions) {
		workingKeyboard.setPair ("XkbOptions",XkbOptions->ascii());
	}
	if (LeftAlt) {
		workingKeyboard.setPair ("LeftAlt",LeftAlt->ascii());
	}
	if (RightAlt) {
		workingKeyboard.setPair ("RightAlt",RightAlt->ascii());
	}
	if (ScrollLock) {
		workingKeyboard.setPair ("ScrollLock",ScrollLock->ascii());
	}
	if (RightCtl) {
		workingKeyboard.setPair ("RightCtl",RightCtl->ascii());
	}
	// ...
	// save the values of the auto repeat sliders, if one
	// of them has another value than the default value
	// ---
	if (
		(mDelay->value() != XKBDDELAY_DEFAULT) || 
		(mRepeat->value() != XKBDRATE_DEFAULT)
	) {
		QString* autoRepeat = new QString();
		autoRepeat->sprintf("%d %d",
			mDelay->value(),mRepeat->value()
		);
		workingKeyboard.setPair (
			"AutoRepeat",autoRepeat->ascii()
		);
	}
	apply();
}

//=====================================
// XKeyboard auto repeat set
//-------------------------------------
void XKeyboard::autoRepeat ( void ) {
	// log(L_INFO,"XKeyboard::autoRepeat() called\n");
	// ...
	// This function is called if either the repeat or
	// delay rate has changed. We will use xset to apply
	// the new rate/delay values to the X-Server. The
	// function is called after the slider has been 
	// released
	// ---
	XkbDescPtr xkb;
	int major = XkbMajorVersion;
	int minor = XkbMinorVersion;
	int op, event, error;
	int timeout = 500;
	Display* dpy = QApplication::desktop()->x11Display();
	if (XkbQueryExtension(dpy, &op, &event, &error, &major, &minor) == 0) {
		log (L_INFO,"XKeyboard::Unable to initialize XKB extension");
		return;
	}
	while (timeout > 0) {
		xkb = XkbGetKeyboard (
			dpy, XkbGBN_AllComponentsMask, XkbUseCoreKbd
		);
		if (xkb == NULL || xkb->geom == NULL) {
			timeout -= 1; usleep(100);
		} else {
			break;
		}
	}
	if (timeout <= 0) {
		log (L_INFO,"XKeyboard::Couldn't get keyboard\n");
		return;
	}
	XkbGetControls (dpy, XkbRepeatKeysMask, xkb);
	xkb->ctrls->repeat_delay = mDelay->value();
	xkb->ctrls->repeat_interval = 1000/mRepeat->value();
	XkbSetControls (
		dpy, XkbRepeatKeysMask, xkb
	);
	XSync (dpy, False);
}

//=====================================
// XKeyboard apply keyboard
//-------------------------------------
bool XKeyboard::apply ( void ) {
	// log(L_INFO,"XKeyboard::apply() called\n");
	// ...
	// apply the keyboard to the X-Server for immediate
	// usage. This call will use the XKB extension
    // ---
	XWrapPointer< QDict<char> > mText (mTextPtr);

	QString* XkbModel   = NULL;
	QString* XkbLayout  = NULL;
	QString* XkbVariant = NULL;
	QString* XkbOptions = NULL;
	// 1) primary XKB model
	QDictIterator<char> itModel (mModelHash);
	for (; itModel.current(); ++itModel) {
	if (QString::fromLocal8Bit (itModel.current()) == mType->currentText()) {
		XkbModel = new QString (itModel.currentKey());
	}
	}
	// 2) primary XKB layout
	QDictIterator<char> itLayout (mLayoutHash);
	for (; itLayout.current(); ++itLayout) {
	if (QString::fromLocal8Bit(itLayout.current()) == mLayout->currentText()) {
		XkbLayout = new QString (itLayout.currentKey());
	}
	}
	// 3) variant for primary XKB layout
	XkbVariant = new QString();
	if (mVariant->currentText()) {
		*XkbVariant = mVariant->currentText();
	}
	// 4) additional layout and variant
	QListViewItemIterator itAdd (mAddView);
	for ( ; itAdd.current(); ++itAdd ) {
	QCheckListItem* item = (QCheckListItem*)itAdd.current();
	if (item->isOn()) {
		QString layout  = itAdd.current()->text(2);
		QString variant = "";
		XkbLayout->sprintf("%s,%s",
			XkbLayout->ascii(),layout.ascii()
		);
		if (itAdd.current()->text(3)) {
			variant = itAdd.current()->text(3);
		}
		XkbVariant->sprintf("%s,%s",
			XkbVariant->ascii(),variant.ascii()
		);
	}
	}
	// 5) options
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);
	XWrapPointer<XData> workingKeyboard (
		mFiles["sys_KEYBOARD"]->getDevice (0)
	);
	if (workingKeyboard["XkbOptions"]) {
	if (! workingKeyboard["XkbOptions"].isEmpty()) {
		XkbOptions = new QString (workingKeyboard["XkbOptions"]);
	}
	}
	QString optm ("-model");
    QString optl ("-layout");
    QString opto ("-option");
    QString optv ("-variant");
	if ((XkbVariant) && (XkbOptions)) {
		#if 0
		printf("%s %s %s %s %s %s %s %s\n",
			optm.ascii(),XkbModel->ascii(),optl.ascii(),XkbLayout->ascii(),
			opto.ascii(),XkbOptions->ascii(),optv.ascii(),XkbVariant->ascii()
		);
		#endif
		#if 1
		qx ( SETXKBMAP,STDNONE,8,"%s %s %s %s %s %s %s %s",
			optm.ascii(),XkbModel->ascii(),optl.ascii(),XkbLayout->ascii(),
			opto.ascii(),XkbOptions->ascii(),optv.ascii(),XkbVariant->ascii()
		);
		#endif
	} else if (XkbVariant) {
		#if 0
		printf("%s %s %s %s %s %s\n",
			optm.ascii(),XkbModel->ascii(),optl.ascii(),XkbLayout->ascii(),
			optv.ascii(),XkbVariant->ascii()
		);
		#endif
		#if 1
		qx ( SETXKBMAP,STDNONE,8,"%s %s %s %s %s %s",
			optm.ascii(),XkbModel->ascii(),optl.ascii(),XkbLayout->ascii(),
			optv.ascii(),XkbVariant->ascii()
		);
		#endif
	} else if (XkbOptions) {
		#if 0
		printf("%s %s %s %s %s %s\n",
			optm.ascii(),XkbModel->ascii(),optl.ascii(),XkbLayout->ascii(),
			opto.ascii(),XkbOptions->ascii()
		);
		#endif
		#if 1
		qx ( SETXKBMAP,STDNONE,8,"%s %s %s %s %s %s",
			optm.ascii(),XkbModel->ascii(),optl.ascii(),XkbLayout->ascii(),
			opto.ascii(),XkbOptions->ascii()
		);
		#endif
	} else {
		#if 0
		printf("%s %s %s %s\n",
			optm.ascii(),XkbModel->ascii(),optl.ascii(),XkbLayout->ascii()
		);
		#endif
		#if 1
		qx ( SETXKBMAP,STDNONE,8,"%s %s %s %s",
			optm.ascii(),XkbModel->ascii(),optl.ascii(),XkbLayout->ascii()
		);
		#endif
	}
	return (true);
}
