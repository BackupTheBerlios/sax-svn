#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#define UP    1
#define DOWN  2

typedef char string[256];


// The Parameters of the Program are the following:     //
// GetModeline ModeX ModeY DotClock HorizFreq VertFreq  //   

// The following modifications were made by Robert Goetz //
// 06JAN99 to add an expert mode and fix some bugs       //
//                                                       //
// 4 new Parameters at command line:                     //
// Length of Left Shoulder, Right Shoulder, HSYNC, VSYNC //
//                                                       //
// changed name for right shoulder in program, because   //
// it's actually the hsync time!                         //
// new name is now hsync_imp                             //
//                                                       //
// changed limit for hor and ver from fix values to the  //
// max values given on command line                      //
// (hope it's not a mistake, otherwise the hor. limit    //
//  should be greater than 100kHz due to technical       //
//  improvement ;==)                                  )  //
//                                                       //
// setting of VsyncFreq=StoreVsyncFreq deleted because   //
// possible endlos loop! (hope I'm right...)             //
// setting of HsyncFreq=StoreHsyncFreq deleted because   //
// possible endlos loop! (hope I'm right...)             //
//                                                       //
// end of loop comparison with HTOTAL changed to the new //
// HTOTALEND. Otherwise the loop will be terminated when //
// reaching the first value where right shoulder < HTOTAL//
// So, we miss the right shoulder !!!!!!!!!!!            //
// My monitor told me that too ;=)                       //
// Same with VTOTAL and VOTALEND                         //

    

