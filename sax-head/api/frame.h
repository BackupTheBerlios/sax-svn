/**************
FILE          : frame.h
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
              : - frame.h: SaXGUI::SCCFrame header definitions
              : - frame.cpp: builds basic layout structure
              : ----
              :
STATUS        : Status: Development
**************/
#ifndef SCCFRAME_H
#define SCCFRAME_H 1

//====================================
// Includes...
//------------------------------------
#include <qwidget.h>
#include <qframe.h>
#include <qdict.h>
#include <qstring.h>
#include <qlayout.h>
#include <qsplitter.h>
#include <qpushbutton.h>
#include <qlistbox.h>
#include <qwidgetstack.h>
#include <qprogressdialog.h>
#include <qlistview.h>
#include <qlabel.h>
#include <qvbox.h>
#include <qdict.h>

#include "../libsax/sax.h"

//====================================
// Includes...
//------------------------------------
#include "file.h"
#include "xapi.h"
#include "message.h"
#include "wrapper.h"
#include "mouse.h"
#include "monitor.h"
#include "touchscreen.h"
#include "keyboard.h"
#include "tablet.h"
#include "vnc.h"
#include "xidle.h"

namespace SaXGUI {
//====================================
// Defines...
//------------------------------------
#define FRAME_MAIN         1
#if 0
#define FRAME_TITLE        2
#endif
#define FRAME_MODULES      3
#define FRAME_TOPIC        4

//====================================
// Defines...
//------------------------------------
#define TDOMAIN            "/usr/share/locale"
#define INFDIR             "/usr/share/sax/api/data/"
#define MACRODIR           "/usr/share/sax/api/macros/"
#define PIXDIR             "/usr/share/sax/api/figures"
#define XQUERY             "/sbin/xquery"

//====================================
// Defines...
//------------------------------------
#define GTX_FILE           INFDIR   "xapi.gtx"
#define GET3D              MACRODIR "get3D"
#define GETINSTALLED       MACRODIR "getInstalled"

//====================================
// Defines...
//------------------------------------
#if 0
#define TITLE_GRADIENT     PIXDIR "/title-bar-gradient.png"
#endif
#define TITLE_PIXMAP       PIXDIR "/logo.png"
#define POINTERS_PIXMAP    PIXDIR "/mouse.png"
#define KEYBOARD_PIXMAP    PIXDIR "/keyboard.png"
#define MONITORS_PIXMAP    PIXDIR "/monitor.png"
#define VNCSERVE_PIXMAP    PIXDIR "/vnc.png"
#define TOUCHERS_PIXMAP    PIXDIR "/touch.png"
#define XTABLETS_PIXMAP    PIXDIR "/tablet.png"

//====================================
// Class SCCFrame
//------------------------------------
class SCCFrame : public QWidget {
	Q_OBJECT

	private:
	QDict<QString>* mTextPtr;

	private:
	QDict<SaXImport> mSection;
	SaXConfig*       mConfig;
	bool mYaSTMode;
	int  mGUIMode;

	private:
	QFrame*       mMainFrame;
	#if 1
	QFrame*       mSeperator;
	#endif
	QBoxLayout*   mMainLayout;
	QBoxLayout*   mTitleLayout;
	QBoxLayout*   mWorkLayout;
	QBoxLayout*   mDoneLayout;

	private:
	QListBox*     mModuleList;
	#if 0
	QListBox*     mModuleTitle;
	#else
	QHBox*        mModuleTitleBox;
	QVBox*        mModuleTitleLabel;
	QLabel*       mModuleTitle;
	#endif
	QWidgetStack* mDialogStack;
	#if 0
	QHBox*        mTitleBox;
	#endif
	QHBox*        mModuleBox;

	private:
	QPushButton*  mHelper;
	QPushButton*  mCancel;
	QPushButton*  mFinish;

	private:
	SCCMouse*       mMouse;
	SCCKeyboard*    mKeyboard;
	SCCMonitor*     mMonitor;
	SCCTablet*      mTablet;
	SCCTouchScreen* mToucher;
	SCCVNC*         mVNC;

	private:
	void runDialog ( QListBoxItem* );
	void hideFrame ( int = FRAME_MODULES );
	void loadText  ( void );
	void exitSaX   ( int = 0 );
	void setupModules    ( void );
	void loadApplication ( void );
	void saveConfiguration ( void );
	void testConfiguration ( void );
	void installConfiguration ( void );
	bool prepareConfiguration ( void );

	public:
	void evaluateAutoDetection ( void );
	bool startDialog ( const QString& );
	QDict<QString>* getTextPtr ( void );

	protected:
	virtual bool eventFilter ( QObject*, QEvent* );
	virtual void closeEvent  ( QCloseEvent* );

	public slots:
	void slotHelp   ( void );
	void slotCancel ( void );
	void slotFinish ( void );

	public slots:
	void slotSelected ( QListBoxItem* );
	
	public:
	SCCFrame (
		bool, int, bool, bool,
		bool, bool, QString*, bool, WFlags=WType_TopLevel
	);
};
} // end namespace
#endif
