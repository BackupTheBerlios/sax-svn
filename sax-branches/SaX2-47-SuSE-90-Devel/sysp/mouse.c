/**************
FILE          : mouse.cc
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the XFree86 license
              :
DESCRIPTION   : mouse detection code 
              : 
              :
STATUS        : Status: Up-to-date
**************/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "mouse.h"

//======================================
// ScanMouse: constructor...
//--------------------------------------
ScanMouse::ScanMouse(void) {
	elements = 0;
	current  = 0;
}

//======================================
// ScanMouse: set file name to save
//--------------------------------------
void ScanMouse::SetFile(str name) {
	file = name;
}

//======================================
// ScanMouse: push MouseData to map
//--------------------------------------
void ScanMouse::Push (MouseData e) {
	qM[elements] = e;
	elements++;
}

//======================================
// ScanMouse: read MouseData from map
//--------------------------------------
MouseData ScanMouse::Pop (void) {
	if (current >= elements) {
		throw ReadLastElement();
	}
	MouseData element(qM[current]);
	current++; 
	return element;
}

//======================================
// ScanMouse: reset map counter...
//--------------------------------------
void ScanMouse::Reset (void) {
	current = 0;
}

//======================================
// ScanMouse: hw scan of the mice
//--------------------------------------
void ScanMouse::Scan (void) {
	int mcount = 0;     // count number of detected devices
	int count  = 0;     // current mouse count
	#if 0
	int USB    = 0;     // USB container device descriptor
	#endif
	str link   = "";    // result link name used for symlink()
	int USBGeneric = 1; // decide to add a generic USB mouse section
	mp = MouseGetData();
	// ...
	// first count number of mouse devices and check if
	// a USB mouse was detected
	// ---
	for (MouseData* lp=mp; lp; lp=lp->next) {
	mcount++;
	if (strstr(lp->device,"/dev/input")) {
		USBGeneric = 0;
	}
	}
	// ...
	// if no USB mouse was detected but the USB subsystem was
	// loaded we will add a generic USB device to enable hot plug
	// USB devices
	// ---
	#if 0
	if (USBGeneric) {
	USB = open ("/proc/bus/usb/drivers",O_RDONLY|O_NONBLOCK);
	if (USB != -1) {
		close (USB);
		MouseData* usbGeneric = NULL;
		usbGeneric = (MouseData*)malloc(sizeof(MouseData));
		strcpy (usbGeneric->protocol,"imps/2");
		strcpy (usbGeneric->device  ,"/dev/input/mice");
		usbGeneric->emulate = 0;
		usbGeneric->buttons = 3;
		usbGeneric->wheel   = 1;
		Push (*usbGeneric);
	}
	}
	#endif
	// ...
	// add detected devices now
	// ---
	for (MouseData* lp=mp; lp; lp=lp->next) {
		if (mcount > 1) {
			// more than one mouse detected...
			sprintf (link,"/dev/pointer%d",count);
			count++;
		} else {
			// only one mouse detected...
			sprintf (link,"/dev/mouse");
		}
		unlink  (link);
		symlink (lp->device,link);
		strcpy  (lp->device,link);
		Push(*lp);
	}
	// ...
	// if no mouse could be detected we will add the generic
	// AUTO protocol entry
	// ---
	if (mp == NULL) {
		MouseData* define = NULL;
		define = (MouseData*)malloc(sizeof(MouseData));
		strcpy(define->protocol,"Auto");
		strcpy(define->device  ,"/dev/mouse");
		define->emulate = 1;
		define->buttons = 2;
		define->wheel   = 0;
		Push (*define);
	}
}

//======================================
// ScanMouse: serialize data to file
//--------------------------------------
int ScanMouse::Save (void) {
	MouseData data;
	ofstream handle(file.c_str(),ios::binary);
	if (! handle) {
		cout << "ScanMouse: could not create file: ";
		cout << file << endl;
		return(-1);
	}
	Reset();
	for (int i = Count(); i > 0; i--) {
		data = Pop();
		handle.write((char*)&data,sizeof(data));
	}
	handle.close();
	return(0); 
}

//======================================
// ScanMouse: retrieve data from file
//--------------------------------------
int ScanMouse::Read (void) {
	MouseData data;
	ifstream handle(file.c_str(),ios::binary);
	if (! handle) {
		//cout << "ScanMouse: could not open file: "
		//cout << file << endl;
		return(-1);
	}
	elements = 0;
	current  = 0;
	qM.clear();

	while(1) {
		handle.read((char*)&data,sizeof(data));
		if (handle.eof()) {
			break;
		}
		Push(data);
	}
	handle.close();
	return(0);
}
