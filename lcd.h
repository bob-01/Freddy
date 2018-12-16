// LCD function
#define uchar unsigned char
#define uint  unsigned int
#define LCD_Data P0

sbit    LCD_RW     = P2^3;
sbit    LCD_RS     = P2^4;
sbit    LCD_E      = P2^2;

void LCD_W(char LCD_C);
void LCD_SetCursor(char a, char b);
void DisplayOneChar(uchar X,uchar Y,uchar ASCII);
void delayms(uint ms);

int LCD_init()
{
//SET LCD  0x38,0x38,0x06,0x0c,0x01,0
		 LCD_W(0x38);		// LCD 2lines, 5*7 matrix
		 LCD_W(0x0f);		// Display ON cursor ON Blinking off
		 LCD_W(0x01);		// Clear the LCD
		 LCD_W(0x80);		// Cursor to First line First Position
return 0;

}

void LCD_W(uchar LCD_C)		  // Write bit on display
{
 LCD_Data = LCD_C;
 LCD_RS = 0;
 LCD_RW = 0;
 LCD_E = 1;

 delayms(5);

 LCD_E = 0;
 delayms(5);

}

void LCD_Write(char ch) 
{ 
	LCD_Data = ch;	// Send the data to LCD 
	LCD_RS = 1;
	LCD_RW = 0;
	LCD_E = 1;
		delayms(5);
 	LCD_E = 0;
		delayms(5);
}

void LCD_String(char *string_ptr, int x,int y)
{ 
	LCD_SetCursor(x,y);
	while(*string_ptr) 
		LCD_Write(*string_ptr++);		 
}

void LCD_SetCursor(int x, int y)
{
	if(x == 1){
	  LCD_W(0x80 + y);			//Move cursor on line 1
	  return;
	}
	if(x == 2){
	  LCD_W(0xc0 + y);			//Move cursor on line 2
	  return;
	}
}

void LCD_clear()
{

	LCD_W(0x01);

}

void LCD_ClrStr(unsigned int len, unsigned int x,unsigned int y)
{
	if (x) LCD_SetCursor(x,y);
	while(len > 0)
	{
		LCD_Write(' ');
		len--;
	}
}
void cursor() // pokazat' kursor na screen, miganie kursora
{
	LCD_W(0x0f);
}
void noCursor()						// scrit' kursor
{
	LCD_W(0x0c);
}
void delayms(uint Ms)
{
  uint i,TempCyc;
  _nop_();
  for(i=0;i<Ms;i++)
  {
    TempCyc =70;
    while(TempCyc--);
  }
}
