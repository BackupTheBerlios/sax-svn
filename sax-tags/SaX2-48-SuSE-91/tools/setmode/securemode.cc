#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{

//---------------------//
// PARAMETER VARIABLES //
//---------------------//
    
float DCF_max=0;    // This is the Maximum dot-clock  
float HSF=0;        // This is the hsync frequency
float VSF=0;        // This is the vsync frequency
float HSP_min=1;    // Number of horizontal sync impulse 
int HR=0;           // Resolution X
int VR=0;           // Resolution Y

//-------------------------//  
// CALCULATED VARIABLES    //
//-------------------------//
  
float HSP_us;       // Horizontal Sync Impulse in u sec
float DCF;          // The calculated dot-clock
float RR;           // calculated display refresh rate
int HFL;            // Horizontal frame length
int VFL;            // Vertikal frame length
int HFT_us;         // Horizontal frame timing in u sec 
int HSP;            // Number of horizontal sync impuls
int VSP;            // Number of vertical sync impulse

//----------------------//  
// CONSTANT VARIABLES   //
//----------------------//
  
double HCONST=1.3;    // 1.3
double VCONST=1.045;  // 1.045
int   HGT1=16;
int   HGT2=7*16;

  
//--------------------//  
// PROGRAM VARIABLES  //
//--------------------//
  
int VERBOSE=0;     // Give me a message if something is wrong
int ERROR=0;       // Set the error code 

  
/* ----- READING COMMAND LINE PARAMETERS ----- */

if (argc != 6)
{ 
 printf ("Wrong arguments\n");
 exit (0);
}      
       
DCF_max=atof(argv[3]);
DCF_max=DCF_max/1e6;
HSF=atof(argv[4]);
HSF=HSF/1e3;
VSF=atof(argv[5]);

HSP_min=1;
       
HR=atoi(argv[1]);
VR=atoi(argv[2]);


/* ----- CHECKING AND CORRECTING PARAMETERS ----- */

if ((HR<320)||(HR>2048))
{
 if (VERBOSE==255)
 {
  printf("\nERROR(101): invalid horizontal resolution : %u\n\n", HR);
  exit(101);
 }
}
  
if ((VR<200)||(VR>1600))
{
 if (VERBOSE==255)
 {
  printf("\nWARNING: invalid vertical resolution : %u", VR);
  printf("\ncorrecting vertical resolution using the 4:3 ratio...\n\n");
 }
 VR = (int)(HR * 0.75);
}
  
if ((DCF_max<10)||(DCF_max>320))
{
 if (VERBOSE==255)
 { 
  printf("\nERROR(102): invalid maximum dot clock rate : %-6.2f\n\n", DCF_max);
  exit(102);
 }
}
  
if ((HSF<31)||(HSF>180))
{
 if (VERBOSE==255) 
 {
  printf("\nERROR(103): invalid horizontal refresh frequency : %-6.2f\n\n", HSF);
  exit(103);
 } 
}
  
if ((VSF<50)||(VSF>300))
{
 if (VERBOSE==255)
 {
  printf("\nERROR(104): invalid vertical refresh frequency : %-6.2f\n\n", VSF);
  exit(104);
 }  
}
  
if ((HSP_min<0.5)||(HSP_min>5))
{
 if (VERBOSE==255)
 {
  printf("\nERROR(105): invalid horizontal sync pulse length : %-6.2f\n\n", HSP_min);
  exit(105);
 } 
}
  
  
/* ----- STARTING TO CALCULATE ----- */
  
ERROR=128;
    
while (ERROR!=0)
{
    ERROR=0; 
      
    HFL = (int)(HR * HCONST);
    HFL = (HFL / 8) * 8;         // is a multiple of 8 for compatibility 
    DCF = (HFL * HSF) / 1000;
         
    HSP = HFL - (HR + HGT1 + HGT2);
    HSP_us = HSP / DCF;
     
    VFL = (int)(VR * VCONST);
    HFT_us = (int)(HFL / DCF);
    VSP = 150 / HFT_us;          // assumed 150us for vertical sync 
     
    RR = ( ((DCF / HFL) * 1000) / VFL)*1000;
     
     /* This checkback below could be done much more elegantly, but it works
        this way and in most cases it does not hurt to waste some performance
        here. For speed-critical cases this part must be rewritten! */
     
    if (DCF>DCF_max)
    {
     HSF-=0.1;
     ERROR=255;
    }
       
    if (RR>VSF)
    {
     HSF-=0.1;
     ERROR=255;
    }
       
    if (HSP_us<HSP_min)
    {
     HSF-=0.1;
     ERROR=255;
    }
    
//printf("%d\n%d\n",(int)(DCF*1e3/HFL),(int)(DCF*1e6/(HFL*VFL)));     
//printf("Modeline \"%ux%u\" %-1.2f %u %u %u %u %u %u %u %u\n",HR,VR,DCF,HR,HR+HGT1,HR+HGT1+HSP,HFL,VR,VR,VR+VSP,VFL);
      
}
  
  
/* ----- PRINT OUT THE RESULTS ----- */
  

if (VERBOSE==255)
{
 printf("\n\nPLEASE CHECK THESE CALCULATED VALUES:\n\n");
 printf("        visible resolution:  %u x %u pixels",HR,VR);
 printf("\n            dot clock rate:  %6.2f MHz",DCF);
 printf("\n     vertical refresh rate:  %6.2f Hz",RR);
 printf("\n     horizontal sync pulse:  %6.2f us",HSP_us);
 printf("\n\nMake sure that none of the above exceeds the specifications\n");
 printf("of your hardware. If one does then DO NOT use the calculated\n");
 printf("mode line values, because bad settings can damage your system!\n\n");
 printf("If they are OK then use the line below in your XF86Config file:\n\n");
}
 
printf("%d\n%d\n",(int)(DCF*1e3/HFL),(int)(DCF*1e6/(HFL*VFL)));     
printf("Modeline \"%ux%u\" %-1.2f %u %u %u %u %u %u %u %u\n",HR,VR,DCF,HR,HR+HGT1,HR+HGT1+HSP,HFL,VR,VR,VR+VSP,VFL);
      
}
 

