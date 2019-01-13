/*

*/

sbit SDA = P2^3;
sbit SCK = P2^2;

// Define i2c speed
#define I2C_SPEED_FACTOR	1	// Low value means low i2c frequency		
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
void I2C_Stop(void);
bit  I2C_Write_Byte(unsigned char);

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
    __delay_us(HalfBitDelay+100);
	Set_SCK_Low;

	return SDA;
}
