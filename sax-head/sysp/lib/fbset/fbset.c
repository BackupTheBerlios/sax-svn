/**************
FILE          : fbset.c
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the GPL license
              :
DESCRIPTION   : framebuffer implementation
              : © Copyright 1995-1998 by Geert Uytterhoeven
              :
              :
STATUS        : development
**************/

#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <sys/stat.h>

struct file;
struct inode;

#include "fb.h"
#include "fbset.h"
#include "../syslib.h"

//====================================
// Defines...
//------------------------------------
#define DEFAULT_FRAMEBUFFER	"/dev/fb0"
#define DEFAULT_MODEDBFILE	"/etc/fb.modes"

//====================================
// Globals...
//------------------------------------
static const char *ProgramName;

static int Opt_test = 0;
static int Opt_show = 0;
static int Opt_info = 0;
static int Opt_version = 0;
static int Opt_verbose = 0;
static int Opt_xfree86 = 0;
static int Opt_change = 0;
static int Opt_all = 0;

static const char *Opt_fb = NULL;
const char *Opt_modedb = DEFAULT_MODEDBFILE;
static const char *Opt_xres = NULL;
static const char *Opt_yres = NULL;
static const char *Opt_vxres = NULL;
static const char *Opt_vyres = NULL;
static const char *Opt_depth = NULL;
static const char *Opt_pixclock = NULL;
static const char *Opt_left = NULL;
static const char *Opt_right = NULL;
static const char *Opt_upper = NULL;
static const char *Opt_lower = NULL;
static const char *Opt_hslen = NULL;
static const char *Opt_vslen = NULL;
static const char *Opt_accel = NULL;
static const char *Opt_hsync = NULL;
static const char *Opt_vsync = NULL;
static const char *Opt_csync = NULL;
static const char *Opt_gsync = NULL;
static const char *Opt_extsync = NULL;
static const char *Opt_bcast = NULL;
static const char *Opt_laced = NULL;
static const char *Opt_double = NULL;
static const char *Opt_move = NULL;
static const char *Opt_step = NULL;
static const char *Opt_modename = NULL;
static const char *Opt_rgba = NULL;
static const char *Opt_nonstd = NULL;
static const char *Opt_grayscale = NULL;
static const char *Opt_matchyres = NULL;

//====================================
// Structures...
//------------------------------------
static struct {
    const char *name;
    const char **value;
    const int change;
} Options[] = {
    { "-fb", &Opt_fb, 0 },
    { "-db", &Opt_modedb, 0 },
    { "-xres", &Opt_xres, 1 },
    { "-yres", &Opt_yres, 1 },
    { "-vxres", &Opt_vxres, 1 },
    { "-vyres", &Opt_vyres, 1 },
    { "-depth", &Opt_depth, 1 },
    { "-nonstd", &Opt_nonstd, 1},
    { "-pixclock", &Opt_pixclock, 1 },
    { "-left", &Opt_left, 1 },
    { "-right", &Opt_right, 1 },
    { "-upper", &Opt_upper, 1 },
    { "-lower", &Opt_lower, 1 },
    { "-hslen", &Opt_hslen, 1 },
    { "-vslen", &Opt_vslen, 1 },
    { "-accel", &Opt_accel, 1 },
    { "-hsync", &Opt_hsync, 1 },
    { "-vsync", &Opt_vsync, 1 },
    { "-csync", &Opt_csync, 1 },
    { "-gsync", &Opt_gsync, 1 },
    { "-extsync", &Opt_extsync, 1 },
    { "-bcast", &Opt_bcast, 1 },
    { "-laced", &Opt_laced, 1 },
    { "-double", &Opt_double, 1 },
    { "-move", &Opt_move, 1 },
    { "-step", &Opt_step, 1 },
    { "-rgba", &Opt_rgba, 1 },
    { "-grayscale", &Opt_grayscale, 1 },
    { NULL, NULL, 0 }
};


struct VideoMode *VideoModes = NULL;
static struct textentry {
    __u32 id;
    const char *name;
} Textmodes[] = {
    { FB_AUX_TEXT_MDA, "Monochrome text" },
    { FB_AUX_TEXT_CGA, "CGA/EGA/VGA Color text" },
    { FB_AUX_TEXT_S3_MMIO, "S3 MMIO fasttext" },
    { FB_AUX_TEXT_MGA_STEP16, "MGA Millennium I step 16 text" },
    { FB_AUX_TEXT_MGA_STEP8, "MGA step 8 text" },
};

