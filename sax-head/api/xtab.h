/**************
FILE          : xtab.h
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the GPL license
              :
DESCRIPTION   : Wrapper class for QTabDialog
              : to create a frame around the dialog because
              : YaST2 does not provide a Windowmanager
              :
STATUS        : Status: Up-to-date
**************/
#ifndef XTAB_H
#define XTAB_H 1

#include <qapplication.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qframe.h>
#include <qtabwidget.h>

//============================================
// XTabDialog Class
//--------------------------------------------
class XTabDialog : public QWidget {
	Q_OBJECT

	private:
	QFrame*       mFrame;
	QTabWidget*   mTab;
	QPushButton*  mCancel;
	QPushButton*  mOk;
	QBoxLayout*   mLayer2;
	QBoxLayout*   mLayer3;

	public:
	XTabDialog (
		QWidget* =0, const char* =0, bool =FALSE, WFlags =0,int =0 
	);
	void addTab ( QWidget *, const QString & );
	void setCancelButton ( const QString & );
	void changeTab ( QWidget*, const QString & );
	void setOkButton ( const QString & );
	void showPage ( QWidget* );

	protected:
	virtual void showEvent  ( QShowEvent * );

	public:
	virtual void enterEvent  ( QEvent* );
	virtual bool eventFilter ( QObject*, QEvent* );

	private slots:
	void slotCancel (void);
	void slotOk (void);

	signals:
	void cancelButtonPressed (void);
	void applyButtonPressed (void);
};

#endif
