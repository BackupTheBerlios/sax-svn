/**************
FILE          : keyboard.h
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the XFree86 license
              :
DESCRIPTION   : header file for the keyboard detection  
              : 
              :
STATUS        : Status: Up-to-date
**************/
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <iosfwd>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <queue>
#include <map>
#include <unistd.h>
#include <stdio.h>
#include <algorithm>

using namespace std;

#include "lib/syslib.h"
#include "sysp.h"

//=====================================
// Class Headers
//-------------------------------------
class ScanKeyboard {
	protected:
	Keymap kp;
	string file;
	string kmap;
 
	public:
	void SetFile (str name);
	void Scan (void);
	int  Save (void);
	int  Read (void);
	ScanKeyboard (str keymap);
	Keymap Pop (void);
};
