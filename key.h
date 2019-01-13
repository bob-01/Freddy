//Function KeyPress
//E0 nagato bolee odnoi knopki
//E1 ne vibran regim izmereniya

sbit    KeyS_c     = P2^7;	// Key S6
sbit    KeyB_c      = P3^6;	// Key S7
sbit    KeyFX         = P2^6;	// Key S1
sbit    KeyLX         = P2^0;	// Key S3
sbit    KeyCX         = P2^1;	// Key S2

void KeyPress()
{

unsigned int cnt_key=0,KEY=0;

if (KeyS_c == 0){
	cnt_key++;
	KEY=10;
}

if (KeyB_c == 0){
	cnt_key++;
	KEY=11;
}

if (KeyFX == 0){
	cnt_key++;
	KEY=2;
}

if (KeyLX == 0){
	cnt_key++;
	KEY=3;
}

if (KeyCX == 0){
	cnt_key++;
	KEY=4;
}

if (cnt_key <= 1){

	switch(KEY){
		
		case 0:
			LCD_String("C+=0",2,0);
			delayms(300);
			break;

		case 10:
			LCD_String("Cs=0",2,0);		
			break;

		case 11:
			LCD_String("Cb=0",2,0);
			break;

		case 2:
			LCD_String("Fx=0",2,0);
			fx_calc();
			fx_print();
			break;

		case 3:
			LCD_String("Lx=0",2,0);
//			lx_calc();
//			lx_print();
			break;

		case 4:
			LCD_String("Cx=0",2,0);
			break;

		default:
			LCD_String("E1",1,14);
			break;
	}

} else {

	LCD_String("E0",1,14);

}

}