static struct textentry VGAModes[] = {
    { FB_AUX_VGA_PLANES_VGA4, "VGA 16 colors in 4 planes" },
    { FB_AUX_VGA_PLANES_CFB4, "VGA 16 colors in 1 plane" },
    { FB_AUX_VGA_PLANES_CFB8, "VGA 256 colors in 4 planes" },
    /* last entry has name == NULL */
    { 0,                      NULL}
};

static struct accelentry {
    __u32 id;
    const char *name;
} Accelerators[] = {
    { FB_ACCEL_NONE, "No" },
    { FB_ACCEL_ATARIBLITT, "Atari Blitter" },
    { FB_ACCEL_AMIGABLITT, "Amiga Blitter" },
    { FB_ACCEL_S3_TRIO64, "S3 Trio64" },
    { FB_ACCEL_NCR_77C32BLT, "NCR 77C32BLT" },
    { FB_ACCEL_S3_VIRGE, "S3 ViRGE" },
    { FB_ACCEL_ATI_MACH64GX, "ATI Mach64GX" },
    { FB_ACCEL_DEC_TGA, "DEC 21030 TGA" },
    { FB_ACCEL_ATI_MACH64CT, "ATI Mach64CT" },
    { FB_ACCEL_ATI_MACH64VT, "ATI Mach64VT" },
    { FB_ACCEL_ATI_MACH64GT, "ATI Mach64GT" },
    { FB_ACCEL_SUN_CREATOR, "Sun Creator/Creator3D" },
    { FB_ACCEL_SUN_CGSIX, "Sun cg6" },
    { FB_ACCEL_SUN_LEO, "Sun leo/zx" },
    { FB_ACCEL_IMS_TWINTURBO, "IMS Twin Turbo" },
    { FB_ACCEL_3DLABS_PERMEDIA2, "3Dlabs Permedia 2" },
    { FB_ACCEL_MATROX_MGA2064W, "Matrox MGA2064W (Millennium)" },
    { FB_ACCEL_MATROX_MGA1064SG, "Matrox MGA1064SG (Mystique)" },
    { FB_ACCEL_MATROX_MGA2164W, "Matrox MGA2164W (Millennium II)" },
    { FB_ACCEL_MATROX_MGA2164W_AGP, "Matrox MGA2164W (Millennium II AGP)" },
    { FB_ACCEL_MATROX_MGAG100, "Matrox G100 (Productiva G100)" },
    { FB_ACCEL_MATROX_MGAG200, "Matrox G200 (Millennium, Mystique)" },
    { FB_ACCEL_SUN_CG14, "Sun cg14" },
    { FB_ACCEL_SUN_BWTWO, "Sun bw2" },
    { FB_ACCEL_SUN_CGTHREE, "Sun cg3" },
    { FB_ACCEL_SUN_TCX, "Sun tcx" },
    { FB_ACCEL_MATROX_MGAG400, "Matrox G400" },
};

struct VideoMode Current;


//====================================
// Functions...
//------------------------------------
int  OpenFrameBuffer(const char *name);
void CloseFrameBuffer(int fh);
void GetVarScreenInfo(int fh, struct fb_var_screeninfo *var);
void SetVarScreenInfo(int fh, struct fb_var_screeninfo *var);
void GetFixScreenInfo(int fh, struct fb_fix_screeninfo *fix);
static void ConvertFromVideoMode(const struct VideoMode *vmode,
				 struct fb_var_screeninfo *var);
static void ConvertToVideoMode(const struct fb_var_screeninfo *var,
			       struct VideoMode *vmode);
static int atoboolean(const char *var);
static void ReadModeDB(void);
static struct VideoMode *FindVideoMode(const char *name);
static void ModifyVideoMode(struct VideoMode *vmode);
static void DisplayVModeInfo(struct VideoMode *vmode);
static void DisplayFBInfo(struct fb_fix_screeninfo *fix);
static int FillScanRates(struct VideoMode *vmode);
int main(int argc, char *argv[]);


