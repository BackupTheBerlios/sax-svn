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
#include <errno.h>
#include <signal.h>

#include "qx.h"
#include "log.h"
#include "xlock.h"

//=================================
// Functions...
//---------------------------------
char* qx (char*command,int channel,int anz,char* format,...) {
	char data[30] = "/tmp/data";
	char *result  = NULL;
	char *arg[anz+2];
	va_list ap;
	FILE* new_channel = NULL;
	FILE *fd;
	int n=1;
	long size=0;
	int child;

	signal( SIGCHLD , handlesigchld );

	//=================================
	// prepare arguments for execv...
	//---------------------------------
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

	//=================================
	// call the program...
	//---------------------------------
	switch ( child = fork() ) {
	case -1:
	log(L_ERROR,
		"qx: could not fork: %s\n",strerror(errno)
	);
	exit(1);

	case 0:
	// child process...
	// -----------------
	if (channel == 0) {
		new_channel = freopen(data,"w",stdout);
	}
	if (channel == 1) {
		new_channel = freopen(data,"w",stderr);
	}
	execv (command,arg);

	// ups should not happen here...
	// ------------------------------
	if (new_channel) {
		fclose (new_channel);
		unlink (data);
	}
	log(L_ERROR,
		"qx: could not execute command: %s\n",arg
	);

	default:
	// parent process...
	// ------------------
	waitpid (child,NULL,WNOHANG);
	if ((channel == 0) || (channel == 1)) {
	while (getproc(data)) {
		usleep(500);
	}
	if (! (fd = fopen(data,"r"))) {
		return(NULL);
	}
	fseek(fd,0L,SEEK_END);
	size = ftell(fd) - 1;
	rewind(fd);
	if (size <= 0) {
		fclose(fd); unlink(data); 
		log(L_INFO,
        	"qx: command returned (null) string\n"
		);
		return(NULL);
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

void handlesigchld  (int) {
	union wait wstatus;
	while (wait3(&wstatus,WNOHANG,NULL) > 0);
}
