/*______________________________________________
 | ss_snn_lib
 | Scuola Sisini Shallow Neural Network Library
 | Francesco Sisini (c) 2019
 */


/*_______________________________________________________________________
  |LICENZA GPL
  |
  |ss_snn_lib: libreria per costruire una rete di percettroni a più strati
  |Copyright (C) 2018  Francesco Sisini (francescomichelesisini@gmail.com)
  |
  |This program is free software: you can redistribute it and/or modify
  |it under the terms of the GNU General Public License as published by
  |the Free Software Foundation, either version 3 of the License, or
  |(at your option) any later version.
  |
  |This program is distributed in the hope that it will be useful,
  |but WITHOUT ANY WARRANTY; without even the implied warranty of
  |MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  |GNU General Public License for more details.
  |
  |You should have received a copy of the GNU General Public License
  |along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "ss_snn_lib.h"

/*_____________________________________________________
 | stream: il file su cui scrivere 
 | v_w: l'array sequenziale con tutti i pesi del layer
 | n_dend: numero di dendriti per percettrone
 | n_perc: numero di percettroni nel layer
 */
void layer_writedown(FILE * stream,double *v_w, int n_dend, int n_perc)
{
  fwrite(v_w, sizeof(double), n_dend*n_perc, stream);
}

/*_____________________________________________________
 | stream: il file da cui
 | v_w: l'array sequenziale con tutti i pesi del layer
 | n_dend: numero di dendriti per percettrone
 | n_perc: numero di percettroni nel layer
 */
void layer_read(FILE * stream,double *v_w, int n_dend, int n_perc)
{
  fread(v_w, sizeof(double), n_dend*n_perc, stream);
}



/*___________________________________________________________________________________ 
 *|
 *| v_s: vettore delle somme dei canali dendritici per gli n_perc percettroni
 *| v_y: vettore degli output per gli n_perc percetroni
 *| v_w: vettore dei pesi dendritici per gli n_perc percetroni
 *| v_x: vettore degli input al percettrone (uguale per tutti  gli n_perc percetroni)
 *| n_perc: numero di percettroni nello strato
 *| n_dend: numero di dendriti per percettrone */
void layer_feed_forward(double v_s[],double v_y[],double v_w[],double v_x[],int n_perc, int n_dend)
{
  for(int i=0;i<n_perc;i++)
    {
      /*calcola l'output per ogni percettrone*/
      v_s[i]=perc_calc_output(v_w+i*(n_dend+1),v_x,n_dend);
      v_y[i]=activ_function(v_s[i]);
    }
}

/*__________________________________________________________________________________________
 *| v_w vettore di dimensione n_dend+1. Il primo elemento è 1, il resto sono i pesi sinaptici
 *| v_x vettore dell'input del percettrone. Il primo elemento è 1
 *| z somma pesata dell'input: v_w < . v_x
 *| d Valore Atteso - Valore Calcolato
 *| rate learning rate
 *| n_dend numero dei dendriti del percettrone
 */
void perc_correzione(double v_w[],double v_x[],double z,double d,double rate,int n_dend)
{
  /* cicolo sui dendriti */
   for(int i=0;i<n_dend+1;i++)
    {
      v_w[i]=v_w[i]+rate*v_x[i]*(d)*Dactiv_function(z);
    }
}

void layer_map_out_in(double v_x[],double v_y[], int n_dend)
{
  v_x[0]=1;
  for(int i=1;i<n_dend+1;i++)v_x[i]=v_y[i-1];
}

/*  v_w: vettore di dimensione  n_dend+1 di pesi dendritici
 *  v_x: vettore delgi n_dend+1 (c'è il bias) input al percettrone
 *  n_dend numero di dendriti  */
double perc_calc_output(double v_w[],double v_x[],int n_dend)
{
  double a=0;
  /*somma pesata degli stimoli di ingresso*/
  for(int i=0;i<n_dend+1;i++) a=a+v_w[i]*v_x[i];
 
  /*Attivazione del percettrone*/
  return a;
}
double activ_function(double summed_input)
{
  // double r=tanh(summed_input);
  double r=1/(1+exp(-summed_input));
  return r;
}
double Dactiv_function(double summed_input)
{
  //double r=tanh(summed_input);
  double r=activ_function(summed_input);
  //return 1-r*r;
  return r*(1-r);
}