void Die(const char *fmt, ...) {
    va_list ap;

    fflush(stdout);
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    exit(1);
}


int OpenFrameBuffer(const char *name) {
    int fh;

    if (Opt_verbose)
	printf("Opening frame buffer device `%s'\n", name);

    //if ((fh = open(name, O_RDONLY)) == -1)
	//Die("open %s: %s\n", name, strerror(errno));
	fh = open(name, O_RDONLY);
    return fh;
}


void CloseFrameBuffer(int fh) {
    close(fh);
}

void GetVarScreenInfo(int fh, struct fb_var_screeninfo *var) {
    if (ioctl(fh, FBIOGET_VSCREENINFO, var))
	Die("ioctl FBIOGET_VSCREENINFO: %s\n", strerror(errno));
}


void SetVarScreenInfo(int fh, struct fb_var_screeninfo *var) {
    if (ioctl(fh, FBIOPUT_VSCREENINFO, var))
	Die("ioctl FBIOPUT_VSCREENINFO: %s\n", strerror(errno));
}


void GetFixScreenInfo(int fh, struct fb_fix_screeninfo *fix) {
    if (ioctl(fh, FBIOGET_FSCREENINFO, fix))
	Die("ioctl FBIOGET_FSCREENINFO: %s\n", strerror(errno));
}


static void ConvertFromVideoMode(const struct VideoMode *vmode,
				 struct fb_var_screeninfo *var)
{
    memset(var, 0, sizeof(struct fb_var_screeninfo));
    var->xres = vmode->xres;
    var->yres = vmode->yres;
    var->xres_virtual = vmode->vxres;
    var->yres_virtual = vmode->vyres;
    var->bits_per_pixel = vmode->depth;
    var->nonstd = vmode->nonstd;
    if (Opt_test)
	var->activate = FB_ACTIVATE_TEST;
    else
	var->activate = FB_ACTIVATE_NOW;
    if (Opt_all)
	var->activate = FB_ACTIVATE_ALL;
    var->accel_flags = vmode->accel_flags;
    var->pixclock = vmode->pixclock;
    var->left_margin = vmode->left;
    var->right_margin = vmode->right;
    var->upper_margin = vmode->upper;
    var->lower_margin = vmode->lower;
    var->hsync_len = vmode->hslen;
    var->vsync_len = vmode->vslen;
    if (vmode->hsync == HIGH)
	var->sync |= FB_SYNC_HOR_HIGH_ACT;
    if (vmode->vsync == HIGH)
	var->sync |= FB_SYNC_VERT_HIGH_ACT;
    if (vmode->csync == HIGH)
	var->sync |= FB_SYNC_COMP_HIGH_ACT;
    if (vmode->gsync == HIGH)
	var->sync |= FB_SYNC_ON_GREEN;
    if (vmode->extsync == TRUE)
	var->sync |= FB_SYNC_EXT;
    if (vmode->bcast == TRUE)
	var->sync |= FB_SYNC_BROADCAST;
    if (vmode->laced == TRUE)
	var->vmode = FB_VMODE_INTERLACED;
    else if (vmode->dblscan == TRUE)
	var->vmode = FB_VMODE_DOUBLE;
    else
	var->vmode = FB_VMODE_NONINTERLACED;
    var->vmode |= FB_VMODE_CONUPDATE;
    var->red.length = vmode->red.length;
    var->red.offset = vmode->red.offset;
    var->green.length = vmode->green.length;
    var->green.offset = vmode->green.offset;
    var->blue.length = vmode->blue.length;
    var->blue.offset = vmode->blue.offset;
    var->transp.length = vmode->transp.length;
    var->transp.offset = vmode->transp.offset;
    var->grayscale = vmode->grayscale;
}


