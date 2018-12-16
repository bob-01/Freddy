#include <reg52.h>
#include <intrins.h>
#include <lcd.h>
#include <old.c>

char *VERSION="FreDDy-1.59";

void i2s(long num){ // Preobrazovanie integer v ASCII

char str[11];
int i=0;

 if (num == 0) {
 
	LCD_Write('0');
	return; 
 }
	while(num != 0)
	{
	   str[i] = num%10 + '0';
	   num /= 10;
	   i++;
	}

	while(i > 0){
	i--;
	LCD_Write(str[i]);
	}
}

void main(void){

//  delayms(100);
  LCD_init();
  noCursor();

   fangdian_button=1;
   init_t0();
   strive_f1();

	LCD_String(VERSION,1,0);

   while(1)
   {
   strive_f2();//«Û»°F1
     strive_cx();
     strive_fx(); 
     strive_min_c();
     strive_max_c();
     judge_xianshi();
   }
}