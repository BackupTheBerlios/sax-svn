/**************
FILE          : fbset.h
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the GPL license
              :
DESCRIPTION   : framebuffer header file and definitions  
              : © Copyright 1995-1998 by Geert Uytterhoeven
              :
              :
STATUS        : development
**************/

#include <stdio.h>
#include <sys/types.h>

#ifdef __GLIBC__
#include <asm/types.h>
#endif


//=================================
// Defines...
//---------------------------------
#define LOW			(0)
#define HIGH		(1)
#define FALSE		(0)
#define TRUE		(1)


//=================================
// Structures...
//---------------------------------
struct color {
    unsigned int length;
    unsigned int offset;
};

struct VideoMode {
    struct VideoMode *next;
    const char *name;
    __u32 xres;
    __u32 yres;
    __u32 vxres;
    __u32 vyres;
    __u32 depth;
    __u32 nonstd;
    __u32 accel_flags;
    __u32 pixclock;
    __u32 left;
    __u32 right;
    __u32 upper;
    __u32 lower;
    __u32 hslen;
    __u32 vslen;
    unsigned hsync : 1;
    unsigned vsync : 1;
    unsigned csync : 1;
    unsigned gsync : 1;
    unsigned extsync : 1;
    unsigned bcast : 1;
    unsigned laced : 1;
    unsigned dblscan : 1;
    unsigned grayscale : 1;
    double drate;
    double hrate;
    double vrate;
    struct color red, green, blue, transp;
};

extern FILE *yyin;
extern int  line;
extern const char *Opt_modedb;

extern int  yyparse(void);
extern void Die(const char *fmt, ...) __attribute__ ((noreturn));
extern void AddVideoMode(const struct VideoMode *vmode);
extern void makeRGBA(struct VideoMode *vmode, const char* opt);

