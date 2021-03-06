/**************
FILE          : vnc.h
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Sch�fer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the GPL license
              :
DESCRIPTION   : xapi (X11-Application-Interface)
              : header definitions for vnc.c
              :
STATUS        : Status: Up-to-date
**************/
#ifndef XVNC_H
#define XVNC_H 1

#include <unistd.h>
#include <qmessagebox.h>
#include <qradiobutton.h>
#include <qstatusbar.h>
#include <qmainwindow.h>
#include <qframe.h>
#include <qlayout.h>
#include <qcheckbox.h>
#include <qspinbox.h>
#include <time.h>

#include "../intro.h"
#include "../../xbox.h"

#define UNIQUE 10

enum {
	XVncPage1 = XVnc,
};

//=====================================
// Class Headers...
//-------------------------------------
class Xvnc : public XTemplate {
	Q_OBJECT

    private:
    QFrame*       mDialog;
    XIntro*       mIntro;
	QDict<XFile>* mFilePtr;
	QString       mPWD;
	bool          VNCPrepared;
	
	private:
	QCheckBox*    mCheckVNC;
	QCheckBox*    mCheckPWD;
	QButtonGroup* mOptions;
	QLineEdit*    mPWD1;
	QLineEdit*    mPWD2;
	QSpinBox*     mHTTP;
	QCheckBox*    mCheckViewOnly;
	QCheckBox*    mCheckShared;
	QCheckBox*    mCheckHTTP;
	QVBox*        mEntries;
	QVBox*        mHTTPEntries;

	private:
	bool noInputConnection;

    public:
	Xvnc (XFrame*,XIntro*);
	Xvnc (void);
	void addTo (XFrame* xf,XIntro* xi);
	void setMessage (
		const QString&,XBox::Icon = XBox::Warning,const QString& = 0,int=0
	);

	private:
	bool saveConfiguration    (void);
	void setDeviceOption      (QString);
	void setDeviceRawOption   (QString,QString,int=0);
	void unsetDeviceOption    (QString);
	void unsetDeviceRawOption (QString);
	void resetDeviceRawOption (QString,QString,int=0);
	void resetDeviceOption    (QString);

	private:
	void ap_to64 (char*,unsigned long,int);
	QString cryptPassword   (QString);
	QString getDeviceOption (QString);

	private:
	void dialogCreate (void);
	void resetPage    (int = PAGE_NOLOAD);
	void pageCalled   (int);
	void initPage     (void);

	protected:
	bool slotRun   (int);
	void slotIntro (int);

	public slots:
	void slotState    (bool);
	void slotPassword (bool);
	void slotHTTP     (bool);
	void slotInput1Changed (const QString&);
};

#endif
