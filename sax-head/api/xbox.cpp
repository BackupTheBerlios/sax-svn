/**************
FILE          : xbox.c
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
#include <stdio.h>
#include <unistd.h>

#include "xbox.h"
#include <X11/Xlib.h>

//============================================
// XPM global definitions
//--------------------------------------------
static const char * const information_xpm[]={
"32 32 5 1", 
". c None",
"c c #000000",
"* c #999999",
"a c #ffffff",
"b c #0000ff",
"...........********.............",
"........***aaaaaaaa***..........",
"......**aaaaaaaaaaaaaa**........",
".....*aaaaaaaaaaaaaaaaaa*.......",
"....*aaaaaaaabbbbaaaaaaaac......",
"...*aaaaaaaabbbbbbaaaaaaaac.....",
"..*aaaaaaaaabbbbbbaaaaaaaaac....",
".*aaaaaaaaaaabbbbaaaaaaaaaaac...",
".*aaaaaaaaaaaaaaaaaaaaaaaaaac*..",
"*aaaaaaaaaaaaaaaaaaaaaaaaaaaac*.",
"*aaaaaaaaaabbbbbbbaaaaaaaaaaac*.",
"*aaaaaaaaaaaabbbbbaaaaaaaaaaac**",
"*aaaaaaaaaaaabbbbbaaaaaaaaaaac**",
"*aaaaaaaaaaaabbbbbaaaaaaaaaaac**",
"*aaaaaaaaaaaabbbbbaaaaaaaaaaac**",
"*aaaaaaaaaaaabbbbbaaaaaaaaaaac**",
".*aaaaaaaaaaabbbbbaaaaaaaaaac***",
".*aaaaaaaaaaabbbbbaaaaaaaaaac***",
"..*aaaaaaaaaabbbbbaaaaaaaaac***.",
"...caaaaaaabbbbbbbbbaaaaaac****.",
"....caaaaaaaaaaaaaaaaaaaac****..",
".....caaaaaaaaaaaaaaaaaac****...",
"......ccaaaaaaaaaaaaaacc****....",
".......*cccaaaaaaaaccc*****.....",
"........***cccaaaac*******......",
"..........****caaac*****........",
".............*caaac**...........",
"...............caac**...........",
"................cac**...........",
".................cc**...........",
"..................***...........",
"...................**..........."};
/* XPM */
static const char* const warning_xpm[]={
"32 32 4 1",
". c None",
"a c #ffff00",
"* c #000000",
"b c #999999",
".............***................",
"............*aaa*...............",
"...........*aaaaa*b.............",
"...........*aaaaa*bb............",
"..........*aaaaaaa*bb...........",
"..........*aaaaaaa*bb...........",
".........*aaaaaaaaa*bb..........",
".........*aaaaaaaaa*bb..........",
"........*aaaaaaaaaaa*bb.........",
"........*aaaa***aaaa*bb.........",
".......*aaaa*****aaaa*bb........",
".......*aaaa*****aaaa*bb........",
"......*aaaaa*****aaaaa*bb.......",
"......*aaaaa*****aaaaa*bb.......",
".....*aaaaaa*****aaaaaa*bb......",
".....*aaaaaa*****aaaaaa*bb......",
"....*aaaaaaaa***aaaaaaaa*bb.....",
"....*aaaaaaaa***aaaaaaaa*bb.....",
"...*aaaaaaaaa***aaaaaaaaa*bb....",
"...*aaaaaaaaaa*aaaaaaaaaa*bb....",
"..*aaaaaaaaaaa*aaaaaaaaaaa*bb...",
"..*aaaaaaaaaaaaaaaaaaaaaaa*bb...",
".*aaaaaaaaaaaa**aaaaaaaaaaa*bb..",
".*aaaaaaaaaaa****aaaaaaaaaa*bb..",
"*aaaaaaaaaaaa****aaaaaaaaaaa*bb.",
"*aaaaaaaaaaaaa**aaaaaaaaaaaa*bb.",
"*aaaaaaaaaaaaaaaaaaaaaaaaaaa*bbb",
"*aaaaaaaaaaaaaaaaaaaaaaaaaaa*bbb",
".*aaaaaaaaaaaaaaaaaaaaaaaaa*bbbb",
"..*************************bbbbb",
"....bbbbbbbbbbbbbbbbbbbbbbbbbbb.",
".....bbbbbbbbbbbbbbbbbbbbbbbbb.."};
/* XPM */
static const char* const critical_xpm[]={
"32 32 4 1",
". c None",
"a c #999999",
"* c #ff0000",
"b c #ffffff",
"...........********.............",
".........************...........",
".......****************.........",
"......******************........",
".....********************a......",
"....**********************a.....",
"...************************a....",
"..*******b**********b*******a...",
"..******bbb********bbb******a...",
".******bbbbb******bbbbb******a..",
".*******bbbbb****bbbbb*******a..",
"*********bbbbb**bbbbb*********a.",
"**********bbbbbbbbbb**********a.",
"***********bbbbbbbb***********aa",
"************bbbbbb************aa",
"************bbbbbb************aa",
"***********bbbbbbbb***********aa",
"**********bbbbbbbbbb**********aa",
"*********bbbbb**bbbbb*********aa",
".*******bbbbb****bbbbb*******aa.",
".******bbbbb******bbbbb******aa.",
"..******bbb********bbb******aaa.",
"..*******b**********b*******aa..",
"...************************aaa..",
"....**********************aaa...",
"....a********************aaa....",
".....a******************aaa.....",
"......a****************aaa......",
".......aa************aaaa.......",
".........aa********aaaaa........",
"...........aaaaaaaaaaa..........",
".............aaaaaaa............"};


