/**************
FILE          : xvirtual.c
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the XFree86 license
              :
DESCRIPTION   : xapi (X11-Application-Interface)
              : virtual area widget implementation
              :
              :
STATUS        : Status: Up-to-date
**************/
#include "xvirtual.h"

//====================================
// Constructor...
//------------------------------------
XVirtualArea::XVirtualArea ( int x,int y,QWidget* parent ) : QPushButton (
	parent 
) {
	mX = virtualX = x;
	mY = virtualY = y;
	setBackgroundMode( NoBackground );
	QBoxLayout*  layer1 = new QVBoxLayout ( this );
	QHBox* bottom = new QHBox ( this );
	mStatus = new QStatusBar  ( bottom );
	mPixmap = new QPixmap (PIXVIRTUAL);
	mBuffer = new QPixmap ();
	mStatus -> setSizeGripEnabled (false);
	layer1  -> addStretch (20);
	layer1  -> addWidget  (bottom);
	setMinimumWidth  ( mPixmap->width()  + 60 );
	setMinimumHeight ( mPixmap->height() + mStatus -> height() + 20 );
	setCursor(QCursor::sizeAllCursor);
	//mStatus -> message ( vtext );
	mParent = parent;
	QString vtext;
	mInfo = new QComboBox ( FALSE,this );
	mInfo -> setCursor(QCursor::arrowCursor);
	mInfo -> setFixedWidth (120);
	QObject::connect (
		mInfo     , SIGNAL ( activated    (int) ),
		this      , SLOT   ( slotManual   (int) )
	);
	QTextOStream(&vtext) << mX << "x" << mY;
	mInfo -> setDuplicatesEnabled (false);
	mInfo -> insertItem ( vtext );
	mInfo -> hide();

	//======================================
	// Setup standard resolution list...
	//--------------------------------------
	mResolution.append ( "640x480"   );
	mResolution.append ( "800x600"   );
	mResolution.append ( "1024x768"  );
	mResolution.append ( "1152x864"  );
	mResolution.append ( "1280x960"  );
	mResolution.append ( "1280x1024" );
	mResolution.append ( "1400x1050" );
	mResolution.append ( "1600x1000" );
	mResolution.append ( "1600x1024" );
	mResolution.append ( "1600x1200" );

	int  count = 0;
	long limes = mX * mY;
	int  startIndex = 10;
	QListIterator<char> it ( mResolution );
	for (; it.current(); ++it) {
		QStringList items = QStringList::split ("x", it.current() );
		int x = items[0].toInt();
		int y = items[1].toInt();
		long space = x * y;
		if (space > limes) {
			startIndex = count;
			break;
		}
		count++;
	}
	for (int n=startIndex;n<10;n++) {
		mInfo -> insertItem ( mResolution.at(n) );
	}
	mNOPaint = INFO_LEAVE_UNTOUCHED;
	mFirstInsert = true;
	paintEvent (0);
}

//====================================
// paintEvent...
//------------------------------------
void XVirtualArea::paintEvent( QPaintEvent* ) {
	QPainter pDC;
	QColor pColor = backgroundColor();
	//===================================
	// Init buffer and device context
	//-----------------------------------
	mBuffer -> resize ( width(),height() );
	pDC.begin( mBuffer );
	pDC.fillRect (
		0,0,width(),height(),QBrush(pColor)
	);
	//===================================
	// create the virtual pattern area
	//-----------------------------------
	//pDC.setPen (QPen(Qt::darkGreen));
	pDC.setPen (QPen(Qt::darkGray));
	int startx = mPixmap->width()  + 60; 
	int starty = mPixmap->height() + 30;
	int w = 2 * width();
	if (height() > width()) {
		w = 2 * height();
	}
	int y = starty;
	int x = startx;
	while ( 1 ) {
		pDC.drawLine (x,30,50,y);
		y+=10; x+=10;
		if ((x > w) || (y > w)) {
			break;
		}
	}
	//===================================
	// draw the pixmap
	//-----------------------------------
	pDC.drawPixmap ( 60,30,*mPixmap );

	//===================================
	// draw marker lines
	//-----------------------------------
	pDC.setPen (QPen(Qt::black));
	// Pixmap: vertical line
	pDC.drawLine (50,0,50,mPixmap->height()+30);
	pDC.drawLine (40,mPixmap->height()+30,60,mPixmap->height()+30);
	// Pixmap: horizontal line
	pDC.drawLine (30,20,mPixmap->width()+30,20);
	pDC.drawLine (mPixmap->width()+60,10,mPixmap->width()+60,30);
	// Rest: vertical line
	pDC.drawLine (50,mPixmap->height()+30,50,height());
	pDC.drawLine (
		30,height()-mStatus->height()-1,
		70,height()-mStatus->height()-1
	);
	// Rest: horizontal line
	pDC.drawLine (mPixmap->width()+30,20,width(),20);
	pDC.drawLine (width()-1,0,width()-1,40);

	//===================================
	// draw coordinates
	//-----------------------------------
	QString sX; sX.sprintf("%d",mX);
	QString sY; sY.sprintf("%d",mY);
	pDC.drawText (60,15,sX);
	pDC.drawText (10,35,sY);

	int dx = width() - mPixmap->width() - 60;
	int dy = height() - mPixmap->height() - mStatus -> height() - 31;
	int vX = (int)(((float)mX / (float)mPixmap->width())  * dx);
	int vY = (int)(((float)mY / (float)mPixmap->height()) * dy);

	sX.sprintf("%d",vX);
	sY.sprintf("%d",vY);
	pDC.drawText (mPixmap->width()+70,15,sX);
	pDC.drawText (10,mPixmap->height()+45,sY);

	vX = mX + vX;
	vY = mY + vY;
	QString vtext;
	QTextOStream(&vtext) << vX << "x" << vY;
	switch (mNOPaint) {
	case INFO_CHANGE:
		mInfo -> changeItem ( vtext,0 );
	break;
	default:
	break;
	}

	//===================================
	// draw the buffer
	//-----------------------------------
	bitBlt( this, 0, 0, mBuffer );
	pDC.end();

	mInfo -> move (
		(width()  + 60 - mInfo->width())  / 2,
		(height() + 30 + mStatus->height() - mInfo->height()) / 2
	);
	mInfo -> show();
	if (mNOPaint == INFO_CHANGE) {
		virtualX = vX;
		virtualY = vY;
	}
}

