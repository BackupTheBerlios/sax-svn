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
static int Opt_test = 0;
static int Opt_verbose = 0;
static int Opt_xfree86 = 0;
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
static const char *Opt_rgba = NULL;
static const char *Opt_nonstd = NULL;
static const char *Opt_grayscale = NULL;

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
static void ConvertToVideoMode(const struct fb_var_screeninfo *var,
			       struct VideoMode *vmode);
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
