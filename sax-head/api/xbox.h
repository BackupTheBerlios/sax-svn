/**************
FILE          : xbox.h
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the XFree86 license
              :
DESCRIPTION   : xbox wrapper class for QMessageBox
              : to create a frame around the dialog because
              : YaST2 does not provide a Windowmanager
              :
STATUS        : Status: Up-to-date
**************/
#ifndef XBOX_H
#define XBOX_H 1

#include <qapplication.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <qlineedit.h>
#include <qframe.h>
#include <qimage.h>

#define XBOX_NORMAL   0
#define XBOX_OPT_ANY  1
#define XBOX_OPT_INT  2

class XBox : public QWidget {
	Q_OBJECT

	public:
	enum Icon {
	    NoIcon      = 0,
		Information = 1,
		Warning     = 2,
		Critical    = 3
	};
	
	enum Button {
		NoButton = 0,
		Ok       = 1,
		Cancel   = 2,
		Yes      = 3,
		No       = 4,
		Abort    = 5,
		Retry    = 6,
		Ignore   = 7
	};

	public slots:
	void closeEvent (QCloseEvent*);

	private slots:
	void slotButton1 (void);
	void slotButton2 (void);
	void slotButton3 (void);

	private:
	QLineEdit* mLineWidget;
	QSpinBox*  mSpinWidget;
	QFrame* mFrame;
	QLabel* mLabel;
	QLabel* mImage;
	QPushButton* mBtn1;
	QPushButton* mBtn2;
	QPushButton* mBtn3;
	int buttonStatus;
	int mButtonID0;
	int mButtonID1;
	int mButtonID2;
	int mButtonCount;

	public:
	XBox (
	const QString&,const QString&,Icon,
	int,int,int, QWidget* =0,int =0,int =0,const char* =0,
	bool =TRUE,WFlags =0
	);
	void disableButton ( int );
	void enableButton ( int );
	~XBox (void);

	public:
	QString getLineEditText ( void );
	int getSpinBoxCount ( void );
	void setButtonText (Button,const QString&);
	void setButtonText (int,const QString&);
	int exec (void);

	protected:
	virtual void showEvent   ( QShowEvent * );
	virtual bool eventFilter ( QObject*, QEvent* );

	public:
	virtual void enterEvent ( QEvent* );
};

#endif
