/*
I2c device address: 0x70h, 0x71h
Lcd chip: based on PCF8558
Instruction: control byte, address

Instruction	DB7	DB6	DB5	DB4	DB3	DB2	DB1	DB0	Description
Display control	Y2	Y1	Y0	0	0	0	0	0	Y address vector, blank mode
0	1	0	0	1	normal mode
0	1	1	0	1	all segments on mode
X address	0	X address	set column address
 
        pin1     Vlcd (+6.4V)
        pin2     Vlogic (+2.7V)
        pin3     Sdata (i2c)
        pin4     Gnd
        pin5     Sclk  (i2c)

		SDATA = P2^2
		SCLK = P2^3
*/

sbit SDA = P2^2;  // Data pin for i2c
sbit SCK = P2^3;  // Clock pin for i2c

// Define i2c speed
#define I2C_SPEED_FACTOR	2	// Low value means low i2c frequency		
#define Crystal_Value		24  // MHz
#define HalfBitDelay 		(500*Crystal_Value)/(12*I2C_SPEED_FACTOR)

// Define macros
#define Set_SDA_Low		SDA = 0
#define Set_SDA_High	SDA = 1
#define Set_SCK_Low		SCK = 0
#define Set_SCK_High	SCK = 1

//Function Declarations
void __delay_us(unsigned int);	
void InitI2C(void);
void I2C_Start(void);
void I2C_ReStart(void);
void I2C_Stop(void);
void I2C_Send_ACK(void);
void I2C_Send_NACK(void);
bit  I2C_Write_Byte(unsigned char);
unsigned char I2C_Read_Byte(void);

// Function Purpose: Produce approximate delay in given uSecs.
void __delay_us(unsigned int d)
{
   unsigned int i, limit;
   limit = d/15;

   for(i=0;i<limit;i++);
}


// Function Purpose: Set initial values of SCK and SDA pins
void InitI2C(void)
{	
	// Make SDA and SCK pins input initially
	SDA = 1;
	SCK = 1;
}


// Function Purpose: I2C_Start sends start bit sequence
void I2C_Start(void)
{
	Set_SCK_High;				// Make SCK pin high
	Set_SDA_High;				// Make SDA pin High
	__delay_us(HalfBitDelay);	// Half bit delay
	Set_SDA_Low;				// Make SDA Low
	__delay_us(HalfBitDelay);	// Half bit delay
}


// Function Purpose: I2C_ReStart sends start bit sequence
void I2C_ReStart(void)
{
	Set_SCK_Low;				// Make SCK pin low

	__delay_us(HalfBitDelay/2);	// Data pin should change it's value,
								// when it is confirm that SCK is low
	Set_SDA_High;				// Make SDA pin High
	
	__delay_us(HalfBitDelay/2);	// 1/4 bit delay
	Set_SCK_High;				// Make SCK pin high
	__delay_us(HalfBitDelay/2);	// 1/4 bit delay
	Set_SDA_Low;				// Make SDA Low
	__delay_us(HalfBitDelay/2);	// 1/4 bit delay
}


//Function : I2C_Stop sends stop bit sequence
void I2C_Stop(void)
{
	Set_SCK_Low;				// Make SCK pin low

	__delay_us(HalfBitDelay/2);	// Data pin should change it's value,
								// when it is confirm that SCK is low
	Set_SDA_Low;				// Make SDA pin low
	
	__delay_us(HalfBitDelay/2);	// 1/4 bit delay
	Set_SCK_High;				// Make SCK pin high
	__delay_us(HalfBitDelay/2);	// 1/4 bit delay
	Set_SDA_High;				// Make SDA high
	__delay_us(HalfBitDelay/2);	// 1/4 bit delay
}



//Function : I2C_Send_ACK sends ACK bit sequence
void I2C_Send_ACK(void)
{
	Set_SCK_Low;				// Make SCK pin low
	__delay_us(HalfBitDelay/2);	// Data pin should change it's value,
								// when it is confirm that SCK is low
	Set_SDA_Low;				// Make SDA Low
	__delay_us(HalfBitDelay/2);	// 1/4 bit delay
	Set_SCK_High;				// Make SCK pin high
	__delay_us(HalfBitDelay);	// Half bit delay
}


//Function : I2C_Send_NACK sends NACK bit sequence
void I2C_Send_NACK(void)
{
	Set_SCK_Low;				// Make SCK pin low
	__delay_us(HalfBitDelay/2);	// Data pin should change it's value,
								// when it is confirm that SCK is low
	Set_SDA_High;				// Make SDA high
	__delay_us(HalfBitDelay/2);	// 1/4 bit delay
	Set_SCK_High;				// Make SCK pin high
	__delay_us(HalfBitDelay);	// Half bit delay
}


// Function Purpose: I2C_Write_Byte transfers one byte
bit I2C_Write_Byte(unsigned char Byte)
{
	unsigned char i;		// Variable to be used in for loop
	
	for(i=0;i<8;i++)		// Repeat for every bit
	{
		Set_SCK_Low;		// Make SCK pin low
		
		__delay_us(HalfBitDelay/2);	// Data pin should change it's value,
									// when it is confirm that SCK is low

		if((Byte<<i)&0x80)  // Place data bit value on SDA pin
			Set_SDA_High;	// If bit is high, make SDA high
		else				// Data is transferred MSB first
			Set_SDA_Low;	// If bit is low, make SDA low

		__delay_us(HalfBitDelay/2);	// Toggle SCK pin
		Set_SCK_High;				// So that slave can
		__delay_us(HalfBitDelay);	// latch data bit
    }
		
	// Get ACK from slave
	Set_SCK_Low;
    Set_SDA_High;
    __delay_us(HalfBitDelay);
    Set_SCK_High;
    __delay_us(HalfBitDelay);

	return SDA;
}


// Function Purpose: I2C_Read_Byte reads one byte
unsigned char I2C_Read_Byte(void)
{
	unsigned char i, d, RxData = 0;

	for(i=0;i<8;i++)
	{
		Set_SCK_Low;					// Make SCK pin low
		Set_SDA_High;					// Don't drive SDA 
		__delay_us(HalfBitDelay);		// Half bit delay
		Set_SCK_High;					// Make SCK pin high
		__delay_us(HalfBitDelay/2);		// 1/4 bit delay
		d = SDA;					    // Capture Received Bit
		RxData = RxData|(d<<(7-i));   	// Copy it in RxData
		__delay_us(HalfBitDelay/2);		// 1/4 bit delay
	}
 
    return RxData;						// Return received byte
}
