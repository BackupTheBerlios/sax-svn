%{
/*------------------------*/
/*  C declaration part    */
/*------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char  *yytext;
extern int   zeilennr;

/*----TYP DEFINITIONS-----*/
typedef char str[512];
struct LOGSTRUCT { str key,value,id; };
struct CLKSTRUCT { str clk,id; };
char   pdev[30];

/*----VARIABLES-----------*/
str ActId       = "";
int ActClk      = 0;
int ParseError  = 0;
str ErrorString = ""; 

struct LOGSTRUCT detect[200]; int detectindex = 0;
struct CLKSTRUCT clocks[200]; int clocksindex = 0;


/*----FUNCTIONS-----------*/
void yyerror ( char *s)
{
 /*
 ParseError = -1;
 strcpy(ErrorString,yytext);
 */
 return;
}

void log(str key,str value)  {
 strcpy(detect[detectindex].key,key);
 strcpy(detect[detectindex].value,value);
 strcpy(detect[detectindex].id,ActId);
 detectindex++;
}

void clk(str clk) {
 strcpy(clocks[clocksindex].clk,clk);
 strcpy(clocks[clocksindex].id,ActId);
 clocksindex++;
}

%}

%start zeile

%union 
{
  float reel;
  int   integer;
  char  string[256];
} 

%token <string> REFRESH PROBED FROMCONFIG DEFAULT CMDLINE NOTICE INFO DAC
%token <string> WARNING ERROR STRING INT ID MEMORY CHIPSET VESA VESAMODE
%token <string> TRASH CLOCKS FLOAT PANEL RESO PDEV BUSID DDC VMWARE DPIX DPIY
%token NICHTS

%%

zeile    : ausdruck         { }     /* realize line itteration */
         | zeile ausdruck   { } 

ausdruck : error 			{ yyerrok;yyclearin; }
         | Section 			{ }
         ;

Section  : PROBED     ID { strcpy(ActId,$2); } MEMORY  FLOAT  { log("MEM",$5); }
         | DEFAULT    ID { strcpy(ActId,$2); } MEMORY  FLOAT  { log("MEM",$5); }
         | INFO       ID { strcpy(ActId,$2); } MEMORY  FLOAT  { log("MEM",$5); }
         | INFO       PDEV BUSID { sprintf(pdev,"%s",$3); }
         | INFO       ID { strcpy(ActId,$2); } DPIX    FLOAT  { log("DPX",$5); } DPIY FLOAT { log("DPY",$8); }
         | FROMCONFIG ID { strcpy(ActId,$2); } MEMORY  FLOAT  { log("MEM",$5); }
         | PROBED     ID { strcpy(ActId,$2); } FLOAT   MEMORY { log("MEM",$4); }
         | PROBED     ID { strcpy(ActId,$2); } DAC     FLOAT  { log("DAC",$5); }
         | DEFAULT    ID { strcpy(ActId,$2); } DAC     FLOAT  { log("DAC",$5); }
         | INFO       ID { strcpy(ActId,$2); } DAC     FLOAT  { log("DAC",$5); }
         | PROBED     ID { strcpy(ActId,$2); } VMWARE  FLOAT  { log("VMW",$5); }
		 | PROBED     ID { strcpy(ActId,$2); } CHIPSET STRING { log("CHIP",$5);}
         | DEFAULT    ID { strcpy(ActId,$2); } CHIPSET STRING { log("CHIP",$5);}
		 | PROBED     ID { strcpy(ActId,$2); } CLOCKS  Clocks { }
         | PROBED     ID { strcpy(ActId,$2); } PANEL   RESO   { log("PAN",$5); }
         | INFO       ID { strcpy(ActId,$2); } PANEL   RESO   { log("PAN",$5); }
         | PROBED     ID { strcpy(ActId,$2); } RESO    PANEL  { log("PAN",$4); }
		 | REFRESH    INT { log("REFRESH",$2); }
         | INFO       ID { strcpy(ActId,$2); } VESAMODE       { log("VESA",$4);}
		 | VESAMODE      { log("VESA",$1);   }
         | INFO       ID { strcpy(ActId,$2); } DDC            { log("DDS",$4); }                                               FLOAT          { log("DDC",$6); }
         ;

Clocks   : FLOAT          { clk($1); }
		 | Clocks FLOAT   { clk($2); }
		 ;

%%


#include "lex.yy.c"


