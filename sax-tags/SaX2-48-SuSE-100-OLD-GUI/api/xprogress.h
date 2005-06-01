/**************
FILE          : xprogress.h
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the GPL license
              :
DESCRIPTION   : wrapper class for QProgressDialog
              : to create a frame around the dialog because
              : YaST2 does not provide a Windowmanager
              :
STATUS        : Status: Up-to-date
**************/
#ifndef XPROGRESS_H
#define XPROGRESS_H 1

#include <qapplication.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qframe.h>
#include <qlabel.h>
#include <qprogressbar.h>
#include <qwidgetlist.h>

class XProgressDialog : public QWidget {
	Q_OBJECT

	private:
	QLabel*       mTopic;
	QProgressBar* mBar;
	QPushButton*  mCancel;
	QFrame*       mFrame;
	bool          mView;

	public:
	XProgressDialog ( 
	const QString &, const QString &, int, QWidget* =0, 
	const char* =0, bool =FALSE, WFlags f=0,int=0
	);
	void setLabelText (const QString &);
	void setCancelButtonText (const QString &);
	void setTotalSteps (int);
	void setProgress (int);
	void setMinimumDuration (int);
	void setAutoClose (bool);
	void cancel (void);
	int totalSteps (void);
	void showDialog (bool);

	protected:
	virtual void showEvent  ( QShowEvent * );
	virtual void enterEvent ( QEvent* );

	public slots:
	void slotCancel (void);
	
	signals:
	void cancelled (void);
};

#endif
