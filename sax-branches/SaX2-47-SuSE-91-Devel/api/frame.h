/**************
FILE          : frame.h
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the XFree86 license
              :
DESCRIPTION   : xapi (X11-Application-Interface)
			  : header definitions for frame.c
              :
STATUS        : Status: Up-to-date
**************/
#ifndef XFRAME_H
#define XFRAME_H 1

#include <unistd.h>
#include <qapplication.h>
#include <qstatusbar.h>
#include <qwidget.h>
#include <qpainter.h>
#include <math.h>
#include <qlist.h>
#include <qtimer.h>
#include <stdio.h>
#include <stdlib.h>
#include <qframe.h>
#include <qdict.h>
#include <qwidgetstack.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qscrollview.h>
#include <qlistbox.h>
#include <qlistview.h>

#include "xquery.h"
#include "xprogress.h"
#include "config.h"

//====================================
// Defines...
//------------------------------------
#define EXIT_NOSAVE 0
#define EXIT_SAVE   1

//====================================
// Enumerators...
//------------------------------------
enum Weight { 
	Light    = 25, 
	Normal   = 50, 
	DemiBold = 63, 
	Bold     = 75, 
	Black    = 87 
};
enum Modules {
	Intro    = 1,
	Mouse    = 2,
	Tablet   = 3,
	Toucher  = 4,
	Keyboard = 5,
	Monitor  = 6,
	Colors   = 7,
	Layout   = 8,
	Modus    = 9,
	Geometry = 10,
	Card     = 11,
	Xaccess  = 12,
	OpenGL   = 13,
	Virtual  = 14
};

//=====================================
// Templates
//-------------------------------------
template <class T>
class XWrapText {
	private:
	T* pM;

	public:
	XWrapText (void) { }
	XWrapText (T* typ) {
		pM = typ;
	}
	void init (T* typ) {
		pM = typ;
	}
	QString operator[] (const char* key) {
		return (QString::fromLocal8Bit(pM->operator[](key)));
	}
};

//====================================
// Class headers...
//------------------------------------
class XFrame : public QWidget {
	Q_OBJECT

	private:
	XProgressDialog *mProgress;
	QFrame*         mainFrame;
	QFrame*         mDialog;
	QFrame*         mMenu;
	QFrame*         mButtonBar1;
	QFrame*         mButtonBar2;
	QString*        mDisplay;
	QWidgetStack*   mWstack;
	QScrollView*    mSview;
	QStatusBar*     mStatus;
	QLabel*         mImage;
	QPushButton*    mBack;
	QPushButton*    mNext;
	QPushButton*    mCancel;
	QPushButton*    mStart;
	QPushButton*    mFinish;
	QListView*      mView;

	private:
	int             mFrameWidth;
	int             mComponent;
	QDict<char>     mText;
	QDict<char>     mInit;
	QDict<char>     mTree;
	bool            mDataChanged;
	QDict<int>      mFileChange;
	bool            mSetFrameCalled;
	bool            mAdjustable;
	int             mDialogWidth;
	int             mExitCode;
	int             mWidth;
	int             mHeight;
	QListViewItem*  IDdesktop;  
	QListViewItem*  IDmulti;
	QListViewItem*  IDinput;
	QListViewItem*  IDcolor;
	QListViewItem*  IDmonitor;
	QListViewItem*  IDcard;
	QListViewItem*  IDkeyboard;
	QListViewItem*  IDtoucher;
	QListViewItem*  IDtablet;
	QListViewItem*  IDmouse;
	QListViewItem*  IDgeo;
	QListViewItem*  IDmode;
	QListViewItem*  IDlayout;
	QListViewItem*  IDaccessx;
	QListViewItem*  IDvirtual;
	QListViewItem*  ID3D;
	QGridLayout*    layer8;

	private:
	XWrapText< QDict<char> > xText;

	public:
	bool fastSetup;
	bool useHwData;
	bool fullScreen;
	bool yastMode;

	public:
	XFrame (int =0,WFlags=WType_TopLevel);
	XFrame (int,int,int,int,int =0,WFlags=WType_TopLevel);
	QWidgetStack* getStack (void);
	QDict<char>   getInit  (void);
	QDict<char>   getText  (void);
	QDict<char>*  getTextPointer (void);
	XProgressDialog* getProgressDialog (void);
	QListView*       getComponentView  (void);
	QPushButton* cancelButton (void);
	QPushButton* finishButton (void);
	QPushButton* backButton   (void);
	QPushButton* nextButton   (void);
	QPushButton* startButton  (void);
	QFrame*      buttonBar1   (void);
	QFrame*      buttonBar2   (void);
	QStatusBar*  statusBar    (void);
	QString*     getDisplay   (void);
	void enableInteraction    (void);
	void disableInteraction   (void);
	void linkConfiguration    (void);
	bool frameInitialized     (void);
	void activateFirstItem    (void);
	void runDialog      (int);
	void setModified    (bool,QString*);
	void hideListView   (void);
	void showListView   (void);
	void setFrame       (void);
	void setFrameWidth  (int);
	void updateComplete (void);
	void loadText       (void);
	void set3DScript    (const QString&);
	QString get3DScript (void);

	public:
	virtual void enterEvent   ( QEvent* );

	protected:
	virtual bool eventFilter  ( QObject*, QEvent* );
	virtual void resizeEvent  ( QResizeEvent* );
	virtual void showEvent    ( QShowEvent * );

	private:
	void setDisplay    (void);
	void setArguments  (void);
	void setInit       (void);

	public slots:
	void runDialog     (QListViewItem*);
	void runIntro      (QListViewItem*);
	void listClicked   (int,QListViewItem* ,const QPoint&,int);
	void slotChanged   (QListViewItem*);
	void closeEvent    (QCloseEvent*);
	void slotFastExit  (void);

	private slots:
	void slotExit      (void);
	void slotAbout     (void);
	void slotModules   (void);
	void slotNext      (void);
	void slotBack      (void);
	void slotCancel    (void);
	void slotStart     (void);
	void slotFinish    (void);
	void slotWhatsThis (void);

	signals:
	void sigNext       (void);
	void sigBack       (void);
	void sigRun        (int);
	void sigCheck      (int);
	void sigIntro      (int);
	void sigAbort      (int);
	void sigResize     (void);
};

class XCommand {
    private:
    QString mFile;
    int     mCount;
    char    *pArg[MAX_ARGS];
    char    *pBase;

    public:
    XCommand (const QString&,const QString& = "");
    int runCommand (void);
    void addOption (const QString&);
};

#endif
