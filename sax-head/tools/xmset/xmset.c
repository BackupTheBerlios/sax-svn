/**************
FILE          : xmset.c
***************
PROJECT       : SaX ( SuSE advanced X configuration )
              :
BELONGS TO    : Configuration tool X11 version 4.0
              :  
              :
BESCHREIBUNG  : xmset change the mouse properties on the
              : fly of a running X-Server
              : 
              :
STATUS        : Status: Up-to-date
**************/
#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <X11/StringDefs.h>
#include <X11/Xatom.h>
#include <X11/Xmd.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <X11/XKBlib.h>
#include <X11/extensions/XKBgeom.h>
#include <X11/extensions/XKM.h>
#include <X11/extensions/XKBfile.h>
#include <X11/extensions/XKBui.h>
#include <X11/extensions/XKBrules.h>
#include <X11/Xproto.h>
#include <X11/Xfuncs.h>
#include <X11/extensions/xf86misc.h>
#include <X11/Xos.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/kdev_t.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>

#include "../whois/remote.h"
#include "hwdata.h"

//========================================
// Structs...
//----------------------------------------
struct mouse {
	char link[256];
	char device[256];
	char type[256];
	int  baudrate;
	int  samplerate;
	int  resolution;
	int  buttons;
	char emu3button[256];
	int  emu3timeout;
	char chordmiddle[256];
	char flag[256];
};

struct devnode {
	char  name[256];    /* name of the device file */
	int   major;        /* major number            */
	int   minor;        /* minor number            */
	dev_t device;       /* device struct for mknod */
	int   mode;         /* char(0) or block(1) dev */
	uid_t user;         /* user id                 */
	gid_t group;        /* group id                */
	mode_t protect;     /* protection mode         */
};

static char *msetable[] = { 
	"None", 
	"Microsoft", 
	"MouseSystems", 
	"MMSeries",
	"Logitech", 
	"BusMouse", 
	"Mouseman", 
	"PS/2",
	"MMHitTab", 
	"GlidePoint", 
	"IntelliMouse",
	"ThinkingMouse", 
	"IMPS/2", 
	"ThinkingMousePS/2",
	"MouseManPlusPS/2", 
	"GlidePointPS/2", 
	"NetMousePS/2", 
	"NetScrollPS/2", 
	"SysMouse",
	"Auto", 
	"AceCad",
	"ExplorerPS/2",
	"Xqueue",
	"OSMouse" 
};

//========================================
// Defines...
//----------------------------------------
#define BUFSIZE      128
#define MOUSELINK    "/dev/mouse"
#define MSETABLESIZE (sizeof(msetable)/sizeof(char *))

//========================================
// Functions...
//----------------------------------------
int strsplit(str string,char deli,str left,str right);
int StrCaseCmp(char* s1, char* s2);
int SetMouse(Display *dpy, struct mouse* argv);
int IsRoot(void);
int catchErrors (Display *dpy, XErrorEvent *ev);
XF86MiscMouseSettings GetMouse(Display *dpy);
Display* XOpen(str name);
void showMouseConfig (void);
void showMonitorConfig (void);
void usage(void);

//================================================
// special string comparison Note, the characters 
// '_', ' ', and '\t' are ignored 
//------------------------------------------------
int StrCaseCmp(char* s1, char* s2) {
	char c1, c2;
	if (*s1 == 0) {
	if (*s2 == 0) {
		return(0);
	} else {
		return(1);
	}
	}
	while (*s1 == '_' || *s1 == ' ' || *s1 == '\t') {
		s1++;
	}
	while (*s2 == '_' || *s2 == ' ' || *s2 == '\t') {
		s2++;
	}
	c1 = (isupper(*s1) ? tolower(*s1) : *s1);
	c2 = (isupper(*s2) ? tolower(*s2) : *s2);
	while (c1 == c2) {
	if (c1 == '\0') {
		return(0);
	}
	s1++; s2++;
	while (*s1 == '_' || *s1 == ' ' || *s1 == '\t') {
		s1++;
	}
	while (*s2 == '_' || *s2 == ' ' || *s2 == '\t') {
		s2++;
	}
	c1 = (isupper(*s1) ? tolower(*s1) : *s1);
	c2 = (isupper(*s2) ? tolower(*s2) : *s2);
	}
	return(c1 - c2);
}


