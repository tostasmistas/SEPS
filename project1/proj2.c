//Loop_intr.c Loop program using interrupt. Output = delayed input
// Testado na placa n. 2 - 05/10-A809

#include "dsk6713_aic23.h"		//codec-DSK support file
#include "C6713dskinit.h"

Uint32 fs=DSK6713_AIC23_FREQ_16KHZ;	//set sampling rate

char	intflag = FALSE;
short 	sine[4] = {0,32767,0,-32767};
union {Uint32 samples; short channel[2];} AIC_buffer;

interrupt void c_int11()         	//interrupt service routine
{
   output_sample(AIC_buffer.samples);   	//output data
   AIC_buffer.samples= input_sample(); 	    //input data
   intflag = TRUE;
   return;
}

void main()
{
  short 	d_n, y;
  short		b_i = 1, sine_i = 0, b = 0;
  short		c_n = 0;
  short		b_n = 1, shift15_cn = 0, not_shift15 = 0;

  comm_intr();
  while(1){
    if(intflag != FALSE){
      intflag = FALSE;

      sine_i= sine_i&3;
      y = sine[sine_i];
      sine_i++;
      y = (32767*y)>>15 ;


      /* implementacao do contador sem recurso a instruccao condicional if */
        //b_i++;
        //b=(b_i&16)>>4;
        //b_n=(b_n^b);
        //b_i=(b_i&15);*/
      if(b_i>15){
    	  b_i=0;
    	  b_n=(b_n^1);
    	  c_n=c_n^b_n;
    	  shift15_cn=c_n<<15;
    	  not_shift15=~shift15_cn;
    	  d_n=not_shift15>>14;

      }
      b_i++;
      y=d_n*y;

      AIC_buffer.channel[LEFT] = y;
    }
  }
}
