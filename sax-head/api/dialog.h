/**************
FILE          : dialog.h
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
              : - dialog.h: SaXGUI::SCCDialog header definitions
              : - dialog.cpp: build base class for all dialogs
              : ----
              :
STATUS        : Status: Development
**************/
#ifndef SCCDIALOG_H
#define SCCDIALOG_H 1

#include <qwidget.h>
#include <qwidgetstack.h>
#include <qlayout.h>
#include <qdict.h>

#include "/suse/ms/sax/libsax/sax.h"

#include "common/log.h"
#include "wrapper.h"
#include "message.h"
#include "file.h"

namespace SaXGUI {
//====================================
// Class SCCDialog
//------------------------------------
class SCCDialog : public QWidget {
	Q_OBJECT

	private:
	QWidgetStack* mDialogStack;
	int mDialogID;

	protected:
	QWidget*         mParent;
	QDict<QString>*  mTextPtr;
	QDict<SaXImport> mSection;
	QBoxLayout*      mDialogLayout;
	QFrame*          mDialogFrame;

	protected:
	int  mGUIMode;
	bool needImportData;
	bool needInitData;

	public:
	virtual void init   ( void );
	virtual void import ( void );

	public:
	int  getDialogID   ( void );
	bool needInit      ( void );
	bool needImport    ( void );
	QPoint getPosition ( int,int );

	public:
	SCCDialog (
		QWidgetStack*,QDict<QString>*,
		QDict<SaXImport>, QWidget* = 0
	);
};
} // end namespace
#endif