//======================================
// Implement mouse apply                                         
// Usage: 
// SetMouse ( 
//  display       -> display
//  device        -> device 
//  mousetype     -> Protocol
//  baudrate      -> Baudrate
//  samplerate    -> Samplerate
//  resolution    -> Resolution 
//  buttons       -> Buttons
//  on|off        -> Emulate3Buttons
//  timeout       -> Emulate3Timeout
//  on|off        -> ChordMiddle
//   
//  you may set one of this flags
//  -----------------------------
//  [ClearDTR]    -> Flag
//  [ClearRTS]    -> Flag
//  [ReOpen]      -> apply immediatelly 
// )
//----------------------------------------
int SetMouse(Display *dpy, struct mouse* argv) {
	XF86MiscMouseSettings mseinfo;
	unsigned i;

	mseinfo.device = argv->device;
	mseinfo.type   = -1;
	for (i = 1; i < sizeof(msetable)/sizeof(char *); i++) {
	if (!StrCaseCmp(msetable[i], argv->type)) {
		mseinfo.type = i - 1;
	}
	}

	/* some special cases... */
	if (!StrCaseCmp("Xqueue", argv->type)) {
		mseinfo.type = MTYPE_XQUEUE;
	} else if (!StrCaseCmp("OSMouse", argv->type)) {
		mseinfo.type = MTYPE_OSMOUSE;
	}
	if (mseinfo.type == -1) {
		return (-1);
	}

	mseinfo.baudrate   = argv->baudrate;
	mseinfo.samplerate = argv->samplerate;
	mseinfo.resolution = argv->resolution;
	if (argv->buttons != 0) {
		mseinfo.buttons    = argv->buttons;
	}
	if (!StrCaseCmp(argv->emu3button, "on")) {
		mseinfo.emulate3buttons = 1;
	} else {
		mseinfo.emulate3buttons = 0;
	}
	mseinfo.emulate3timeout = argv->emu3timeout;

	if (!StrCaseCmp(argv->chordmiddle, "on")) {
		mseinfo.chordmiddle = 1;
	} else {
		mseinfo.chordmiddle = 0;
	}

	mseinfo.flags = 0;
	if (!StrCaseCmp(argv->flag,        "cleardtr")) {
		mseinfo.flags |= MF_CLEAR_DTR;
	} else if (!StrCaseCmp(argv->flag, "clearrts")) {
		mseinfo.flags |= MF_CLEAR_RTS;
	} else if (!StrCaseCmp(argv->flag, "reopen")) {
		mseinfo.flags |= MF_REOPEN;
	} else {
		mseinfo.flags |= MF_REOPEN; 
	}

	XSync (dpy, False);
	XF86MiscSetMouseSettings (dpy, &mseinfo);
	XSync (dpy, False);
	return (0);
}

//========================================
// GetMouse...
//----------------------------------------
XF86MiscMouseSettings GetMouse(Display *dpy) {
	XF86MiscMouseSettings mseinfo;
	char tmpbuf[200] = {0};

	if (!XF86MiscGetMouseSettings(dpy, &mseinfo)) {
		return(mseinfo);
	} else {
		char *name;
		if (mseinfo.type == MTYPE_XQUEUE) {
			name = "Xqueue";
		} else if (mseinfo.type == MTYPE_OSMOUSE) {
			name = "OSMouse";
		} else if ( 
			(mseinfo.type < 0) || ((unsigned)mseinfo.type >= MSETABLESIZE)
		) {
			name = "Unknown";
		} else {
			name = msetable[mseinfo.type+1];
		}
		snprintf (
			tmpbuf, sizeof(tmpbuf), "%s %s %d %d %d %d %s %d %s",
			mseinfo.device==NULL? "{}": mseinfo.device, 
			name,
			mseinfo.baudrate, 
			mseinfo.samplerate,
			mseinfo.resolution, 
			mseinfo.buttons,
			mseinfo.emulate3buttons? "on": "off",
			mseinfo.emulate3timeout,
			mseinfo.chordmiddle? "on": "off"
		);
		if (mseinfo.flags & MF_CLEAR_DTR) {
			strncat(tmpbuf, " ClearDTR", sizeof(tmpbuf)-strlen(tmpbuf)-1);
		}
		if (mseinfo.flags & MF_CLEAR_RTS) {
			strncat(tmpbuf, " ClearRTS", sizeof(tmpbuf)-strlen(tmpbuf)-1);
		}
		if (mseinfo.device) {
			XFree(mseinfo.device);
		}
		printf("%s\n",tmpbuf);
		return(mseinfo);
	}
}

