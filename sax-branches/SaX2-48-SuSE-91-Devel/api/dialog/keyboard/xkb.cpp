/**************
FILE          : xkb.c
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the XFree86 license
              :
DESCRIPTION   : XKB helper application to create the
              : gettext used translation keys
              :
              :
STATUS        : Status: Up-to-date
**************/
#include <stdio.h>
#include <stdlib.h>

#include "rules.h"

int main (void) {
	XKeyRules rules;
	//=================================
	// Layout strings
	//---------------------------------
	QDict<char> l = rules.getLayouts();
	QDictIterator<char> n (l);
	for (; n.current(); ++n) {
		printf("gettext(\"%s\")\n",n.current());
	}

	//=================================
	// Model strings
	//---------------------------------
	QDict<char> m = rules.getModels();
	QDictIterator<char> nn (m);
    for (; nn.current(); ++nn) {
        printf("gettext(\"%s\")\n",nn.current());
    }

	//=================================
	// Option strings
	//---------------------------------	
	QDict<char> o = rules.getOptions();
	QDictIterator<char> nnn (o);
    for (; nnn.current(); ++nnn) {
        printf("gettext(\"%s\")\n",nnn.current());
    }
	exit (0);
}