int main(int argc, char *argv[])
{

if (argc != 6 && argc != 10) 
{
 //cout<<"usage: < ModeX > < ModeY > < DotClock > < HorizFreq > < VertFreq >";
 //cout<<endl;
 printf("usage: < ModeX > < ModeY > < DotClock > < HorizFreq > < VertFreq >");
 printf("\n");
 printf("       [ Left_SH ] [ Right_SH ] [ HSYNC ] [ VSYNC ]");
 printf("\n");
 exit(0);
} 
 

//------------------------//
// Commandline parameters //
//------------------------//
int     ModeX     = atoi(argv[1]);
int     ModeY     = atoi(argv[2]);
double  DotClock  = atof(argv[3]);
if (DotClock <= 1e6) exit(0);
double  HsyncFreq = atof(argv[4]);
if (HsyncFreq <= 1e3) exit(0);
double  VsyncFreq = atof(argv[5]);


double  StoreHsyncFreq=HsyncFreq;
double  StoreVsyncFreq=VsyncFreq;
int  HUpDown = UP;
int  VUpDown = UP;
//int  VSstep=1;
//int  HSstep=500;
int  expertmode=0;
double left_shoulder	= 0;
double right_shoulder	= 0;
double dcf_ca			= 0;
double vsync_imp		= 0;
double hsync_imp		= 0;

//---------------------------//
// these are the timings set // 
// in my experience          //
//---------------------------//

if (argc == 6) 
{
 left_shoulder	= 0.5e-6;        //1.2
 hsync_imp		= 1.1e-6;        //3.8
 dcf_ca			= 1.3;
 vsync_imp		= 150e-6;
}
else
{
 left_shoulder	= atof(argv[6]) * 1e-6;
 if (left_shoulder == 0) exit(0);
 right_shoulder	= atof(argv[7]) * 1e-6;
 if (right_shoulder == 0) exit(0);
 hsync_imp		= atof(argv[8]) * 1e-6;
 if (hsync_imp == 0) exit(0);
 vsync_imp		= atof(argv[9]) * 1e-6;
 if (vsync_imp == 0) exit(0);
 expertmode				= 1;
}


//-----------------------//
// Correcting parameters //
//-----------------------//
double Dot = 0;
if (expertmode == 0)
{
 Dot = HsyncFreq * dcf_ca * ModeX;
 right_shoulder = left_shoulder;  //hope the programmer won't shoot me down.. //
// printf("%f \n", Dot);
}
else
{
 Dot = ( HsyncFreq * ModeX ) / ( 1 - HsyncFreq * (left_shoulder + right_shoulder + hsync_imp ) );
// printf("%f \n", Dot);
 if (Dot < 1e6) exit(0);
}
if (Dot <  DotClock)  DotClock=Dot;

// printf("%f us %f us %f us %f us \n", left_shoulder * 1e6, right_shoulder * 1e6, hsync_imp * 1e6, vsync_imp * 1e6);

//------------------------------//
// calculate horizontal timings //
//------------------------------//

int    error      = 256;
double DCF        = 0;
int    HDISP      = 0;
int    HSYNCSTART = 0;
int    HSYNCEND   = 0;
int    HTOTALEND  = 0;
int    HTOTAL     = 0;
int    VDISP      = 0;
int    VSYNCSTART = 0;
int    VSYNCEND   = 0;
int    VTOTAL     = 0;
int    VTOTALEND  = 0;


while ( error != 128 )
{

 error      = 128;
 DCF        = 0;
 HDISP      = 0;
 HSYNCSTART = 0;
 HSYNCEND   = 0;
 HTOTALEND  = 0;
 HTOTAL     = 0;
 VDISP      = 0;
 VSYNCSTART = 0;
 VSYNCEND   = 0;
 VTOTAL     = 0;
 
 
 //horizontal
 DCF=DotClock;
 HDISP=ModeX;
 HTOTAL=int(DCF/HsyncFreq); HTOTAL=(HTOTAL/8)*8;
 HSYNCSTART=int(HDISP + ( left_shoulder * DCF )); HSYNCSTART=(HSYNCSTART/8)*8;
 HSYNCEND=int(HSYNCSTART + ( hsync_imp * DCF )); HSYNCEND=(HSYNCEND/8)*8;
 HTOTALEND=int(HTOTAL - ( right_shoulder * DCF )); HTOTALEND=(HTOTALEND/8)*8;
// printf("HTOTAL = %i ", HTOTAL);
 
 //vertical
 VDISP=ModeY;
 VTOTAL=int(DCF/( HTOTAL * VsyncFreq ));
 VSYNCSTART=int(VDISP + ( 0.25 * (VTOTAL - VDISP) ));
 VSYNCEND=int(VSYNCSTART + ( (vsync_imp * DCF ) / HTOTAL ));
 VTOTALEND=int(VTOTAL - ( 0.25 * (VTOTAL - VDISP) ));
// printf("VTOTAL = %i ", VTOTAL);
// printf("HF = %f  VF = %f \n", HsyncFreq, VsyncFreq);
 
 
 //------------------------------//
 // Prove the values             //
 //------------------------------//
 
 
 if ((VDISP > VSYNCSTART) || (VSYNCSTART > VSYNCEND) || (VSYNCEND >= VTOTALEND))
 {
  //cout<<"Hs_Ti_Fault"<<endl;
  if (VsyncFreq > StoreVsyncFreq) { HsyncFreq=StoreHsyncFreq; VUpDown=DOWN; }
  switch (VUpDown)
  {
   case UP    :  VsyncFreq=VsyncFreq+1;
                 break;
   case DOWN  :  VsyncFreq=VsyncFreq-1;
                 break;
  }
  error=256;
  continue;   
  //exit(0);
 }
 else
 {
  float want_vsync=atof(argv[5]);
  float have_vsync=DCF/(HTOTAL*VTOTAL);
  //cout<<want_vsync<<" "<<have_vsync<<endl;
  if ( have_vsync > want_vsync ) 
  {
   error=256;
   VsyncFreq=VsyncFreq-1; 
   continue;  
  }
 }
 
 if ((HDISP > HSYNCSTART) || (HSYNCSTART > HSYNCEND) || (HSYNCEND >= HTOTALEND))
 {
  //cout<<"Vs_Ti_Fault"<<endl;
  if (HsyncFreq > StoreHsyncFreq) { VsyncFreq=StoreVsyncFreq; HUpDown=DOWN; }
  switch (HUpDown)
  {
   case UP   :   HsyncFreq=HsyncFreq+1000;  
                 break;
   case DOWN :   HsyncFreq=HsyncFreq-1000;
                 break;
  }               
  error=256; 
  continue; 
  //exit(0);
 }
 else
 {
  float want_hsync=atof(argv[4]);
  float have_hsync=DCF/(HTOTAL);
  //cout<<want_hsync<<" "<<have_hsync<<endl;
  if ( have_hsync > want_hsync ) 
  {
   error=256;
   HsyncFreq=HsyncFreq-500; 
   continue;  
  }
 }
 

}


//------------------------------//
// prove the line               //
//------------------------------//

float HS=(DCF/(HTOTAL*1000));
float VS=(DCF/(HTOTAL*VTOTAL));
printf("%.2f\n",HS);
printf("%.2f\n",VS);
//cout <<HS<<endl;
//cout <<VS<<endl;
//HsyncFreq = rint(DCF / (HTOTAL*1000));
//VsyncFreq = rint(DCF / (HTOTAL*VTOTAL));
//cout <<HsyncFreq<<endl;
//cout <<VsyncFreq<<endl;

//------------------------------//
// print the Modeline           //
//------------------------------//

//cout <<"Modeline \""<<ModeX<<"x"<<ModeY<<"\" "<<(int)DCF/1e6;
//cout <<"  "<<HDISP<<" "<<HSYNCSTART<<" "<<HSYNCEND<<" "<<HTOTAL;
//cout <<"  "<<VDISP<<" "<<VSYNCSTART<<" "<<VSYNCEND<<" "<<VTOTAL<<endl;

printf("Modeline \"%dx%d\" %-1.2f %d %d %d %d %d %d %d %d\n",ModeX,ModeY,(int)DCF/1e6,
        HDISP,HSYNCSTART,HSYNCEND,HTOTAL,VDISP,VSYNCSTART,VSYNCEND,VTOTAL);                   

//cout<<endl;
printf("\n");
}




