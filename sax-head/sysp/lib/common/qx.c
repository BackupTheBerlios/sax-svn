/**************
FILE          : qx.c
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the XFree86 license
              :
DESCRIPTION   : common used functions: 
              : qx -> call program and return stdout data
              :
STATUS        : development
**************/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#include "../syslib.h"

//=================================
// Functions...
//---------------------------------
char* qx(char*command,int channel,int anz,char* format,...) {
 char data[30] = "/tmp/data";
 char *result  = NULL;
 char *arg[anz+2];
 va_list ap;
 FILE *fd;
 int n=1;
 long size=0;

 // prepare arguments for execv...
 // -------------------------------
 arg[0] = (char*)malloc(sizeof(char)*MAX_PROGRAM_SIZE);
 strcpy(arg[0],command);
 arg[anz+1] = NULL;

 // prepare data file...
 // ---------------------
 sprintf(data,"%s-%d",data,getpid());

 // get arguments...
 // ----------------- 
 if (format != NULL) {
  va_start(ap, format);
  while (*format) {
   switch(*format++) {
    case 's':
    arg[n] = va_arg(ap, char*);
    n++;
    break;
   }
  }
  va_end(ap);
 }

 // call the program...
 // --------------------
 switch(fork()) {
  case -1:
  perror("fork");
  exit(1);

  case 0:
  // child process...
  // -----------------
  if (channel == 0) {
   freopen(data,"w",stdout);
  }
  if (channel == 1) {
   freopen(data,"w",stderr);
  }
  execv(command,arg);

  default:
  // parent process...
  // ------------------
  wait(NULL);

  if ((channel == 0) || (channel == 1)) {
   fd = fopen(data,"r");
   if (fd == NULL) {
    return(NULL);
   }
   fseek(fd,0L,SEEK_END);
   size = ftell(fd) - 1;
   rewind(fd);
   if (size <= 0) {
    fclose(fd); return(NULL);
   }
   result = (char*)malloc(sizeof(char)*size + 1);
   fread(result,size,1,fd);
   result[size] = '\0';
   fclose(fd);
   unlink(data);
  }
  return(result);
 }
}


