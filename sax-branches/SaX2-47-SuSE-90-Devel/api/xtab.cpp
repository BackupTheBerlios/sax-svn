/**************
FILE          : xtab.c
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system
              : released under the XFree86 license
              :
DESCRIPTION   : Wrapper class for QTabWidget
              : to create a frame around the dialog because
              : YaST2 does not provide a Windowmanager
              :
STATUS        : Status: Up-to-date
**************/
#include <stdio.h>
#include <unistd.h>

#include "xtab.h"
#include <X11/Xlib.h>

//============================================
// XTabDialog Constructor
//--------------------------------------------
XTabDialog::XTabDialog (
	QWidget* parent, const char* name, 
	bool modal,WFlags f,int frameWidth
) : QWidget ( parent,name, 
	(modal ? (f|WType_Modal) : f) | WType_TopLevel | WStyle_Dialog
) {
	installEventFilter (this);
	mCancel = mOk = NULL;

	QGridLayout* layer0  = new QGridLayout (this,1,1);
	mFrame = new QFrame ( this );
	mFrame -> setFrameStyle (
		QFrame::Box|QFrame::Raised
	);
	mFrame -> setLineWidth ( frameWidth - 1 );
	mFrame -> setMidLineWidth ( frameWidth );
	layer0 -> addWidget ( mFrame,0,0 );

	QBoxLayout* layer1 = new QVBoxLayout ( mFrame );
	mLayer2 = new QHBoxLayout ( layer1 );
	layer1 -> setMargin  (8);
	layer1 -> addSpacing (10);
	mLayer3 = new QHBoxLayout ( layer1 );

	mTab = new QTabWidget ( mFrame,name,f );
	mLayer2 -> addWidget  ( mTab );
	mLayer3 -> addStretch ( 10 );

	QWidget* thisParent = parentWidget();
	if (thisParent) {
		setGeometry (
		thisParent->x() + 80,thisParent->y() + 80,
		thisParent->width() - 160,thisParent->height() - 160
		);
	}
}

//============================================
// XTabDialog addTab 
//--------------------------------------------
void XTabDialog::addTab ( QWidget* w, const QString & text ) {
	mTab -> addTab (w,text);
}

//============================================
// XTabDialog changeTab 
//--------------------------------------------
void XTabDialog::changeTab ( QWidget * w, const QString & label ) {
	mTab -> changeTab (w,label);
}

//============================================
// XTabDialog showPage 
//--------------------------------------------
void XTabDialog::showPage ( QWidget* w ) {
	mTab -> showPage (w);
}

//============================================
// XTabDialog setCancelButton
//--------------------------------------------
void XTabDialog::setCancelButton ( const QString & text ) {
	mCancel = new QPushButton ( text,mFrame );
	QObject::connect (
		mCancel,SIGNAL (clicked()), this , SLOT (slotCancel())
	);
	mLayer3 -> addWidget ( mCancel );
}

//============================================
// XTabDialog setOkButton
//--------------------------------------------
void XTabDialog::setOkButton ( const QString & text ) {
	mOk = new QPushButton (text,mFrame);
	mOk -> setAutoDefault (true);
	QObject::connect (
		mOk,SIGNAL (clicked()), this , SLOT (slotOk())
	);
	mLayer3 -> addWidget ( mOk );
	mLayer3 -> setSpacing (8);
}

//============================================
// XTabDialog slotCancel
//--------------------------------------------
void XTabDialog::slotCancel (void) {
	hide();
	cancelButtonPressed();
}

//============================================
// XTabDialog slotOk
//--------------------------------------------
void XTabDialog::slotOk (void) {
	hide();
	applyButtonPressed();
}

//============================================
// XTabDialog showEvent
//--------------------------------------------
void XTabDialog::showEvent ( QShowEvent* ) {
	int xo = 0;
	int yo = 0;
	int w  = width();
	int h  = height();
	QWidget* parent = parentWidget();
	if ( parent ) {
		xo = parent->x();
		yo = parent->y();
	}
	if (qApp->mainWidget()) {
	move (
		qApp->mainWidget()->x() + ((parent->width()  - w) / 2),
		qApp->mainWidget()->y() + ((parent->height() - h) / 2)
	);
	}
	setFocus();
	enterEvent ( 0 );
}

//============================================
// XTabDialog set input focus if entered
//--------------------------------------------
void XTabDialog::enterEvent ( QEvent * ) {
	XSetInputFocus (
		x11Display(),winId(),RevertToParent,CurrentTime
	);
}

//===================================== 
// XTabDialog set global event filter
//-------------------------------------
bool XTabDialog::eventFilter ( QObject* obj, QEvent* event ) {
	if ((QPushButton*)obj != NULL) {
		QPushButton* btn = (QPushButton*)qApp->focusWidget();
		if (! btn) {
			return (false);
		}
		switch (event->type()) {
		//======================================
		// handle press event
		//--------------------------------------
		case 6:
		if (((QKeyEvent*)event)->key() == Key_Return) {
			QKeyEvent spacePress ( (QEvent::Type)6, Qt::Key_Space, 32 ,0 );
			QApplication::sendEvent ( (QObject*)btn,&spacePress );
			return (true);
		}
		break;
		//======================================
		// handle release event
		//--------------------------------------
		case 7:
		if (((QKeyEvent*)event)->key() == Key_Return) {
			QKeyEvent spaceRelease ( (QEvent::Type)7, Qt::Key_Space, 32 ,0 );
			QApplication::sendEvent ( (QObject*)btn,&spaceRelease );
			return (true);
		}
		break;
		default:
		break;
		}
	}
	return (false);
}

