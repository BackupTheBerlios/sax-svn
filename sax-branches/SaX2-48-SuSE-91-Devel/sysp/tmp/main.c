#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/mman.h>
#include <assert.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

#include "../lib/syslib.h"

int main(void) {
 MsgDetect *result = NULL;
 int cards;
 int i,n;
 
 #if 1
 result = PLogGetData ("/var/log/XFree86.0.log");
 if (result == NULL) {
  printf("could not open file...\n");
  exit(1);
 }
 cards = result[0].cards;

 for (i=0;i<cards;i++) {
  printf("\nId:       %s\n",result[i].id);
  printf("Memory:   %ld\n",result[i].memory);
  printf("Dacspeed: %d\n",result[i].dacspeed);
  for (n=0;n<result[i].vesacount;n++) {
   printf("Vesa:     %d %d %d %d\n",
    result[i].vmodes[n].x,
    result[i].vmodes[n].y,
    result[i].vmodes[n].hsync,
    result[i].vmodes[n].vsync
   );
  }
  printf("Clock:    %s\n",result[i].clkstr);
  printf("Hsync:    %d\n",result[i].hsync_max);
  printf("Vsync:    %d\n",result[i].vsync_max);
  printf("Chipset:  %s\n",result[i].chipset);
  printf("Primary:  %s\n",result[i].ddc);
 }
 #endif

 exit(0);
}