//====================================
// mouse moved...
//------------------------------------
void XVirtualArea::mouseMoveEvent ( QMouseEvent* m ) {
	int dx = m->x() - mStartx;
	int dy = m->y() - mStarty;
	if (mWidth + dx >= mParent->width()) {
		return;
	}
	if (mHeight + dy >= mParent->height()) {
		return;
	}
	resize (mWidth + dx,mHeight + dy);
	mInfo -> move (
		(width()  + 60 - mInfo->width())  / 2,
		(height() + 30 + mStatus->height() - mInfo->height()) / 2
	);
}

//====================================
// mouse pressed...
//------------------------------------
void XVirtualArea::mousePressEvent ( QMouseEvent* m ) {
	mStartx = m->x();
	mStarty = m->y();
	mWidth  = width();
	mHeight = height();
	if (! mFirstInsert) {
		mInfo -> removeItem ( 0 );
	}
	mNOPaint = INFO_CHANGE;
	mInfo -> insertItem ( "1x1",0 );
	mInfo -> setCurrentItem (0);
	mInfo -> show();
	mFirstInsert = false;
	repaint();
}

//====================================
// mouse released...
//------------------------------------
void XVirtualArea::mouseReleaseEvent ( QMouseEvent* ) {
	mNOPaint = INFO_LEAVE_UNTOUCHED;
}

//====================================
// set text...
//------------------------------------
void XVirtualArea::setText ( const QString & text ) {
	mText = text;
}

//====================================
// set virtual resolution as member...
//------------------------------------
void XVirtualArea::setVirtual ( const QString & res ) {
	QStringList items = QStringList::split ("x", res );
	int x = items[0].toInt();
	int y = items[1].toInt();

	int w = (int)((mPixmap->width() / (float)mX) * x) + 61;
	int h = (int)((mPixmap->height()/ (float)mY) * y) + mStatus->height() + 30;
	mNOPaint = INFO_LEAVE_UNTOUCHED;
	QString vtext;
	QTextOStream(&vtext) << x << "x" << y;
	if (mFirstInsert) {
		mInfo -> insertItem ( vtext,0 );
	} else {
		mInfo -> changeItem ( vtext,0 );
	}
	resize ( w,h );
	virtualX = x;
	virtualY = y;
}

//====================================
// manual selection...
//------------------------------------
void XVirtualArea::slotManual ( int id ) {
	QString item = mInfo->text (id);
	QStringList items = QStringList::split ("x", item );
	int x = items[0].toInt();
	int y = items[1].toInt();

	int w = (int)((mPixmap->width() / (float)mX) * x) + 61;
	int h = (int)((mPixmap->height()/ (float)mY) * y) + mStatus->height() + 30;
	mNOPaint = INFO_LEAVE_UNTOUCHED;
	resize ( w,h );
	virtualX = x;
	virtualY = y;
}

//====================================
// get virtual resolution...
//------------------------------------
QPoint XVirtualArea::getVirtualResolution ( void ) {
	QPoint p (virtualX,virtualY);
	return (p);
}

//====================================
// check if virtual res. differs...
//------------------------------------
bool XVirtualArea::hasChanged ( void ) {
	if ((mX != virtualX) || (mY != virtualY)) {
		return (true);
	}
	return (false);
}

//====================================
// check if real equals virtual...
//------------------------------------
bool XVirtualArea::virtualEqualsReal ( void ) {
	if ((mX == virtualX) && (mY == virtualY)) {
		return (true);
	}
	return (false);
}