static void ConvertToVideoMode(const struct fb_var_screeninfo *var,
			       struct VideoMode *vmode)
{
    vmode->name = NULL;
    vmode->xres = var->xres;
    vmode->yres = var->yres;
    vmode->vxres = var->xres_virtual;
    vmode->vyres = var->yres_virtual;
    vmode->depth = var->bits_per_pixel;
    vmode->nonstd = var->nonstd;
    vmode->accel_flags = var->accel_flags;
    vmode->pixclock = var->pixclock;
    vmode->left = var->left_margin;
    vmode->right = var->right_margin;
    vmode->upper = var->upper_margin;
    vmode->lower = var->lower_margin;
    vmode->hslen = var->hsync_len;
    vmode->vslen = var->vsync_len;
    vmode->hsync = var->sync & FB_SYNC_HOR_HIGH_ACT ? HIGH : LOW;
    vmode->vsync = var->sync & FB_SYNC_VERT_HIGH_ACT ? HIGH : LOW;
    vmode->csync = var->sync & FB_SYNC_COMP_HIGH_ACT ? HIGH : LOW;
    vmode->gsync = var->sync & FB_SYNC_ON_GREEN ? TRUE : FALSE;
    vmode->extsync = var->sync & FB_SYNC_EXT ? TRUE : FALSE;
    vmode->bcast = var->sync & FB_SYNC_BROADCAST ? TRUE : FALSE;
    vmode->grayscale = var->grayscale;
    vmode->laced = FALSE;
    vmode->dblscan = FALSE;
    switch (var->vmode & FB_VMODE_MASK) {
	case FB_VMODE_INTERLACED:
	    vmode->laced = TRUE;
	    break;
	case FB_VMODE_DOUBLE:
	    vmode->dblscan = TRUE;
	    break;
    }
    vmode->red.length = var->red.length;
    vmode->red.offset = var->red.offset;
    vmode->green.length = var->green.length;
    vmode->green.offset = var->green.offset;
    vmode->blue.length = var->blue.length;
    vmode->blue.offset = var->blue.offset;
    vmode->transp.length = var->transp.length;
    vmode->transp.offset = var->transp.offset;
    FillScanRates(vmode);
}


static int atoboolean(const char *var) {
    int value = 0;

    if (!strcasecmp(var, "false") || !strcasecmp(var, "low") ||
	!strcasecmp(var, "no") || !strcasecmp(var, "off") ||
	!strcmp(var, "0"))
	value = 0;
    else if (!strcasecmp(var, "true") || !strcasecmp(var, "high") ||
	     !strcasecmp(var, "yes") || !strcasecmp(var, "on") ||
	     !strcmp(var, "1"))
	value = 1;
    else
	Die("Invalid value `%s'\n", var);

    return value;
}


FbData* FbGetData(void) {
 FbData *fb;
 struct VideoMode vmode;
 struct fb_var_screeninfo var;
 int res,sstart,send,total;
 str ht,vt;
 str flags;
 int fh = -1; 

 fb = (FbData*)malloc(sizeof(FbData));
 fb->x     = 640;
 fb->y     = 480;
 fb->depth = 8;
 fb->hsync = 33; 
 fb->vsync = 72;
 fb->clock = 35;
 strcpy(fb->ht,"");
 strcpy(fb->vt,"");
 strcpy(fb->flags,"");

 fh = OpenFrameBuffer(DEFAULT_FRAMEBUFFER);
 if (fh < 0) {
  return(fb);
 }
 GetVarScreenInfo(fh, &var);
 ConvertToVideoMode(&var, &vmode);

 #if __powerpc__
 // on PPC we had a real framebuffer driver which provides
 // the correct timing values of the active framebuffer
 // ...
 #else
 // on other architectures we will limit the framebuffer
 // to a 60Hz mode
 // ...
 vmode.hrate = (60.0 / vmode.vrate) * vmode.hrate;
 vmode.drate = (vmode.xres+vmode.right+vmode.hslen+vmode.left) * vmode.hrate;
 vmode.vrate = 60;
 #endif
 fb->x     = vmode.xres;
 fb->y     = vmode.yres;
 fb->depth = vmode.transp.offset;
 fb->hsync = vmode.hrate/1e3;
 fb->vsync = vmode.vrate;
 fb->clock = vmode.drate/1e6;

 res = vmode.xres;
 sstart = res+vmode.right;
 send = sstart+vmode.hslen;
 total = send+vmode.left;
 sprintf(ht,"%d %d %d %d",res, sstart, send, total);

 res = vmode.yres;
 sstart = res+vmode.lower;
 send = sstart+vmode.vslen;
 total = send+vmode.upper;
 sprintf(vt,"%d %d %d %d", res, sstart, send, total);

 strcpy(flags,"");
 if (vmode.laced)
  strcat(flags,"Interlace ");
 if (vmode.dblscan)
  strcat(flags,"DoubleScan ");
 if (vmode.hsync)
  strcat(flags,"+HSync ");
 else
  strcat(flags,"-HSync ");
 if (vmode.vsync)
  strcat(flags,"+VSync ");
 else
  strcat(flags,"-VSync ");
 if (vmode.csync)
  strcat(flags,"Composite ");
 if (vmode.bcast)
  strcat(flags,"bcast ");

 strcpy(fb->ht,ht);
 strcpy(fb->vt,vt);
 strcpy(fb->flags,flags);
 CloseFrameBuffer(fh);
 return(fb);
}

