/**************
FILE          : hwupdate.h
***************
PROJECT       : SaX2 ( SuSE advanced X configuration )
              :
BELONGS TO    : Configuration tool XFree86 version 4.x
              :  
DESCRIPTION   : header file for hwupdate.c
              : for a detailed description please see
              : the source file
              : 
              : 
STATUS        : Status: Up-to-date
**************/
#ifndef HWUPDATE_H
#define HWUPDATE_H 1

//===========================
// Prototypes...
//---------------------------
int doScan (hd_hw_item_t item);
int doList (hd_hw_item_t item);
int doConf (int type, char *val, char *id);
int accessAllowed (void);

#endif
