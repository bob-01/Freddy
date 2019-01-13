
//sbit ENC_B = P2^1;

extern Fset;
unsigned char EncState,  SPEED;
char EncData;

//////////////////////
bit EncoderScan(){


		if (EncState == 0) Fset = Fset+SPEED*5;
		else Fset = Fset-SPEED*5;

		 	SPEED++;
			ENC = 0;
	

/*
unsigned char New;
 
New = P2 & 0x03;
 
switch(EncState)
	{
	case 2:
		{
		if(New == 3) Fset = Fset+SPEED*5;
		if(New == 0) Fset = Fset-SPEED*5;
	 	SPEED++;
		break;
		}
 
	case 0:
		{
		if(New == 2) Fset = Fset+SPEED*5;
		if(New == 1) Fset = Fset-SPEED*5;
	 	SPEED++;
		break;
		}
	case 1:
		{
		if(New == 0) Fset = Fset+SPEED*5;
		if(New == 3) Fset = Fset-SPEED*5;
	 	SPEED++;
		break;
		}
	case 3:
		{
		if(New == 1) Fset = Fset+SPEED*5;
		if(New == 2) Fset = Fset-SPEED*5; 
	 	SPEED++;
		break;
		}
	}// End Case

	EncState = New;
*/

}//End Encoder Scan