static void DisplayVModeInfo(struct VideoMode *vmode) {
    u_int res, sstart, send, total;

    puts("");
    if (!Opt_xfree86) {
	printf("mode \"%dx%d", vmode->xres, vmode->yres);
	if (vmode->pixclock) {
	    printf("-%d\"\n", (int)(vmode->vrate+0.5));
	    printf("    # D: %5.3f MHz, H: %5.3f kHz, V: %5.3f Hz\n",
		   vmode->drate/1E6, vmode->hrate/1E3, vmode->vrate);
	} else
	    puts("\"");
	printf("    geometry %d %d %d %d %d\n", vmode->xres, vmode->yres,
	       vmode->vxres, vmode->vyres, vmode->depth);
	printf("    timings %d %d %d %d %d %d %d\n", vmode->pixclock,
	       vmode->left, vmode->right, vmode->upper, vmode->lower,
	       vmode->hslen, vmode->vslen);
	if (vmode->hsync)
	    puts("    hsync high");
	if (vmode->vsync)
	    puts("    vsync high");
	if (vmode->csync)
	    puts("    csync high");
	if (vmode->gsync)
	    puts("    gsync true");
	if (vmode->extsync)
	    puts("    extsync true");
	if (vmode->bcast)
	    puts("    bcast true");
	if (vmode->laced)
	    puts("    laced true");
	if (vmode->dblscan)
	    puts("    double true");
	if (vmode->nonstd)
            printf("    nonstd %u\n", vmode->nonstd);
	if (vmode->accel_flags)
	    puts("    accel true");
	if (vmode->grayscale)
	    puts("    grayscale true");
	printf("    rgba %u/%u,%u/%u,%u/%u,%u/%u\n",
	    vmode->red.length, vmode->red.offset, vmode->green.length,
	    vmode->green.offset, vmode->blue.length, vmode->blue.offset,
	    vmode->transp.length, vmode->transp.offset);
	puts("endmode\n");
    } else {
	printf("Mode \"%dx%d\"\n", vmode->xres, vmode->yres);
	if (vmode->pixclock) {
	    printf("    # D: %5.3f MHz, H: %5.3f kHz, V: %5.3f Hz\n",
		   vmode->drate/1E6, vmode->hrate/1E3, vmode->vrate);
	    printf("    DotClock %5.3f\n", vmode->drate/1E6+0.001);
	} else
	    puts("    DotClock Unknown");
	res = vmode->xres;
	sstart = res+vmode->right;
	send = sstart+vmode->hslen;
	total = send+vmode->left;
	printf("    HTimings %d %d %d %d\n", res, sstart, send, total);
	res = vmode->yres;
	sstart = res+vmode->lower;
	send = sstart+vmode->vslen;
	total = send+vmode->upper;
	printf("    VTimings %d %d %d %d\n", res, sstart, send, total);
	printf("    Flags   ");
	if (vmode->laced)
	    printf(" \"Interlace\"");
	if (vmode->dblscan)
	    printf(" \"DoubleScan\"");
	if (vmode->hsync)
	    printf(" \"+HSync\"");
	else
	    printf(" \"-HSync\"");
	if (vmode->vsync)
	    printf(" \"+VSync\"");
	else
	    printf(" \"-VSync\"");
	if (vmode->csync)
	    printf(" \"Composite\"");
	if (vmode->extsync)
	    puts("    # Warning: XFree86 doesn't support extsync\n");
	if (vmode->bcast)
	    printf(" \"bcast\"");
	if (vmode->accel_flags)
	    puts("    # Warning: XFree86 doesn't support accel\n");
	if (vmode->grayscale)
	    puts("    # Warning: XFree86 doesn't support grayscale\n");
	puts("\nEndMode\n");
    }
}