//========================================
// XOpen...
//----------------------------------------
Display* XOpen(str name) {
	Display *dpy;
	char display[256] = "";
 
	if (strcmp(name,"null") == 0) {
	if (getenv("DISPLAY") != NULL) {
		strcpy(display,getenv("DISPLAY"));
	}
	} else {
		strcpy(display,name);
	}
	if (remoteDisplay (display)) {
		exit (0);
	}
	if (!(dpy = XOpenDisplay(display))) {
		/*fprintf(stderr, "unable to open display: %s\n",getenv("DISPLAY"));*/
		return(NULL);
	}
	return(dpy);
}

//========================================
// check for root privilege...
//----------------------------------------
int IsRoot(void) {
	if (getuid() != 0) { return(0); }
	return(1);
}

//========================================
// usage...
//----------------------------------------
void usage(void) {
	printf("usage: xmset options\n");
	printf("options:\n");
	printf("  [ -d | --display ]\n");
	printf("     set display to use for operation\n");
	printf("  [ -s | --show    ]\n");
	printf("     show current mouse configuration on\n"); 
	printf("     actual display\n");
	printf("  [ -a | --apply   ]\n");
	printf("     apply new mouse configuration. using the\n"); 
	printf("     following list format, field seperator is \":\"\n");
	printf("     --\n");
	printf("     1)  RealDevice\n");
	printf("     2)  Protocol\n");
	printf("     3)  Baudrate\n"); 
	printf("     4)  Samplerate\n");
	printf("     5)  Resolution\n");
	printf("     6)  Buttons\n");
	printf("     7)  Emulate3Buttons  (on|off)\n");
	printf("     8)  Emulate3Timeout\n");
	printf("     9)  ChordMiddle      (on|off)\n");
	printf("     10) reopen\n");
	printf("\n");
	printf("  [ -D | --disable ]\n");
	printf("     disable the mouse pointer\n");
	printf("  [ -E | --enable  ]\n");
	printf("     enable the mouse pointer after it was\n");
	printf("     disabled with the option --disable\n");
	printf("  [ -v | --version ]\n");
	printf("     print the major:minor version information\n");
	printf("     of the used XF86Misc extension\n");
	exit(0);
}

