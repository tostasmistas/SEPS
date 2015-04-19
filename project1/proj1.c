#include "dsk6713_aic23.h"		//codec-DSK support file
#include "C6713dskinit.h"

Uint32 fs=DSK6713_AIC23_FREQ_16KHZ;	//set sampling rate

char	intflag = FALSE;
union {Uint32 samples; short channel[2];} AIC_buffer;

short sine[33] = {0,3212,6393,9512,12540,15447,18205,20788,23170,25330,27246,28899,30274,31357,32138,32610,32767,32610,32138,31357,30274,28899,27246,25330,23170,20788,18205,15447,12540,9512,6393,3212,0};

interrupt void c_int11()                  //interrupt service routine
{
   output_sample(AIC_buffer.samples);   	//output data
   AIC_buffer.samples= input_sample(); 	  //input data
   intflag = TRUE;
   return;
}

void main()
{
  short 	inbuf = 0;
  short 	delta = 0 ;
  short		status = -32767;
  short     y1 = 0 , y2 = 0 , y = 0;
  short     i = 0, delta_x = 0;
  short		amp = 16384;

  comm_intr();
  while(1){
    if(intflag != FALSE){
      intflag = FALSE;

      inbuf = AIC_buffer.channel[LEFT];

      delta = 16384 + (inbuf>>2);

      status = status + delta;
      delta_x = (status & 1023)<<5;

      i = (status>>10)&31;
      y1 = sine[i];
      y2 = sine[i+1];
      y = (amp*(y1 + ((y2-y1)*delta_x>>15))>>15);

      if(status < 0){
    	  y1 = -y1;
      }
      AIC_buffer.channel[LEFT] = y;
    }
  }
}
