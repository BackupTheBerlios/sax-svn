/**************
FILE          : keyboard.c
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the GPL license
              :
DESCRIPTION   : keyboard detection code 
              : 
              :
STATUS        : Status: Up-to-date
**************/

#include "keyboard.h"
#include "config.h"

//======================================
// Definitions...
//--------------------------------------
#define KEYTABLE  "KEYTABLE"

//======================================
// ScanKeyboard: constructor...
//--------------------------------------
ScanKeyboard::ScanKeyboard(str keymap) {
	kmap = keymap;
}

//======================================
// ScanKeyboard: set file for saving
//--------------------------------------
void ScanKeyboard::SetFile(str name) {
	file = name;
}

//======================================
// ScanKeyboard: get the keyboard data
//--------------------------------------
Keymap ScanKeyboard::Pop (void) {
	return(kp); 
}

//======================================
// ScanKeyboard: hw scan of keyboard
//--------------------------------------
void ScanKeyboard::Scan (void) {
	KbdData* kbd = KeyboardGetData();
	SPReadFile<RcData> rcdata;
	SPReadFile<Keymap> rckeymap;
	RcData config;
	Keymap keymap;
 
	rcdata.SetFile(RC_SYSCONFIG,"keyboard");
	rckeymap.SetFile(KBD_MAP);

	config = rcdata.ImportRcConfig();
	rckeymap.ImportKeymap();

	// ...
	// get machine architecture...
	// ------------------------------
	string arch = qx("/bin/arch",STDOUT);
	// ...
	// save data from hw scan...
	// --------------------------
	kp.variant = "";
	if (strstr((char*)arch.c_str(), ARCH_SPARC) != NULL) {
		kp.model  = kbd->model;
		kp.layout = kbd->layout;
	} else {
		kp.model  = "undef";
		kp.layout = "undef";
	}
	// ...
	// check KEYTABLE variable...
	// ---------------------------
	string check = qx(CHECKMAP,STDOUT,1,"%s",config[KEYTABLE].c_str());
	config[KEYTABLE] = check;

	for (int i = rckeymap.Count(); i > 0; i--) {
	keymap = rckeymap.Pop();
	if (keymap.consolename == config[KEYTABLE]) {
		// found console keymap adapter...
		// -----------------------------------
		if (kp.model  == "undef") {
			kp.model  = keymap.model;
		}
		if (kp.layout == "undef") {
			kp.layout = keymap.layout;
		}
		kp.variant     = keymap.variant;
		kp.consolename = config[KEYTABLE]; 
		kp.keycodes    = keymap.keycodes;
		kp.leftalt     = keymap.leftalt;
		kp.rightalt    = keymap.rightalt;
		kp.scrollock   = keymap.scrollock;
		kp.rightctl    = keymap.rightctl;
		kp.options     = keymap.options;
		break;
	}
	}
	// ...
	// handle special cases...
	// ------------------------
	// 1)
	// PPC protocol detection via script
	// ----------------------------------
	if (arch == ARCH_PPC) {
	string model = qx(MACHINE,STDOUT); 
	if (model != "") {
		kp.model = model;
	}
	}
}

//======================================
// ScanKeyboard: save data to file
//--------------------------------------
int ScanKeyboard::Save (void) {
	ofstream handle(file.c_str(),ios::binary);
	Keymap_S save; 
	if (! handle) {
		cout << "ScanKeyboard: could not create file: ";
		cout << file << endl;
		return(-1);
	}
	// reformat to primitive types...
	// -------------------------------
	strcpy(save.consolename , kp.consolename.c_str());
	strcpy(save.model       , kp.model.c_str());
	strcpy(save.layout      , kp.layout.c_str());
	strcpy(save.variant     , kp.variant.c_str());
	strcpy(save.options     , kp.options.c_str());
	strcpy(save.keycodes    , kp.keycodes.c_str());
	strcpy(save.leftalt     , kp.leftalt.c_str());
	strcpy(save.rightalt    , kp.rightalt.c_str());
	strcpy(save.scrollock   , kp.scrollock.c_str());
	strcpy(save.rightctl    , kp.rightctl.c_str());

	handle.write((char*)&save,sizeof(save));
	handle.close();
	return(0);
}

//======================================
// ScanKeyboard: retrieve data from file
//--------------------------------------
int ScanKeyboard::Read (void) {
	ifstream handle(file.c_str(),ios::binary);
	Keymap_S input;
	if (! handle) {
		//cout << "ScanKeyboard: could not open file: ";
		//cout << file << endl;
		return(-1);
	}
	handle.read((char*)&input,sizeof(input));

	// save to object data
	// ---------------------
	kp.consolename = input.consolename;
	kp.model       = input.model;
	kp.layout      = input.layout;
	kp.variant     = input.variant;
	kp.options     = input.options;
	kp.keycodes    = input.keycodes;
	kp.leftalt     = input.leftalt;
	kp.rightalt    = input.rightalt;
	kp.scrollock   = input.scrollock;
	kp.rightctl    = input.rightctl;

	handle.close();
	return(0);
}