//========================================
// main...
//----------------------------------------
int main (int argc, char*argv[]) {
	XF86MiscMouseSettings mseinfo;
	Display* dpy;
	int    c;
	int    DEBUG   = False;
	int    show    = False;
	int    Version = False;
	int    apply   = False;
	int    enable  = False;
	int    disable = False;
	int    major   = 0;
	int    minor   = 0;
	char   link    [256] = "";
	char   mydisp  [256] = "";
	char   option  [256] = "";
	char   left    [256] = "";
	struct mouse   *pointer;
	struct devnode *dev;
	struct stat    *linkstatus = NULL;

	pointer = (struct mouse*)   malloc(sizeof(struct mouse));
	dev     = (struct devnode*) malloc(sizeof(struct devnode));

	// get commandline options...
	//-----------------------------
	while (1) {
	int option_index = 0;
	static struct option long_options[] =
	{
		{"show"     , 0 , 0 , 's'},
		{"apply"    , 1 , 0 , 'a'},
		{"display"  , 1 , 0 , 'd'},
		{"help"     , 0 , 0 , 'h'},
		{"link"     , 1 , 0 , 'l'},
		{"enable"   , 0 , 0 , 'E'},
		{"disable"  , 0 , 0 , 'D'},
		{"version"  , 0 , 0 , 'v'},
		{"config"   , 0 , 0 , 'c'},
		{"monitor"  , 0 , 0 , 'm'},
		{0          , 0 , 0 , 0  }
	};

	c = getopt_long (argc, argv, "hsa:d:l:EDvcm",long_options, &option_index);
	if (c == -1)
	break;

	switch (c) {
	case 0:
		if (DEBUG) {
			fprintf (stderr,"option %s", long_options[option_index].name);
		if (optarg)
			fprintf (stderr," with arg %s", optarg);
			fprintf (stderr,"\n");
		}
	break;

	case 'h':
		usage();
	break;

	case 'c':
		showMouseConfig();
	break;	

	case 'm':
		showMonitorConfig();
	break;

	case 's':
		show  = True;
	break;

	case 'v':
		Version = True;
	break;

	case 'a':
		if (! IsRoot()) {
			fprintf(stderr,"xmset: only root can do this\n");
			exit(1);
		}
		apply = True;
		strcpy(option,optarg);
	break;

    case 'l':
		// no longer used because we will get the
		// the device name (which may be a link) from the X-Server 
		// and check if it is realy a link to the pointer device. 
		// If not, no symlink is created after apply 
		// ---
		// strcpy(link,optarg);
	break;

	case 'd':
		strcpy(mydisp,optarg);
	break;

	case 'E':
		if (! IsRoot()) {
			fprintf(stderr,"xmset: only root can do this\n");
			exit(1);
		}
		enable  = True;
	break;

	case 'D':
		if (! IsRoot()) {
			fprintf(stderr,"xmset: only root can do this\n");
			exit(1);
		}
		disable = True;
		break;

	default:
		exit(1);
	}
	}
	// ...
	// open display...
	// ---
	if (strcmp(mydisp,"") == 0) {
		strcpy(mydisp,"null");
	}
	dpy = XOpen(mydisp);
	if (dpy == NULL) {
	if (Version) {
		printf("0:6\n");
	} else {
		fprintf(stderr,"xmset: could not open display\n");
	}
	exit(1);
	}
	if (Version) {
	if (XF86MiscQueryVersion(dpy,&major,&minor)) {
		printf("%d:%d\n",major,minor);
		exit (0);
	}
	}
	// ...
	// store link name if the device specification
	// of the current configuration is a link
	// ---
	if (!XF86MiscGetMouseSettings(dpy, &mseinfo)) {
		fprintf(stderr,"xmset: could not get mouse settings\n");
		exit(1);
	}
	if (apply == True) {
		linkstatus = (struct stat*)malloc(sizeof(struct stat));
		lstat (mseinfo.device,linkstatus);
		if (S_ISLNK (linkstatus->st_mode)) {
			strcpy (link,mseinfo.device);
		}
	}

	//=================================
	// enable mouse
	//---------------------------------
	if (enable == True)  {
		XF86MiscMouseSettings mseinfo;
		if (!XF86MiscGetMouseSettings(dpy, &mseinfo)) {
			exit(1);
		}
		mseinfo.flags |= MF_REOPEN;
		XSetErrorHandler (catchErrors);
		XF86MiscSetMouseSettings(dpy, &mseinfo);
		XSync(dpy, False);
		exit(0);
	}

	//=================================
	// disable mouse
	//---------------------------------
	if (disable == True) {
		XF86MiscMouseSettings mseinfo;
		if (!XF86MiscGetMouseSettings(dpy, &mseinfo)) {
			exit(1);
		}
		mseinfo.flags |= MF_REOPEN;
		mseinfo.device = "/dev/unused";
		XSetErrorHandler (catchErrors);
		XF86MiscSetMouseSettings(dpy, &mseinfo);
		XSync(dpy, False);
		exit(0);
	}

 
	//==================================
	// apply mouse settings
	//----------------------------------
	if (apply == True) {
		XF86MiscMouseSettings mseinfo;
		strsplit (option,':',left,option);  
		strcpy (pointer->device,left);
		strsplit (option,':',left,option);
		strcpy (pointer->type,left);
		strsplit (option,':',left,option);
		pointer->baudrate = atoi(left);
		strsplit (option,':',left,option);
		pointer->samplerate = atoi(left);
		strsplit (option,':',left,option);
		pointer->resolution = atoi(left);
		strsplit (option,':',left,option);
		pointer->buttons = atoi(left);
		strsplit (option,':',left,option);
		if (strcmp(left,"on") == 0) {
			strcpy(pointer->emu3button,"on");
		} else {
			strcpy(pointer->emu3button,"off");
		}
		strsplit(option,':',left,option);
		pointer->emu3timeout = atoi(left);
		strsplit(option,':',left,option);
		if (strcmp(left,"on") == 0) {
			strcpy(pointer->chordmiddle,"on");
		} else {
			strcpy(pointer->chordmiddle,"off");
		}
		strsplit (option,':',left,option);
		strcpy (pointer->flag,left);
		// ...
		// relink the /dev/sax link if the configuration
		// use a device link to point to the input device
		// ---
		if (S_ISLNK (linkstatus->st_mode)) {
			remove (link);
			symlink (pointer->device,link);
		}
		// ...
		// remove mouse from currently 
		// accessed device
		// ---
		if (!XF86MiscGetMouseSettings(dpy, &mseinfo)) {
			exit(1);
		}
		mseinfo.flags |= MF_REOPEN;
		sprintf(mseinfo.device,"disabled");
		XSetErrorHandler (catchErrors);
		XF86MiscSetMouseSettings(dpy, &mseinfo);
		XSync(dpy, False);
		// ...
		// set new mouse parameters...
		// ---
		usleep (1000);
		SetMouse (dpy,pointer);
	}

	//==================================
	// show current mouse settings
	//----------------------------------
	if (show == True) {
		GetMouse(dpy);
	}
	return(0);
}

