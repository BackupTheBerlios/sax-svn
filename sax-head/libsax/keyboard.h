/**************
FILE          : keyboard.h
***************
PROJECT       : SaX2 - library interface [header]
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : SaX2 - SuSE advanced X11 configuration 
              : 
              :
DESCRIPTION   : native C++ class library to access SaX2
              : functionality. Easy to use interface for
              : //.../
              : - importing/exporting X11 configurations
              : - modifying/creating X11 configurations 
              : ---
              :
              :
STATUS        : Status: Development
**************/
#ifndef SAX_KEYBOARD_H
#define SAX_KEYBOARD_H 1

//====================================
// Includes...
//------------------------------------
#include "import.h"

#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#include <X11/extensions/XKBrules.h>
#include <X11/extensions/XKBgeom.h>
#include <X11/extensions/XKM.h>
#include <X11/extensions/XKBfile.h>
#include <X11/extensions/XKBui.h>

//====================================
// Defines...
//------------------------------------
#define SAX_DEFAULT_RULE   "xfree86"
#define XKB_LEFT_ALT       "LeftAlt"
#define XKB_RIGHT_ALT      "RightAlt"
#define XKB_SCROLL_LOCK    "ScrollLock"
#define XKB_RIGHT_CTL      "RightCtl"

#define XKB_MAP_META       "Meta"
#define XKB_MAP_COMPOSE    "Compose"
#define XKB_MAP_MODESHIFT  "ModeShift"
#define XKB_MAP_MODELOCK   "ModeLock"
#define XKB_MAP_SCROLLLOCK "ScrollLock"
#define XKB_MAP_CONTROL    "Control"

#define SAX_CORE_KBD       0
#define SAX_NEXT_KBD(x)    ((x)+2)

//====================================
// Class SaXKeyRules...
//------------------------------------
class SaXKeyRulesIF : public SaXException {
	public:
	virtual QDict<QString> getModels   ( void ) = 0;
	virtual QDict<QString> getLayouts  ( void ) = 0;
	virtual QDict<QString> getOptions  ( void ) = 0;
	virtual QList<QString> getVariants ( const QString& ) = 0;

	public:
	virtual ~SaXKeyRulesIF ( void ) { }
};
class SaXKeyRules : public SaXKeyRulesIF {
	protected:
	void loadRules ( QString );

	private:
	QDict<QString> mModels;
	QDict<QString> mLayouts;
	QDict<QString> mOptions;
	QDict<QStringList> mVarLists;
	QString mX11Dir;

	public:
	QDict<QString> getModels   ( void );
	QDict<QString> getLayouts  ( void );
	QDict<QString> getOptions  ( void );
	QList<QString> getVariants ( const QString& );

	public:
	SaXKeyRules ( QString rule=SAX_DEFAULT_RULE );
};

//====================================
// Interface class for dlopen ability
//------------------------------------
class SaXManipulateKeyboardIF : public SaXKeyRules {
	public:
	virtual void setXKBModel       ( const QString& ) = 0;

	public:
	virtual void setXKBLayout      ( const QString& ) = 0;
	virtual void addXKBLayout      ( const QString& ) = 0;
	virtual void removeXKBLayout   ( const QString& ) = 0;

	public:
	virtual void setXKBOption      ( const QString& ) = 0;
	virtual void addXKBOption      ( const QString& ) = 0;
	virtual void removeXKBOption   ( const QString& ) = 0;

	public:
	virtual void setXKBVariant     ( const QString& , const QString& ) = 0;
	virtual void removeXKBVariant  ( const QString& ) = 0;

	public:
	virtual void setMapping        ( const QString& , const QString& ) = 0;

	public:
	virtual QString getXKBVariant ( const QString& ) = 0;
	virtual QString getXKBModel   ( void  ) = 0;

	public:
	virtual QList<QString> getXKBOptionList  ( void ) = 0;
	virtual QList<QString> getXKBLayout      ( void ) = 0;
	virtual QList<QString> getXKBVariantList ( void ) = 0;

	public:
	virtual bool selectKeyboard ( int ) = 0;

	public:
	virtual ~SaXManipulateKeyboardIF ( void ) { }

	protected:
	SaXManipulateKeyboardIF ( void ) : SaXKeyRules() { }
};
//====================================
// Class SaXManipulateKeyboard...
//------------------------------------
class SaXManipulateKeyboard : public SaXManipulateKeyboardIF {
	private:
	SaXImport* mImport;
	int mKeyboard;

	private:
	int findLayout            ( const QString& );
	QList<QString> createList ( const QString& );

	public:
	virtual void setXKBModel       ( const QString& );

	public:
	void setXKBLayout      ( const QString& );
	void addXKBLayout      ( const QString& );
    void removeXKBLayout   ( const QString& );

	public:
	void setXKBOption      ( const QString& );
	void addXKBOption      ( const QString& );
	void removeXKBOption   ( const QString& );

	public:
	void setXKBVariant     ( const QString& , const QString& );
	void removeXKBVariant  ( const QString& );

	public:
	void setMapping        ( const QString& , const QString& );

	public:
	QString getXKBVariant ( const QString& );
	QString getXKBModel   ( void  );

	public:
	QList<QString> getXKBOptionList  ( void );
	QList<QString> getXKBLayout      ( void );
	QList<QString> getXKBVariantList ( void );

	public:
	bool selectKeyboard ( int );

	public:
	SaXManipulateKeyboard ( SaXImport*, int = 0 );
};

#endif