//============================================
// XBox Constructor
//--------------------------------------------
XBox::XBox (
	const QString& caption, const QString &text, Icon icon,
	int button0, int button1, int button2, QWidget* parent, 
	int frameWidth,int type,const char* name, bool modal, WFlags f
) : QWidget ( 
	parent,name,
	(modal ? (f|WType_Modal) : f) | WType_TopLevel | WStyle_Dialog 
) {
		installEventFilter (this);
		mButtonCount = 0;

		mButtonID0 = button0;
		mButtonID1 = button1;
		mButtonID2 = button2;
		mBtn1 = mBtn2 = mBtn3 = 0;

		QGridLayout* layer0  = new QGridLayout (this,1,1);
		mFrame = new QFrame ( this );
		mFrame -> setFocusPolicy (QWidget::StrongFocus);
		mFrame -> setFrameStyle (
			QFrame::Box|QFrame::Raised
		);
		mFrame -> setLineWidth ( frameWidth - 1);
		mFrame -> setMidLineWidth ( frameWidth );
		layer0 -> addWidget (mFrame,0,0);

		QBoxLayout* layer1 = new QVBoxLayout ( mFrame );
		QBoxLayout* layer2 = new QHBoxLayout ( layer1 );
		layer1 -> addSpacing (10);
		QBoxLayout* layer3 = new QVBoxLayout ( layer2 );
		QBoxLayout* layer5 = new QVBoxLayout ( layer2 );
		layer1 -> addStretch (50);
		layer1 -> setMargin  (15);
		QBoxLayout* layer4 = new QHBoxLayout ( layer1 );
		layer4 -> addStretch (10);

		mLineWidget = NULL;
		mSpinWidget = NULL;
		switch (type) {
		case XBOX_OPT_ANY:
			mLineWidget = new QLineEdit (mFrame);
		break;
		case XBOX_OPT_STRING:
			mComboWidget = new QComboBox (mFrame);
		break;
		case XBOX_OPT_INT:
			mSpinWidget = new QSpinBox (0,100,1,mFrame);
		break;
		}
		mLabel = new QLabel ( mFrame );
		mImage = new QLabel ( mFrame );
		mImage -> setFixedWidth (34);

		if ( button0 ) {
			mBtn1 = new QPushButton ( mFrame );
			mBtn1 -> setDefault (true);
			layer4 -> addWidget  ( mBtn1 );
			if ( button1 ) {
				layer4 -> addSpacing ( 10 );
			}
			QObject::connect (
				mBtn1,SIGNAL (clicked()), this , SLOT (slotButton1())
			);
		}
		if ( button1 ) {
			mBtn2 = new QPushButton ( mFrame );
			layer4 -> addWidget  ( mBtn2 );
			if ( button2 ) {
	            layer4 -> addSpacing ( 10 );
			}
			QObject::connect (
				mBtn2,SIGNAL (clicked()), this , SLOT (slotButton2())
			);
		}
		if ( button2 != 0) {
			mBtn3 = new QPushButton ( mFrame );
			layer4 -> addWidget  ( mBtn3 );
			QObject::connect (
				mBtn3,SIGNAL (clicked()), this , SLOT (slotButton3())
			);
		}

		const char * const * xpmData;
		switch ( icon ) {
		case Information:
			xpmData = information_xpm;
		break;
		case Warning:
			xpmData = warning_xpm;
		break;
		case Critical:
			xpmData = critical_xpm;
		break;
		default:
			xpmData = 0;
		break;
		}
		if ( xpmData ) {
			QImage image( (const char **) xpmData);
			QPixmap pixmap;
			pixmap.convertFromImage (image);
			mImage -> setPixmap (
				pixmap
			);
		}

		mLabel -> setText ( text );

		layer4 -> addStretch ( 10 );
		layer3 -> addWidget  ( mImage );
		layer3 -> addStretch ( 50 );
		layer2 -> addSpacing ( 10 );
		layer5 -> addWidget  ( mLabel );

		switch (type) {
		case XBOX_OPT_ANY:
			layer5 -> addSpacing ( 10 );
			layer5 -> addWidget  ( mLineWidget );
			layer5 -> addSpacing ( 10 );
		break;
		case XBOX_OPT_STRING:
			layer5 -> addSpacing ( 10 );
			layer5 -> addWidget  ( mComboWidget );
			layer5 -> addSpacing ( 10 );
		break;
		case XBOX_OPT_INT:
			layer5 -> addSpacing ( 10 );
			layer5 -> addWidget ( mSpinWidget );
			layer5 -> addSpacing ( 10 );
		break;
		}

		setCaption ( caption );
		adjustSize ();
}


