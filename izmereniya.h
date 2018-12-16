// Function izmerenie F,C,L

long res=0;

void   lx_calc()
{

}

void   lx_print()
{

}

void   fx_calc()
{
unsigned char i;

      TR0=1;
      TR1=1;
      res=0;
      for(i=0;i<20;i++)
      {
        f_crycle=0;
        while(f_crycle<1);
        f_crycle=0;
        res+=temp;
      }
      TR0=0;
      TR1=0;

}

void   fx_print()
{
     LCD_String("Fx=",2,0);
	 if (res > 1000){

	 	d = res%1000;
		res /= 1000;
		i2s(res);
		LCD_Write('.');
		i2s(d);
	 	LCD_Write('k');
	 } 
	 else {i2s(res);}

	 LCD_Write('H');
	 LCD_Write('z');
	 LCD_ClrStr(7);

}

void   cx_calc()
{

}

void   cx_print()
{

}