int get_image(int * o,int * out_label,FILE * in_stream)
{
  int r=fscanf(in_stream,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
               out_label,o,o+1,o+2,o+3,o+4,o+5,o+6,o+7,o+8,o+9,o+10,o+11,o+12,o+13,o+14,o+15,o+16,o+17,o+18,o+19,o+20,o+21,o+22,o+23,o+24,o+25,o+26,o+27,o+28,o+29,o+30,o+31,o+32,o+33,o+34,
               o+35,o+36,o+37,o+38,o+39,o+40,o+41,o+42,o+43,o+44,o+45,o+46,o+47,o+48,o+49,o+50,o+51,o+52,o+53,o+54,o+55,o+56,o+57,o+58,o+59,o+60,o+61,o+62,o+63,o+64,o+65,o+66,o+67,o+68,o+69,
               o+70,o+71,o+72,o+73,o+74,o+75,o+76,o+77,o+78,o+79,o+80,o+81,o+82,o+83,o+84,o+85,o+86,o+87,o+88,o+89,o+90,o+91,o+92,o+93,o+94,o+95,o+96,o+97,o+98,o+99,o+100,o+101,o+102,o+103,o+104,
               o+105,o+106,o+107,o+108,o+109,o+110,o+111,o+112,o+113,o+114,o+115,o+116,o+117,o+118,o+119,o+120,o+121,o+122,o+123,o+124,o+125,o+126,o+127,o+128,o+129,o+130,o+131,o+132,o+133,o+134,
               o+135,o+136,o+137,o+138,o+139,o+140,o+141,o+142,o+143,o+144,o+145,o+146,o+147,o+148,o+149,o+150,o+151,o+152,o+153,o+154,o+155,o+156,o+157,o+158,o+159,o+160,o+161,o+162,o+163,o+164,
               o+165,o+166,o+167,o+168,o+169,o+170,o+171,o+172,o+173,o+174,o+175,o+176,o+177,o+178,o+179,o+180,o+181,o+182,o+183,o+184,o+185,o+186,o+187,o+188,o+189,o+190,o+191,o+192,o+193,o+194,
               o+195,o+196,o+197,o+198,o+199,o+200,o+201,o+202,o+203,o+204,o+205,o+206,o+207,o+208,o+209,o+210,o+211,o+212,o+213,o+214,o+215,o+216,o+217,o+218,o+219,o+220,o+221,o+222,o+223,o+224,o+225,o+226,o+227,o+228,o+229,o+230,o+231,o+232,o+233,o+234,o+235,o+236,o+237,o+238,o+239,o+240,o+241,o+242,o+243,o+244,o+245,o+246,o+247,o+248,o+249,o+250,o+251,o+252,o+253,o+254,o+255,o+256,o+257,o+258,o+259,o+260,o+261,o+262,o+263,o+264,o+265,o+266,o+267,o+268,o+269,o+270,o+271,o+272,o+273,o+274,o+275,o+276,o+277,o+278,o+279,o+280,o+281,o+282,o+283,o+284,o+285,o+286,o+287,o+288,o+289,o+290,o+291,o+292,o+293,o+294,o+295,o+296,o+297,o+298,o+299,o+300,o+301,o+302,o+303,o+304,o+305,o+306,o+307,o+308,o+309,o+310,o+311,o+312,o+313,o+314,o+315,o+316,o+317,o+318,o+319,o+320,o+321,o+322,o+323,o+324,o+325,o+326,o+327,o+328,o+329,o+330,o+331,o+332,o+333,o+334,o+335,o+336,o+337,o+338,o+339,o+340,o+341,o+342,o+343,o+344,o+345,o+346,o+347,o+348,o+349,o+350,o+351,o+352,o+353,o+354,o+355,o+356,o+357,o+358,o+359,o+360,o+361,o+362,o+363,o+364,o+365,o+366,o+367,o+368,o+369,o+370,o+371,o+372,o+373,o+374,o+375,o+376,o+377,o+378,o+379,o+380,o+381,o+382,o+383,o+384,o+385,o+386,o+387,o+388,o+389,o+390,o+391,o+392,o+393,o+394,o+395,o+396,o+397,o+398,o+399,o+400,o+401,o+402,o+403,o+404,o+405,o+406,o+407,o+408,o+409,o+410,o+411,o+412,o+413,o+414,o+415,o+416,o+417,o+418,o+419,o+420,o+421,o+422,o+423,o+424,o+425,o+426,o+427,o+428,o+429,o+430,o+431,o+432,o+433,o+434,o+435,o+436,o+437,o+438,o+439,o+440,o+441,o+442,o+443,o+444,o+445,o+446,o+447,o+448,o+449,o+450,o+451,o+452,o+453,o+454,o+455,o+456,o+457,o+458,o+459,o+460,o+461,o+462,o+463,o+464,o+465,o+466,o+467,o+468,o+469,o+470,o+471,o+472,o+473,o+474,o+475,o+476,o+477,o+478,o+479,o+480,o+481,o+482,o+483,o+484,o+485,o+486,o+487,o+488,o+489,o+490,o+491,o+492,o+493,o+494,o+495,o+496,o+497,o+498,o+499,o+500,o+501,o+502,o+503,o+504,o+505,o+506,o+507,o+508,o+509,o+510,o+511,o+512,o+513,o+514,o+515,o+516,o+517,o+518,o+519,o+520,o+521,o+522,o+523,o+524,o+525,o+526,o+527,o+528,o+529,o+530,o+531,o+532,o+533,o+534,o+535,o+536,o+537,o+538,o+539,o+540,o+541,o+542,o+543,o+544,o+545,o+546,o+547,o+548,o+549,o+550,o+551,o+552,o+553,o+554,o+555,o+556,o+557,o+558,o+559,o+560,o+561,o+562,o+563,o+564,o+565,o+566,o+567,o+568,o+569,o+570,o+571,o+572,o+573,o+574,o+575,o+576,o+577,o+578,o+579,o+580,o+581,o+582,o+583,o+584,o+585,o+586,o+587,o+588,o+589,o+590,o+591,o+592,o+593,o+594,o+595,o+596,o+597,o+598,o+599,o+600,o+601,o+602,o+603,o+604,o+605,o+606,o+607,o+608,o+609,o+610,o+611,o+612,o+613,o+614,o+615,o+616,o+617,o+618,o+619,o+620,o+621,o+622,o+623,o+624,o+625,o+626,o+627,o+628,o+629,o+630,o+631,o+632,o+633,o+634,o+635,o+636,o+637,o+638,o+639,o+640,o+641,o+642,o+643,o+644,o+645,o+646,o+647,o+648,o+649,o+650,o+651,o+652,o+653,o+654,o+655,o+656,o+657,o+658,o+659,o+660,o+661,o+662,o+663,o+664,o+665,o+666,o+667,o+668,o+669,o+670,o+671,o+672,o+673,o+674,o+675,o+676,o+677,o+678,o+679,o+680,o+681,o+682,o+683,o+684,o+685,o+686,o+687,o+688,o+689,o+690,o+691,o+692,o+693,o+694,o+695,o+696,o+697,o+698,o+699,o+700,o+701,o+702,o+703,o+704,o+705,o+706,o+707,o+708,o+709,o+710,o+711,o+712,o+713,o+714,o+715,o+716,o+717,o+718,o+719,o+720,o+721,o+722,o+723,o+724,o+725,o+726,o+727,o+728,o+729,o+730,o+731,o+732,o+733,o+734,o+735,o+736,o+737,o+738,o+739,o+740,o+741,o+742,o+743,o+744,o+745,o+746,o+747,o+748,o+749,o+750,o+751,o+752,o+753,o+754,o+755,o+756,o+757,o+758,o+759,o+760,o+761,o+762,o+763,o+764,o+765,o+766,o+767,o+768,o+769,o+770,o+771,o+772,o+773,o+774,o+775,o+776,o+777,o+778,o+779,o+780,o+781,o+782,o+783);
  return r;

}

void print_object(double x[],int r, int c,int R,int C)
{
  double min,max;
  min=10;
  max=-10;
  for(int i=0;i<r;i++)
    for(int j=0;j<c;j++)
      {
	if(x[i*c+j]>max) max=x[i*c+j];
	if(x[i*c+j]<min) min=x[i*c+j];
      }
  double i_range=max-min;
  if(i_range==0)
    {
      printf("X MERDA!");
      printf("%f - %f \n",max,min);
      
    }
  double max_c=255;
  double min_c=0;
  double c_range=max_c-min_c;
  double conv=c_range/i_range;

  
  for(int i=0;i<r;i++)
    for(int j=0;j<c;j++)
      {
	double gl;
	gl=x[i*c+j];
	
	int col=max_c-(min_c+(double)(gl-min)*conv)+min_c;
	if(col<0)printf("%f",gl);
	//if(gl>0.9) col=0;
	printf("\x1b[%d;%dH\x1b[38;5;%dmX\x1b[0m  ",i+R,2*j+C,col);

      }
  fflush(stdout);
}




