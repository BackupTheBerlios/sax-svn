/**************
FILE          : monitor.h
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the GPL license
              :
DESCRIPTION   : xapi (X11-Application-Interface)
              : header definitions for monitor.c
              :
STATUS        : Status: Up-to-date
**************/
#ifndef MONITOR_H
#define MONITOR_H 1

#include <qmessagebox.h>
#include <qframe.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <qcombobox.h>
#include <qcheckbox.h>
#include <qlayout.h>

#include "../intro.h"
#include "../../xtab.h"

//=====================================
// Defines...
//-------------------------------------
enum {
    MonitorPage1 = Monitor,
};

//=====================================
// Class Headers...
//-------------------------------------
class XMonitor : public XTemplate {
	Q_OBJECT

    private:
    QFrame*       mDialog;
    XIntro*       mIntro;
	QDict<XFile>* mFilePtr;
	XTabDialog*   mTop;
	QPushButton*  mSetup;
	QPushButton*  mRemove;
	QListBox*     monitorList;
	QCheckBox*    mDPMS;
	QListBox*     mVendor;
	QListBox*     mName;
	QPushButton*  mDisk;
	QComboBox*    mTraversal;
	QComboBox*    mRatio;
	QSpinBox*     mHSpinMin;
	QSpinBox*     mHSpinMax;
	QSpinBox*     mVSpinMin;
	QSpinBox*     mVSpinMax;
	QComboBox*    mMode;
	QSpinBox*     mXspin;
	QSpinBox*     mYspin;
	QVBox*        layer11;

	private:
	int           mSelected;
	QString       mInitialAlgorithm;
	QString       mInitialHsync;
	QString       mInitialVsync;      

    public:
    XMonitor (XFrame*,XIntro*);
    XMonitor (void);
    void addTo (XFrame* xf,XIntro* xi);

    private:
    void dialogCreate ( void );
    void resetPage    ( int = PAGE_NOLOAD);
    void pageCalled   ( int );
	void initPage     ( void );
	void setupTop     ( void );
	bool validatePage ( void );
	QPoint translateTraversal ( double,int,int );
	double getTraversal  ( int,int );
	QPoint getRatio   ( int,int );

    protected:
    bool slotRun   ( int );
	void slotIntro ( int );

	public slots:
	void slotSelect       ( QListBoxItem* );
	void slotDoubleSelect ( QListBoxItem* );
	void slotVendor       ( QListBoxItem *);
	void slotName         ( QListBoxItem *);
	void slotSetup     ( void );
	void slotRemove    ( void );
	void slotTopOk     ( void );
    void slotTopCancel ( void );
	void slotDisk      ( void );
	void slotHsyncMin  ( int );
	void slotHsyncMax  ( int );
	void slotVsyncMin  ( int );
	void slotVsyncMax  ( int );
	void slotAlgorithm ( int );
	void slotXSize     ( int );
	void slotYSize     ( int );
	void slotTraversal ( int );
	void slotRatio     ( int );
};


#endif
