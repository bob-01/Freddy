/*	
The way we calculate 1 machine cycle period in 8051
clock = 24MHz
then 1m/c = 1/ (24MHz / 12) = 0.5 usec

1 machine cycle = 0.5 usec
For 20msec --> time required is 20000/0.5 counts = 40000 counts
Timer 1 count => 65535-40000 => 0x63BF

	h = 4Mhz/200Hz = 20000
    R divider = 20000
	N = Fout/200
	N = 4000000/200 =20000
*/

#include <reg52.h>
#include <intrins.h>
#include <lcd.h>
#include <kic9256.c>

//sbit LED1  = P2^4;
sbit TX_B = P2^1;
sbit TX_OUT = P2^3;
sbit A = P2^0;

bit TX=0, TX_OLD=1,SHOW=1;

//unsigned char EncState, EncNew;

//unsigned char *VERSION="TeDDy-2";

bit delay_us(unsigned int i) {
    while(i--)
        _nop_();_nop_();
}

void T2Isr(void) interrupt 5 using 1 {

	if (EXF2 == 1) {

			if ( A == 1 ) Fset = Fset + SPEED*Fstep;
			else Fset = Fset - SPEED*Fstep;			

			if(Fset < Fmin) Fset = Fmin;
			if(Fset > Fmax) Fset = Fmax;

			SPEED++;

			if (SPEED > 100) SPEED = 100;

			SHOW = 1;
			_nop_();_nop_();_nop_();_nop_();_nop_();

			EXF2 = 0;
																					   
			return;
	
	}

	if (TF2 == 1) {
		
		TF2 = 0;
		cnt--;

/*		if (cnt == 0) {
		
			cnt = 20;
			LED1 = ~LED1;

		}
*/
		if (SPEED > 1){
		
				SPEED--;
		}

		return;
	}

}//End T2Isr interrupt

void F_PRINT(){ // Preobrazovanie integer v ASCII

unsigned int mid;

		LCD_Write((Fset/1000 + '0'));
		LCD_Write('.');
		mid = Fset%1000;
		LCD_Write(mid/100 + '0');
		mid %= 100;
		LCD_Write(mid/10 + '0');
		mid %= 10;
		LCD_Write(mid/1 + '0');

//		LCD_Write('.');
//		LCD_Write((Fset%10 + '0'));

}//End F_print

void main(void){

//		LED1 = 1;
		TX_OUT = 0;

		LCD_init();
		//noCursor();
		//LCD_String(VERSION,1,0);
 
		cnt = 20;
		ET2 = 1;
		EA = 1;
		RCAP2H = 0x3C;	  // 1 msec
		RCAP2L = 0xAF;
		TL2 = 0;
		T2CON |= 0x8;
		T2CON |= 0x4;
		
		Ustan();
		Freq(Fset+FP4);

while(1)
{//Start While

			if (TX_B == 0){
				
				while (TX_B == 0){
				
					_nop_();				

				}

				SHOW = 1;
				TX_OLD = TX_B;
				TX = ~TX;
						
			}

			if (SHOW == 1){

				LCD_SetCursor(1,0);
				F_PRINT();
				SHOW = 0;

				if (TX == 1){
				
					LCD_String("Tx .....",2,0);
					Freq(Fset);
					TX_OUT = 1;

				}else{
					
					LCD_String("Rx .....",2,0);
					TX_OUT =0;
					Freq(Fset+FP4);

				}

			}

}// End While

} //End Main