//============================================
// XBox destructor
//--------------------------------------------
XBox::~XBox (void) {
	delete (mFrame);
}

//============================================
// get text from line edit widget
//--------------------------------------------
QString XBox::getLineEditText ( void ) {
	QString text;
	if (mLineWidget) {
		text = mLineWidget->text();
	}
	return (text);
}

//============================================
// set text of the line edit widget
//--------------------------------------------
void XBox::setLineEditText ( const QString& data ) {
	if (mLineWidget) {
		mLineWidget->setText (data);
	}
}

//============================================
// get text from combo box widget
//--------------------------------------------
QString XBox::getComboBoxText ( void ) {
	QString text;
	if (mComboWidget) {
		text = mComboWidget->currentText();
	}
	return (text);
}

//============================================
// set combo box items
//--------------------------------------------
void XBox::setComboBoxText ( const QStringList& data ) {
	if (mComboWidget) {
		mComboWidget->insertStringList (data);
	}
}

//============================================
// set current combo box item
//--------------------------------------------
void XBox::setComboBoxCurrent ( const QString& data ) {
	if (mComboWidget) {
		mComboWidget->setCurrentText (data);
	}
}

//============================================
// get number from spin box widget
//--------------------------------------------
int XBox::getSpinBoxCount ( void ) {
	if (mSpinWidget) {
		return (mSpinWidget->value());
	}
	return (0);
}

//============================================
// set button texts
//--------------------------------------------
void XBox::setButtonText (Button nr,const QString& text) {
	mButtonCount++;
	switch (nr) {
	case Ok:
		mBtn1 -> setText ( text );
	break;
	case Cancel:
		mBtn2 -> setText ( text );
	break;
	case Yes:
		mBtn1 -> setText ( text );
	break;
	case No:
		mBtn2 -> setText ( text );
	break;
	default:
		mBtn3 -> setText ( text );
	break;
	}
}

//============================================
// set button texts
//--------------------------------------------
void XBox::setButtonFocus (int nr) {
	mBtn1 -> setDefault (false);
	switch (nr) {
	case 0:
		mBtn1 -> setDefault (true);
		mBtn1 -> setFocus ();
	break;
	case 1:
		mBtn2 -> setDefault (true);
		mBtn2 -> setFocus ();
	break;
	case 2:
		mBtn3 -> setDefault (true);
		mBtn3 -> setFocus ();
	break;
	default:
	break;
	}
}