static void DisplayFBInfo(struct fb_fix_screeninfo *fix) {
    unsigned int i;

    puts("Frame buffer device information:");
    printf("    Name        : %s\n", fix->id);
    printf("    Address     : %p\n", fix->smem_start);
    printf("    Size        : %d\n", fix->smem_len);
    printf("    Type        : ");
    switch (fix->type) {
	case FB_TYPE_PACKED_PIXELS:
	    puts("PACKED PIXELS");
	    break;
	case FB_TYPE_PLANES:
	    puts("PLANES");
	    break;
	case FB_TYPE_INTERLEAVED_PLANES:
	    printf("INTERLEAVED PLANES (%d bytes interleave)\n",
		   fix->type_aux);
	    break;
	case FB_TYPE_TEXT:
	    for (i = 0; i < sizeof(Textmodes)/sizeof(*Textmodes); i++)
		if (fix->type_aux == Textmodes[i].id)
		    break;
	    if (i < sizeof(Textmodes)/sizeof(*Textmodes))
		puts(Textmodes[i].name);
	    else
		printf("Unknown text (%d)\n", fix->type_aux);
	    break;
	case FB_TYPE_VGA_PLANES:
	    {
	    	struct textentry *t;
		
		for (t = VGAModes; t->name; t++)
		    if (fix->type_aux == t->id)
		    	break;
		if (t->name)
		    puts(t->name);
		else
	            printf("Unknown VGA mode (%d)\n", fix->type_aux);
	    }
	    break;
	default:
	    printf("%d (UNKNOWN)\n", fix->type);
	    printf("    Type_aux    : %d\n", fix->type_aux);
	    break;
    }
    printf("    Visual      : ");
    switch (fix->visual) {
	case FB_VISUAL_MONO01:
	    puts("MONO01");
	    break;
	case FB_VISUAL_MONO10:
	    puts("MONO10");
	    break;
	case FB_VISUAL_TRUECOLOR:
	    puts("TRUECOLOR");
	    break;
	case FB_VISUAL_PSEUDOCOLOR:
	    puts("PSEUDOCOLOR");
	    break;
	case FB_VISUAL_DIRECTCOLOR:
	    puts("DIRECTCOLOR");
	    break;
	case FB_VISUAL_STATIC_PSEUDOCOLOR:
	    puts("STATIC PSEUDOCOLOR");
	    break;
	default:
	    printf("%d (UNKNOWN)\n", fix->visual);
	    break;
    }
    printf("    XPanStep    : %d\n", fix->xpanstep);
    printf("    YPanStep    : %d\n", fix->ypanstep);
    printf("    YWrapStep   : %d\n", fix->ywrapstep);
    printf("    LineLength  : %d\n", fix->line_length);
    if (fix->mmio_len) {
	printf("    MMIO Address: %p\n", fix->mmio_start);
	printf("    MMIO Size   : %d\n", fix->mmio_len);
    }
    printf("    Accelerator : ");
    for (i = 0; i < sizeof(Accelerators)/sizeof(*Accelerators); i++)
	if (fix->accel == Accelerators[i].id)
	    break;
    if (i < sizeof(Accelerators)/sizeof(*Accelerators))
	puts(Accelerators[i].name);
    else
	printf("Unknown (%d)\n", fix->accel);
}


static int FillScanRates(struct VideoMode *vmode) {
    u_int htotal = vmode->left+vmode->xres+vmode->right+vmode->hslen;
    u_int vtotal = vmode->upper+vmode->yres+vmode->lower+vmode->vslen;

    if (vmode->dblscan)
	vtotal <<= 2;
    else if (!vmode->laced)
	vtotal <<= 1;

    if (!htotal || !vtotal)
	return 0;

    if (vmode->pixclock) {
	vmode->drate = 1E12/vmode->pixclock;
	vmode->hrate = vmode->drate/htotal;
	vmode->vrate = vmode->hrate/vtotal*2;
    } else {
	vmode->drate = 0;
	vmode->hrate = 0;
	vmode->vrate = 0;
    }

    return 1;
}
