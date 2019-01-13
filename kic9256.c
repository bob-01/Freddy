sbit PERIOD = P2^5;
sbit CLOCK = P2^6;
sbit DATA = P2^7;

unsigned int Fset=3000, Fmin=2700, Fmax=4000, FP4=450, kvartz=4500;
unsigned char Fstep=1, SPEED=1, cnt=0;

extern bit delay_us(unsigned int i);

#define T1 	delay_us(1)
#define T2 	delay_us(2);

void KIC_WRITE(unsigned int Byte,unsigned char s){

unsigned char i;

	for(i=0;i<s;i++)
	{
		if((Byte>>i)&0x01) DATA =1;	 //LSB
		else DATA = 0;
		T2;
		CLOCK = 1;
		T2;
		CLOCK = 0;
    }

}//End Write

void Ustan(){

	   T1;
	   PERIOD=0;
	   T1;
	   CLOCK=0;

		KIC_WRITE(0xD2,7);		   //Set D2h address

		DATA=1;
		T1;
		CLOCK=1;
		T1;
		PERIOD=1;
		T2;
		CLOCK=0;

		KIC_WRITE(0x1C,8);		  // Set 0x1C 
		KIC_WRITE(0xE0,8);		  // Set 0xE0
		KIC_WRITE(0x02,7);

		DATA=0;
		T2;
		CLOCK=1;
		T1;
		PERIOD=0;
		T1;
		PERIOD=1;
		DATA=1;

}//End Ustan

void Freq(unsigned int Fs){

unsigned int Kl,Kh;		//Type word

Fs = Fs / (kvartz/Fstep);
Fs = (Fs<<4);
Kl = Fs & 0xFF;
Kh = ((Fs & 0xFF00) >>8);

	   T1;
	   PERIOD=0;
	   T1;
	   CLOCK=0;

		KIC_WRITE(0xD0,7);

		T1;
		DATA=1;							
		T1;
		CLOCK=1;
		T1;
		PERIOD=1;
		T2;
		CLOCK=0;

		KIC_WRITE(Kl,8);
		KIC_WRITE(Kh,8);

/*
			1kHz	= 0x82
	Step 	2.5kHz	= 0x42
	Steep	5kHz	= 0x46
	Step	10kHz	= 0x4A
*/
		KIC_WRITE(0x82,7);	 //inverter 

		T1;
		DATA=1;				  //last bit of kHz
		T2;
		CLOCK=1;
		T1;
		PERIOD=0;
		T1;
		PERIOD=1;
		DATA=1;

}