//========================================
// catchErrors (X11)...
//----------------------------------------
int catchErrors (Display *dpy, XErrorEvent *ev) {
	return (0);
}

//========================================
// show monitor configuration...
//----------------------------------------
void showMonitorConfig (void) {
	MsgDetect display = MonitorGetData();
	if ( ((display.dpix > 0) && (display.dpix < 100)) && 
		 ((display.dpiy > 0) && (display.dpiy < 100))
	) {
		int x = display.dpix * 10;
		int y = display.dpiy * 10;
		printf ("DisplaySize  %d %d",x,y);
	}
	exit (0);
}

//========================================
// show mouse configuration...
//----------------------------------------
void showMouseConfig (void) {
	MouseData* lp = NULL;
	MouseData* mp = MouseGetData();
	int haveMouse = 0;

	int count = 1;
	for (lp=mp; lp; lp=lp->next) {
		printf("Section \"InputDevice\"\n");
		printf("  Driver     \"mouse\"\n");
		printf("  Identifier \"Mouse[%d]\"\n",count);
		if (
			(strstr(lp->name,"Tablet") != NULL) ||
			(strstr(lp->name,"tablet") != NULL) ||
			(strstr(lp->name,"Intuos") != NULL)
		) {
			// TODO...
			// Tablet detected shouldn't be handled as mouse
			continue;
		}
		if (lp->protocol) {
			printf("  Option     \"Protocol\" \"%s\"\n",lp->protocol);
		}
		if (lp->device) {
			printf("  Option     \"Device\"   \"%s\"\n",lp->device);
		}
		if ((lp->wheel) && (lp->wheel >= 1)) {
			int btns = 5;
			if ((lp->buttons) && (lp->buttons > 5)) {
				printf("  Option     \"Buttons\" \"%d\"\n",lp->buttons);
			}
			printf("  Option     \"ZAxisMapping\" \"%d %d\"\n",
				btns - 1,btns
			);
		}
		if ((lp->buttons) && (lp->buttons == 4)) {
		if ((lp->wheel) && (lp->wheel <= 0)) {
			printf("  Option     \"Buttons\" \"%d\"\n",lp->buttons);
			printf("  Option     \"EmulateWheel\" \"on\"\n");
			printf("  Option     \"EmulateWheelButton\" \"4\"\n");
		}
		}
		printf("EndSection\n\n");
		haveMouse = 1;
		count += 2;
		//break;
	}
	if (! haveMouse) {
		printf("Section \"InputDevice\"\n");
		printf("  Driver     \"mouse\"\n");
		printf("  Identifier \"Mouse[1]\"\n");
		printf("  Option     \"Protocol\" \"Auto\"\n");
		printf("  Option     \"Device\"   \"/dev/input/mice\"\n");
		printf("EndSection\n");
	}
	exit (0);
}
