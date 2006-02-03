/**************
FILE          : keyboardoptions.cpp
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the GPL license
              :
DESCRIPTION   : SaX2 GUI system using libsax to provide
              : configurations for a wide range of X11 capabilities
              : //.../
              : - keyboardoptions.h: SaXGUI::SCCKeyboardOptions implementation
              : - keyboardoptions.cpp: configure XKB keyboard options
              : ----
              :
STATUS        : Status: Development
**************/
#include "keyboardoptions.h"

namespace SaXGUI {
//====================================
// Constructor
//------------------------------------
SCCKeyboardOptions::SCCKeyboardOptions (
	QDict<QString>* text, QDict<SaXImport> section,
	int, QWidget* parent
) : SCCDialog ( 0,text,section,parent ) {
	//=====================================
	// get translation pointer
	//-------------------------------------
	SCCWrapPointer< QDict<QString> > mText (mTextPtr);

	//=====================================
	// create layout for this widget
	//-------------------------------------
	mMainLayout = new QVBoxLayout ( this );

	//=====================================
	// create macro widgets
	//-------------------------------------
	mXKBOptionView = new QListView (this);
	mXKBOptionView -> addColumn(mText["XKBOptions"]);
	mXKBOptionView -> setResizeMode (QListView::LastColumn);
	mXKBOptionView -> setRootIsDecorated(false);
	mXKBOptionView -> setSorting ( -1 );

	//=====================================
	// add caps: tagged options
	//-------------------------------------
	mIDCapsLock = new QCheckListItem (
		mXKBOptionView,mText["XKBCapsLock"]
	);
	mIDCapsLockNone = new QCheckListItem (
		mIDCapsLock,mText["XKBNone"],QCheckListItem::RadioButton
	);
	//=====================================
	// add altwin: tagged options
	//-------------------------------------
	mIDAltWinKey = new QCheckListItem (
		mXKBOptionView,mText["XKBAltWinKey"]
	);
	mIDAltWinKeyNone = new QCheckListItem (
		mIDAltWinKey,mText["XKBNone"],QCheckListItem::RadioButton
	); 
	//=====================================
	// add lv3: tagged options
	//-------------------------------------
	mIDThirdLevel = new QCheckListItem (
		mXKBOptionView,mText["XKBThirdLevelChooser"],QCheckListItem::CheckBox
	);
	//=====================================
	// add ctrl: tagged options
	//-------------------------------------
	mIDControl = new QCheckListItem (
		mXKBOptionView,mText["XKBControlKey"]
	);
	mIDControlNone = new QCheckListItem (
		mIDControl,mText["XKBNone"],QCheckListItem::RadioButton
	);
	//=====================================
	// add compose: tagged options
	//-------------------------------------
	mIDCompose = new QCheckListItem (
		mXKBOptionView,mText["XKBComposeKey"],QCheckListItem::CheckBox
	);
	//=====================================
	// add grp_led: tagged options
	//-------------------------------------
	mIDKbdLedToShow = new QCheckListItem (
		mXKBOptionView,mText["XKBUseKbdLed"],QCheckListItem::CheckBox
	);
	//=====================================
	// add grp: tagged options
	//-------------------------------------	
	mIDGroupShiftLock = new QCheckListItem (
		mXKBOptionView,mText["XKBShiftLockGroup"],QCheckListItem::CheckBox
	);
	//=====================================
	// opening tree
	//-------------------------------------
	mIDGroupShiftLock -> setOpen ( true );
	mIDKbdLedToShow   -> setOpen ( true );
	mIDCompose        -> setOpen ( true );
	mIDControl        -> setOpen ( true );
	mIDThirdLevel     -> setOpen ( true );
	mIDAltWinKey      -> setOpen ( true );
	mIDCapsLock       -> setOpen ( true );

	//=====================================
	// connect widgets
	//-------------------------------------
	QObject::connect(
		mXKBOptionView , SIGNAL (clicked       ( QListViewItem* )),
		this           , SLOT   (slotXKBOption ( QListViewItem* ))
	);
	//=====================================
	// add widgets to the layout
	//-------------------------------------
	mMainLayout -> setMargin ( 15 );
	mMainLayout -> addWidget ( mXKBOptionView );
}
//====================================
// init
//------------------------------------
void SCCKeyboardOptions::init ( void ) {
	//=====================================
	// get translation pointer
	//-------------------------------------
	SCCWrapPointer< QDict<QString> > mText (mTextPtr);

	//====================================
	// query XKB file extension
	//------------------------------------
	SaXKeyRules XKBFile;
	mOptionDict = XKBFile.getOptions();

	//====================================
	// insert available XKB options
	//------------------------------------
	QDictIterator<QString> it (mOptionDict);
	for (; it.current(); ++it) {
		QString key = it.currentKey();
		QString val = mText[*it.current()];
		if (val.isEmpty()) {
			log (L_WARN,
				"SCCKeyboardOptions::Warning: unknown XKB key: %s\n",
				it.current()->ascii()
			);
		}
		if (key.contains("caps:")) {
			new QCheckListItem (mIDCapsLock,val,QCheckListItem::RadioButton);
		}
		if (key.contains("altwin:")) {
			new QCheckListItem (mIDAltWinKey,val,QCheckListItem::RadioButton);
		}
		if (key.contains("lv3:")) {
			new QCheckListItem (mIDThirdLevel,val,QCheckListItem::CheckBox);
		}
		if (key.contains("ctrl:")) {
			new QCheckListItem (mIDControl,val,QCheckListItem::RadioButton);
		}
		if (key.contains("compose:")) {
			new QCheckListItem (mIDCompose,val,QCheckListItem::CheckBox);
		}
		if (key.contains("grp_led:")) {
			new QCheckListItem (mIDKbdLedToShow,val,QCheckListItem::CheckBox);
		}
		if (key.contains("grp:")) {
			new QCheckListItem (mIDGroupShiftLock,val,QCheckListItem::CheckBox);
		}
	}
}
//====================================
// import
//------------------------------------
void SCCKeyboardOptions::import ( void ) {
	//====================================
	// create needed manipulators
	//------------------------------------
	SaXManipulateKeyboard saxKeyboard (
		mSection["Keyboard"]
	);
	//====================================
	// get keyboard option list
	//------------------------------------
	QList<QString> XKBOptions = saxKeyboard.getXKBOptionList();

	//====================================
	// handle options
	//------------------------------------
	QListIterator<QString> it (XKBOptions);
	for (; it.current(); ++it) {
		QString XKBOptionTranslation = translateOption (*it.current());
		QListViewItemIterator io (mXKBOptionView);
		for (; io.current(); ++io ) {
			QCheckListItem* item = (QCheckListItem*)io.current();
			if (item->text() == XKBOptionTranslation) {
				item->setOn   ( true );
				slotXKBOption ( item );
			}
		}
	}
}
//====================================
// slotXKBOption
//------------------------------------
void SCCKeyboardOptions::slotXKBOption ( QListViewItem* item ) {
	if ((item == mIDCompose)        ||
		(item == mIDThirdLevel)     ||
		(item == mIDKbdLedToShow)   || 
		(item == mIDGroupShiftLock)
	) {
		updateTree ( item, true  );
	} else {
		updateTree ( item, false );
	}
	QList<QString> usedOptions;
	QListViewItemIterator it (mXKBOptionView);
	for (; it.current(); ++it ) {
		QCheckListItem* item = (QCheckListItem*)it.current();
		if ((item) && (item->isOn())) {
			usedOptions.append (new QString(item->text()));
		}
	}
	mXKBOption = "";
	QListIterator<QString> io (usedOptions);
	for (; io.current(); ++io) {
		QString XKBOptionName = findOption (*io.current());
		if (! XKBOptionName.isEmpty()) {
			mXKBOption.sprintf ("%s,%s",
				mXKBOption.ascii(),XKBOptionName.ascii()
			);
		}
	}
	mXKBOption.replace(QRegExp("^,"),"");
	emit sigApply();
}
//====================================
// updateTree
//------------------------------------
void SCCKeyboardOptions::updateTree ( QListViewItem* item,bool rootItem ) {
	if ( rootItem ) {
		//=====================================
		// root item selected...
		//-------------------------------------
		QListViewItemIterator it (mXKBOptionView);
		for (; it.current(); ++it ) {
			QCheckListItem* subItem = (QCheckListItem*)it.current();
			if ((subItem) && (subItem->parent() == item)) {
				QCheckListItem* baseItem = (QCheckListItem*)item;
				subItem->setOn (baseItem->isOn());
			}
		}
	} else {
		//=====================================
		// sub-item selected...
		//-------------------------------------
		QCheckListItem* parent   = (QCheckListItem*)item->parent();
		QCheckListItem* thisItem = (QCheckListItem*)item;
		if (thisItem -> isOn()) {
			if (! parent -> isOn()) {
				parent -> setOn ( true );
			}	
			return;
		}
		if ((parent == mIDCompose)        ||
			(parent == mIDThirdLevel)     ||
			(parent == mIDKbdLedToShow)   ||
			(parent == mIDGroupShiftLock)
		) {
			QListViewItemIterator it (mXKBOptionView);
			for (; it.current(); ++it ) {
				QCheckListItem* subItem  = (QCheckListItem*)it.current();
				QCheckListItem* baseItem = (QCheckListItem*)subItem->parent();
				if ((baseItem == parent) && subItem->isOn() ) {
					return;
				}
			}
			parent -> setOn ( false );
		}
	}
}
//====================================
// findOption
//------------------------------------
QString SCCKeyboardOptions::findOption ( const QString& option ) {
	//=====================================
	// get translation pointer
	//-------------------------------------
	SCCWrapPointer< QDict<QString> > mText (mTextPtr);

	//=====================================
	// search option
	//-------------------------------------
	QDictIterator<QString> it (mOptionDict);
	QString* result = new QString("");
	for (; it.current(); ++it) {
		QString key = it.currentKey();
		QString val = mText[*it.current()];
		if (val == option) {
			QTextOStream (result) << key;
			break;
		}
	}
	return *result;
}
//====================================
// translateOption
//------------------------------------
QString SCCKeyboardOptions::translateOption ( const QString& option ) {
	//=====================================
	// get translation pointer
	//-------------------------------------
	SCCWrapPointer< QDict<QString> > mText (mTextPtr);

	//=====================================
	// translate option
	//-------------------------------------
	QString* result = new QString("");
	if (mOptionDict[option]) {
		QTextOStream (result) << mText[*mOptionDict[option]];
	}
	return *result;
}
//====================================
// getOptions
//------------------------------------
QString SCCKeyboardOptions::getOptions ( void ) {
	return mXKBOption;
}
//====================================
// getApplyString
//------------------------------------
QString SCCKeyboardOptions::getApplyString ( void ) {
	QString XkbOptions = getOptions();
	QString result;
	if (! XkbOptions.isEmpty()) {
		QTextOStream (&result) << "-option " << XkbOptions;
	}
	return result;
}
} // end namespace
