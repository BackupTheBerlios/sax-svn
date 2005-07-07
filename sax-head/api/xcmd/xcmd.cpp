/**************
FILE          : capi.cpp
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : commandline configuration tool for the X
              : window system released under the GPL license
              :
DESCRIPTION   : SaX2 commandline system using libsax to provide
              : configurations for the most important X11 capabilities
              : //.../
              : - capi.cpp: main program start sequence
              : - capi.h: buildins and defines
              : ----
              :
STATUS        : Status: Development
**************/
#include "xcmd.h"

int main (int argc,char*argv[]) {
	//=====================================
	// init variables...
	//-------------------------------------
	bool mAutoMode = false;
	bool mUpdate   = false;
	bool m3DStatus = false;

	//=====================================
	// init variables...
	//-------------------------------------
	int mDesktop    = 0;
	int mColorDepth = 0;

	//=====================================
	// start logging...
	//-------------------------------------
	logInit();

	//=====================================
	// set library debugging...
	//-------------------------------------
	SaXException().setDebug (false);

	//=====================================
	// allocate main qt object...
	//-------------------------------------
	QApplication SCCApp (argc,argv,false);

	//=====================================
	// get additional options...
	//-------------------------------------
	while (1) {
		int option_index = 0;
		static struct option long_options[] =
		{
			{"help"       , 0 , 0 , 'h'},
			{"desktop"    , 1 , 0 , 'D'},
			{"colordepth" , 1 , 0 , 'C'},
			{"status3D"   , 0 , 0 , 'S'},
			{"new"        , 0 , 0 , 'n'}
		};
		int c = getopt_long (
			argc, argv, "hC:nD:S",long_options, &option_index
		);
		if (c == -1) {
			break;
		}
		switch (c) {
			case 'h':
				logExit(); usage (0);
			break;
			case 'n':
				mAutoMode = true;
			break;
			case 'C':
				mColorDepth = atoi (optarg);
			break;
			case 'D':
				mDesktop = atoi (optarg);
			break;
			case 'S':
				m3DStatus = true;
			break;
			default:
				logExit(); usage (1);
			break;
		}
	}
	//=====================================
	// import configuration data...
	//-------------------------------------
	QDict<SaXImport> section;
	int importID[7] = {
		SAX_CARD,
		SAX_DESKTOP,
		SAX_POINTERS,
		SAX_KEYBOARD,
		SAX_LAYOUT,
		SAX_PATH,
		SAX_EXTENSIONS
	};
	SaXConfig* config = new SaXConfig;
	for (int id=0; id<7; id++) {
		SaXImport* import = new SaXImport ( importID[id] );
		import -> setSource ( SAX_SYSTEM_CONFIG );
		if (mAutoMode) {
			import -> setSource ( SAX_AUTO_PROBE );
		}
		import -> doImport();
		config -> addImport (import);
		section.insert (
			import->getSectionName(),import
		);
	}

	//=====================================
	// set default color depth...
	//-------------------------------------
	if ( mColorDepth ) {
		setDefaultColorDepth ( section,mDesktop,mColorDepth );
		mUpdate = true;
	}

	//=====================================
	// get 3D status information...
	//-------------------------------------
	if ( m3DStatus ) {
		bool status = get3DStatus ( section,mDesktop );
		if (status) {
			printf ("enabled\n");
			logExit(); exit (0);
		} else {
			printf ("disabled\n");
			logExit(); exit (1);
		}
	}

	//=====================================
	// create configuration...
	//-------------------------------------
	if ( ! mUpdate ) {
		logExit(); exit (0);
	}
	config -> setMode (SAX_NEW);
	if ( ! config -> createConfiguration() ) {
		log (L_ERROR, "SaX: Couldn't create configuration file: %s,%s\n",
			config->errorString().ascii(),
			config->getParseErrorValue().ascii()
		);
		logExit(); exit (1);
	}
	logExit();
	return 0;
}

//=====================================
// usage...
//-------------------------------------
void usage (int code) {
	// .../
	// usage information for options to call the capi tool
	// ----
	printf ("Linux SaX2 Version 7.1 command level (capi)\n");
	printf ("(C) Copyright 2005 SuSE GmbH\n");
	printf ("\n");
	printf ("usage: capi [ options ]\n");
	printf ("options:\n");
	printf ("[ -h | --help\n");
	printf ("   show this message and exit\n");
	printf ("--\n");
	exit (code);
}