//============================================
// set button texts
//--------------------------------------------
void XBox::setButtonText (int nr,const QString& text) {
	setButtonText ((Button)nr,text);
}

//============================================
// show dialog box and wait
//--------------------------------------------
int XBox::exec (void) {
	show();
	enterEvent ( 0 );
	int w = 80;
	if (mButtonCount != 1) {
		w = width() / mButtonCount - 60;
	}
	QFontMetrics metrics ( font() );
	if (mBtn1) {
		if (metrics.width (mBtn1->text()) + 20 > w) {
			w = metrics.width (mBtn1->text()) + 20;
		}
		mBtn1 -> setFixedWidth (w);
	}
	if (mBtn2) {
		if (metrics.width (mBtn2->text()) + 20 > w) {
			w = metrics.width (mBtn2->text()) + 20;
		}
		mBtn2 -> setFixedWidth (w);
	}
	if (mBtn3) {
		if (metrics.width (mBtn3->text())+ 20 > w) {
			w = metrics.width (mBtn3->text()) + 20;
		}
		mBtn3 -> setFixedWidth (w);
	}
	qApp->enter_loop();
	return (
		buttonStatus
	);
}

//============================================
// Button1 event
//--------------------------------------------
void XBox::slotButton1 (void) {
	buttonStatus = mButtonID0;
	qApp->exit_loop();
}

//============================================
// Button2 event
//--------------------------------------------
void XBox::slotButton2 (void) {
	buttonStatus = mButtonID1;
	qApp->exit_loop();
}

//============================================
// Button3 event
//--------------------------------------------
void XBox::slotButton3 (void) {
	buttonStatus = mButtonID2;
	qApp->exit_loop();
}

//============================================
// Close event from WM
//--------------------------------------------
void XBox::closeEvent ( QCloseEvent* ) {
	if (mBtn3) {
		slotButton3();
	} 
	else if (mBtn2) {
		slotButton2();
	} 
	else {
		slotButton1();
	}
}

//============================================
// Show event
//--------------------------------------------
void XBox::showEvent ( QShowEvent* ) {
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
	} else { 
	move (
		(qApp->desktop()->width()  - w) / 2 - xo,
		(qApp->desktop()->height() - h) / 2 - yo
	);
	}

	int btnBarWidth = mBtn1 -> width() + 170;
	if (mBtn2) {
		btnBarWidth += mBtn2 -> width();
	}
	if (mBtn3) {
		btnBarWidth += mBtn3 -> width();
	}
	if (btnBarWidth > mLabel -> width()) {
		setMinimumWidth (btnBarWidth);
	} else {
		setMinimumWidth (400);
	}
	enterEvent (NULL);
	if (mBtn1->isDefault()) {
		mBtn1->setFocus();
	}
}

//=====================================
// XBox set input focus if enterd
//-------------------------------------
void XBox::enterEvent ( QEvent * ) {
	XSetInputFocus (
		x11Display(),winId(),RevertToParent,CurrentTime
	);
}

//=====================================
// XBox set global event filter
//-------------------------------------
bool XBox::eventFilter ( QObject* obj, QEvent* event ) {
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

//=====================================
// XBox disable button X
//-------------------------------------
void XBox::disableButton ( int btn ) {
	switch (btn) {
	case 0:
	if (mBtn1) {
		mBtn1->setDisabled (true);
	}
	break;
	case 1:
	if (mBtn2) {
		mBtn2->setDisabled (true);
	}
	break;
	case 2:
	if (mBtn3) {
		mBtn3->setDisabled (true);
	}
	break;
	}
}

//=====================================
// XBox enable button X
//-------------------------------------
void XBox::enableButton ( int btn ) {
	switch (btn) {
	case 0:
	if (mBtn1) {
		mBtn1->setDisabled (false);
	}
	break;
	case 1:
	if (mBtn2) {
		mBtn2->setDisabled (false);
	}
	break;
	case 2:
	if (mBtn3) {
		mBtn3->setDisabled (false);
	}
	break;
	}
}
