/**************
FILE          : xprogress.cpp
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the XFree86 license
              :
DESCRIPTION   : wrapper class for QProgressDialog
              : to create a frame around the dialog because
              : YaST2 does not provide a Windowmanager
              :
STATUS        : Status: Up-to-date
**************/
#include <stdio.h>
#include <unistd.h>

#include "xprogress.h"
#include <X11/Xlib.h>

//=====================================
// XProgressDialog::Constructor
//-------------------------------------
XProgressDialog::XProgressDialog (
	const QString & labelText, const QString & cancelButtonText, 
	int totalSteps, QWidget* parent, const char* name, bool modal,
	WFlags f,int frameWidth 
) : QWidget (
	parent,name,
	(modal ? (f|WType_Modal) : f) | WType_TopLevel | WStyle_Dialog
) {
	QGridLayout* layer0  = new QGridLayout (
		this,1,1
	);
	mFrame = new QFrame ( this );
	mFrame -> setFrameStyle (
		QFrame::Box|QFrame::Raised
	);
	mFrame -> setLineWidth ( frameWidth - 1 );
	mFrame -> setMidLineWidth ( frameWidth );
	layer0 -> addWidget (mFrame,0,0);

	QBoxLayout* layer1 = new QVBoxLayout ( mFrame );
	layer1 -> setMargin  (10);
	QBoxLayout* layer2 = new QHBoxLayout ( layer1 );
	layer1 -> addSpacing (10);
	QBoxLayout* layer3 = new QHBoxLayout ( layer1 );
	layer1 -> addSpacing (8);
	QBoxLayout* layer4 = new QHBoxLayout ( layer1 );

	mTopic = new QLabel ( 
		labelText,mFrame 
	);
	mBar = new QProgressBar ( 
		totalSteps,mFrame 
	);
	mCancel = new QPushButton ( 
		cancelButtonText,mFrame 
	);
	mCancel -> setAutoDefault (true);

	QObject::connect (
		mCancel,SIGNAL (clicked()), this , SLOT (slotCancel())
	);

	layer2 -> addStretch ( 5 );
	layer2 -> addWidget  ( mTopic );
	layer2 -> addStretch ( 5 );
	layer3 -> addWidget  ( mBar );
	layer4 -> addStretch ( 10 );
	layer4 -> addWidget  ( mCancel );

	adjustSize ();
	mView = true;
}

//=====================================
// XProgressDialog::showDialog
//-------------------------------------
void XProgressDialog::showDialog (bool status) {
	mView = status;
}

//=====================================
// XProgressDialog::slotCancel
//-------------------------------------
void XProgressDialog::slotCancel (void) {
	cancelled();
}

//=====================================
// XProgressDialog::setLabelText
//-------------------------------------
void XProgressDialog::setLabelText (const QString & text) {
	mTopic->setText (text);
}

//=====================================
// XProgressDialog::setCancelButtonText
//-------------------------------------
void XProgressDialog::setCancelButtonText (const QString & text) {
	mCancel->setText (text);
}

//=====================================
// XProgressDialog::setTotalSteps
//-------------------------------------
void XProgressDialog::setTotalSteps (int steps) {
	mBar->setTotalSteps (steps);
}

//=====================================
// XProgressDialog::setProgress
//-------------------------------------
void XProgressDialog::setProgress (int progress) {
	if ((isHidden()) && (mView)) {
		show(); enterEvent ( 0 );
	}
	mBar->setProgress (progress);
	qApp->processEvents();
}

//=====================================
// XProgressDialog::setMinimumDuration
//-------------------------------------
void XProgressDialog::setMinimumDuration (int) {
}

//=====================================
// XProgressDialog::setAutoClose
//-------------------------------------
void XProgressDialog::setAutoClose (bool) {
}

//=====================================
// XProgressDialog::cancel
//-------------------------------------
void XProgressDialog::cancel (void) {
	if (isVisible()) {
		hide();
	}
}

//=====================================
// XProgressDialog::totalSteps
//-------------------------------------
int XProgressDialog::totalSteps (void) {
	return mBar->totalSteps();
}

//============================================
// Show event
//--------------------------------------------
void XProgressDialog::showEvent ( QShowEvent* ) {
	int xo = 0;
	int yo = 0;
	int w  = width();
	int h  = height();
	QWidget* parent = parentWidget();
	if ( parent ) {
		xo = parent->x();
		yo = parent->y();
	}
	if ((parent) && (qApp->mainWidget())) {
	move ( 
		qApp->mainWidget()->x() + ((parent->width()  - w) / 2),
		qApp->mainWidget()->y() + ((parent->height() - h) / 2)
	);
	} else { 
	move (
		(qApp->desktop()->width()  - w) / 2 - xo,
		(qApp->desktop()->height() - h) / 2 - yo
	);
	}
	enterEvent ( 0 );
	setFocus();
}

//=====================================
// XProgressDialog set input focus
//-------------------------------------
void XProgressDialog::enterEvent ( QEvent * ) {
	XSetInputFocus (
		x11Display(),winId(),RevertToParent,CurrentTime
	);
}
