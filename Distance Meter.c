#include <stdint.h>
#include "C:/Keil/Labware/inc/tm4c123gh6pm.h"
//Changge Clock Period in case of not running on a 16 MHZ frequency
#define ClockPeriod 0.0625//in micro seconds
#define HalfSoundSpeed  0.017000//in cm per micro second
#define TTL 159 //equivalent counts for 10 micro seconds
#define MaxCount 0xffffff

/*Pin Notation < 7 Segment digit (#define E0_f = segment f connected to port e pin0)*/

/*10^0 Digit pin specific addressing A,B,E used
 ***********************************************************/
#define A4A3A2_cba		(*((volatile uint32_t *) 0x40004070))//Bit Specific Add for A4:2
#define B7B6_ed			(*((volatile uint32_t *) 0x40005300))//Bit Specific Add for B7:6
#define E0_f			(*((volatile uint32_t *) 0x40024004))//Bit Specific Add for E0
#define B2_g			(*((volatile uint32_t *) 0x40005010))//Bit Specific Add for B2
/*******************EOF 10^0 Digit**************************/

/*10^1 Digit pin specific addressing D,E used
 ***********************************************************/
#define B3_a			(*((volatile uint32_t *) 0x40005020))//Bit Specific Add for B3
#define C7C6C5C4_edcb	(*((volatile uint32_t *) 0x400063c0))//Bit Specific Add for C7:4
#define E2E1_gf 		(*((volatile uint32_t *) 0x40024018))//Bit Specific Add for E2:1
/*******************EOF 10^1 Digit**************************/

/*10^2 Digit pin specific addressing A,B,E used
//***********************************************************/
#define B1_a			(*((volatile uint32_t *) 0x40005008))//Bit Specific Add for B1
#define E5E4_cb 		(*((volatile uint32_t *) 0x400240c0))//Bit Specific Add for E5:4
#define B4_d			(*((volatile uint32_t *) 0x40005040))//Bit Specific Add for B4
#define A7A6A5_gfe 		(*((volatile uint32_t *) 0x40004380))//Bit Specific Add for A7:5
/*******************EOF 10^2 Digit**************************/

//Pin B0 is used as output bin to send trigger signal
#define Trig			(*((volatile uint32_t *) 0x40005004))

void SystemInit() {}

/*******************PORTs INITIALIZATION*********************/
void InitPortA()//initialized for test
{
	SYSCTL_RCGCGPIO_R |= 0x1;
	while ((SYSCTL_PRGPIO_R & 0x1) == 0) {}
	GPIO_PORTA_AMSEL_R = 0x00;//disable analog
	GPIO_PORTA_AFSEL_R = 0x00;//disable alternative function
	GPIO_PORTA_PCTL_R = 0x0;//select general io
	GPIO_PORTA_DEN_R = 0xfc;//enable 7:2 as  digital
	GPIO_PORTA_DIR_R = 0xfc;//7:2 output
	GPIO_PORTA_PUR_R = 0x00;
	GPIO_PORTA_DATA_R=0xfc;
}

void InitPortB()//initialized for test
{
	SYSCTL_RCGCGPIO_R |= 0x2;
	while (((SYSCTL_PRGPIO_R & 0x2)) == 0) {}
	GPIO_PORTB_AMSEL_R = 0x0;//disable analog
	GPIO_PORTB_AFSEL_R = 0x0;//disable alternative function
	GPIO_PORTB_PCTL_R = 0x0;//select general io
	GPIO_PORTB_DEN_R = 0xff;//
	GPIO_PORTB_DIR_R = 0xdf;//pb5 input echo pb0 output trig else for segment
	GPIO_PORTB_PUR_R = 0x00;//connect pull up resistor to switches
	GPIO_PORTB_DATA_R=0xfe;
}

void InitPortC()
{
	SYSCTL_RCGCGPIO_R |= 0x4;
	while (((SYSCTL_PRGPIO_R & 0x4)) == 0) {}
	GPIO_PORTC_AMSEL_R |= 0x0;//disable analog
	GPIO_PORTC_AFSEL_R |= 0x0;//disable alternative function
	GPIO_PORTC_PCTL_R |= 0x0;//select general io
	GPIO_PORTC_DEN_R |= 0xf0;//
	GPIO_PORTC_DIR_R |= 0xf0;//pb5 input echo pb0 output trig else for segment
	GPIO_PORTC_PUR_R |= 0x00;//connect pull up resistor to switches
	GPIO_PORTC_DATA_R|=0xf0;
}
void InitPortD()
{
	SYSCTL_RCGC2_R |= 0x8;
	while ((SYSCTL_PRGPIO_R & 0x8) == 0) {}
	GPIO_PORTD_LOCK_R = 0x4c4f434b;
	GPIO_PORTD_CR_R = 0x80;
	GPIO_PORTD_AMSEL_R = 0x0;
	GPIO_PORTD_AFSEL_R = 0x0;//disable alternative function
	GPIO_PORTD_PCTL_R = 0x0;//select general io
	GPIO_PORTD_DEN_R = 0xff;//enable digital 
	GPIO_PORTD_DIR_R = 0xff;//set desired direction
	GPIO_PORTD_PUR_R = 0x0;//connect pull up resistor to switches
	
}
void InitPortE()
{
	SYSCTL_RCGCGPIO_R |= 0x10;
	while ((SYSCTL_PRGPIO_R & 0x10) == 0) {}
	GPIO_PORTE_AMSEL_R |= 0x0;//disable analog
	GPIO_PORTE_AFSEL_R |= 0x0;//disable alternative function
	GPIO_PORTE_PCTL_R = 0x0;//select general io
	GPIO_PORTE_DEN_R |= 0x3f;//0 output
	GPIO_PORTE_DIR_R |= 0x3f;//set desired direction
	GPIO_PORTE_PUR_R |= 0x0;//connect pull up resistor to switches
	GPIO_PORTE_DATA_R=0x3f;
}
void InitPortF(){ 
	SYSCTL_RCGCGPIO_R |= 0x20;
	while ((SYSCTL_PRGPIO_R & 0x20) == 0) {}
	GPIO_PORTF_LOCK_R = 0x4C4F434B;
  GPIO_PORTF_CR_R = 0x1F;         
  GPIO_PORTF_AMSEL_R = 0x00;        
  GPIO_PORTF_PCTL_R = 0x00000000;   
  GPIO_PORTF_DIR_R = 0x0E;          
  GPIO_PORTF_AFSEL_R = 0x00;      
  GPIO_PORTF_PUR_R = 0x11;             
  GPIO_PORTF_DEN_R = 0x1F; 	
}
/*****************EOF PORTs INITIALIZATION*******************/

//set triger port for 10 micro seconds
void TrigSensor()
{
	NVIC_ST_CTRL_R = 0;
	NVIC_ST_CURRENT_R = 0;
	NVIC_ST_CTRL_R |= 0x4;
	NVIC_ST_RELOAD_R = TTL;
	NVIC_ST_CTRL_R |= 0x1;
	Trig = 0x01;
	while ((NVIC_ST_CTRL_R & 0x10000) == 0) {}
	Trig = 0x00;
}
//Use Systick Timer to delay 10 ms
void Delay10ms()
{
	NVIC_ST_CTRL_R = 0;
	NVIC_ST_CURRENT_R = 0;
	NVIC_ST_CTRL_R |= 0x4;
	NVIC_ST_RELOAD_R = 159999;
	NVIC_ST_CTRL_R |= 0x1;
	while ((NVIC_ST_CTRL_R & 0x10000) == 0) {}
}
//start counter with max count
void Count()
{
	NVIC_ST_CTRL_R = 0;
	NVIC_ST_CURRENT_R = 0;
	NVIC_ST_CTRL_R |= 0x4;
	NVIC_ST_RELOAD_R = MaxCount;
	NVIC_ST_CTRL_R |= 0x1;
}
/*Software Decoding for Ones Digit to be displayed on one 7 segment*/
//B2 E0 B7 B6 A4 A3 A2
//g  f  e  d  c  b  a
void Display10_0(int Ones)
{
	switch (Ones)
	{
	case 0:
	{
		//gfedcba
		//1000000
		A4A3A2_cba = 0x0;
		B7B6_ed = 0x0;
		E0_f = 0x0;
		B2_g = 0x4;

		break;
	}
	case 1:
	{
		//gfedcba
		//1111001
		A4A3A2_cba = 0x4;
		B7B6_ed = 0xc0;
		E0_f = 0x1;
		B2_g = 0x4;
		break;
	}
	case 2:
	{
		//gfedcba
		//0100100
		A4A3A2_cba = 0x10;
		B7B6_ed = 0x0;
		E0_f = 0x1;
		B2_g = 0x0;
		break;
	}
	case 3:
	{
		//gfedcba
		//0110000
		A4A3A2_cba = 0x0;
		B7B6_ed = 0x80;
		E0_f = 0x1;
		B2_g = 0x0;
		break;
	}
	case 4:
	{
		//gfedcba
		//0011001
		A4A3A2_cba = 0x4;
		B7B6_ed = 0xc0;
		E0_f = 0x0;
		B2_g = 0x0;
		break;
	}
	case 5:
	{
		//gfedcba
		//0010010
		A4A3A2_cba = 0x8;
		B7B6_ed = 0x80;
		E0_f = 0x0;
		B2_g = 0x0;
		break;
	}
	case 6:
	{
		//gfedcba
		//0000010
		A4A3A2_cba = 0x8;
		B7B6_ed = 0x0;
		E0_f = 0x0;
		B2_g = 0x0;
		break;
	}
	case 7:
	{
		//gfedcba
		//1111000
		A4A3A2_cba = 0x0;
		B7B6_ed = 0xc0;
		E0_f = 0x1;
		B2_g = 0x4;
		break;
	}
	case 8:
	{
		//gfedcba
		//0000000
		A4A3A2_cba = 0x0;
		B7B6_ed = 0x0;
		E0_f = 0x0;
		B2_g = 0x0;
		break;
	}
	case 9:
	{
		//gfedcba
		//0010000
		A4A3A2_cba = 0x0;
		B7B6_ed = 0x80;
		E0_f = 0x0;
		B2_g = 0x0;
		break;
	}
	}
}

/*Software Decoding for tens Digit to be displayed on one 7 segment*/
//E2 E1 C7 C6 C5 C4 B3
//g  f  e  d  c  b  a
void Display10_1(int Tens)
{
	switch (Tens)
	{
	case 0:
	{
		//gfedcba
		//1000000
		B3_a = 0x0;
		C7C6C5C4_edcb = 0x0;
		E2E1_gf = 0x4;
		break;
	}
	case 1:
	{
		//gfedcba
		//1111001
		B3_a = 0x8;
		C7C6C5C4_edcb = 0xc0;
		E2E1_gf = 0x6;
		break;
	}
	case 2:
	{
		//gfedcba
		//0100100
		B3_a = 0x0;
		C7C6C5C4_edcb = 0x20;
		E2E1_gf = 0x2;
		break;
	}
	case 3:
	{
		//gfedcba
		//0110000
		B3_a = 0x0;
		C7C6C5C4_edcb = 0x80;
		E2E1_gf = 0x2;
		break;
	}
	case 4:
	{
		//gfedcba
		//0011001
		B3_a = 0x8;
		C7C6C5C4_edcb = 0xc0;
		E2E1_gf = 0x0;
		break;
	}
	case 5:
	{
		//gfedcba
		//0010010
		B3_a = 0x0;
		C7C6C5C4_edcb = 0x90;
		E2E1_gf = 0x0;
		break;
	}
	case 6:
	{
		//gfedcba
		//0000010
		B3_a = 0x0;
		C7C6C5C4_edcb = 0x10;
		E2E1_gf = 0x0;
		break;
	}
	case 7:
	{
		//gfedcba
		//1111000
		B3_a = 0x0;
		C7C6C5C4_edcb = 0xc0;
		E2E1_gf = 0x6;
		break;
	}
	case 8:
	{
		//gfedcba
		//0000000
		B3_a = 0x0;
		C7C6C5C4_edcb = 0x0;
		E2E1_gf = 0x0;
		break;
	}
	case 9:
	{
		//gfedcba
		//0010000
		B3_a = 0x0;
		C7C6C5C4_edcb = 0x80;
		E2E1_gf = 0x0;
		break;
	}
	}
}
/*Software Decoding for Ones Hundreds to be displayed on one 7 segment*/
//A7 A6 A5 B4 E5 E4 B1
//g  f  e  d  c  b  a
void Display10_2(int Hundreds)
{
	switch (Hundreds)
	{
	case 0:
	{
		//gfedcba
		//1000000
		B1_a = 0x00;
		E5E4_cb = 0x00;
		B4_d = 0x00;
		A7A6A5_gfe = 0x80;
		break;
	}
	case 1:
	{
		//gfedcba
		//1111001
		B1_a = 0x2;
		E5E4_cb = 0x00;
		B4_d = 0x10;
		A7A6A5_gfe = 0xe0;
		break;
	}
	case 2:
	{
		//gfedcba
		//0100100
		B1_a = 0x00;
		E5E4_cb = 0x20;
		B4_d = 0x00;
		A7A6A5_gfe = 0x40;
		break;
	}
	case 3:
	{
		//gfedcba
		//0110000
		B1_a = 0x00;
		E5E4_cb = 0x00;
		B4_d = 0x00;
		A7A6A5_gfe = 0x60;
		break;
	}
	case 4:
	{
		//gfedcba
		//0011001
		B1_a = 0x02;
		E5E4_cb = 0x00;
		B4_d = 0x10;
		A7A6A5_gfe = 0x20;
		break;
	}
	case 5:
	{
		//gfedcba
		//0010010
		B1_a = 0x00;
		E5E4_cb = 0x10;
		B4_d = 0x00;
		A7A6A5_gfe = 0x20;
		break;
	}
	case 6:
	{
		//gfedcba
		//0000010
		B1_a = 0x00;
		E5E4_cb = 0x10;
		B4_d = 0x00;
		A7A6A5_gfe = 0x00;
		break;
	}
	case 7:
	{
		//gfedcba
		//1111000
		B1_a = 0x00;
		E5E4_cb = 0x00;
		B4_d = 0x10;
		A7A6A5_gfe = 0xe0;
		break;
	}
	case 8:
	{
		//gfedcba
		//0000000
		B1_a = 0x00;
		E5E4_cb = 0x00;
		B4_d = 0x00;
		A7A6A5_gfe = 0x00;
		break;
	}
	case 9:
	{
		//gfedcba
		//0010000
		B1_a = 0x00;
		E5E4_cb = 0x00;
		B4_d = 0x00;
		A7A6A5_gfe = 0x20;
		break;
	}
	}
}
//Interface for Decoding and displaying measurment
void Display(int Hundreds, int Tens, int Ones)
{
	Display10_2(Hundreds);
	Display10_1(Tens);
	Display10_0(Ones);
}

void InitiatePorts()
{
	InitPortA();
	InitPortB();
	InitPortC();
	InitPortE();
	InitPortF();
	}
	
int main()
{
	double TwiceTime;		//useless can be removed
	unsigned int Distance;	//measured in cm
	unsigned int Digits[3];	//[0]=10^2,[1]=10^1,[2]=10^0
	int i;
	InitiatePorts();
	while (1)
	{
		GPIO_PORTF_DATA_R=0X8;
		while((GPIO_PORTF_DATA_R&0X1)==0){GPIO_PORTF_DATA_R=0x0A;}
		//wait 1 second before taking next read
		for (i = 0; i<100; i++)
		{Delay10ms();}
		GPIO_PORTF_DATA_R=0X2;
		TrigSensor();
		while ((GPIO_PORTB_DATA_R & 0x20) ==0) {}//Busy waiting untill echo is received
		Count();//start counter
		//while (((GPIO_PORTB_DATA_R & 0x20)>>5) == 1){}//Busy Waiting=pulse width 	
		while ((GPIO_PORTB_DATA_R & 0x20) != 0){}//Busy Waiting=pulse width
		TwiceTime=NVIC_ST_CURRENT_R;
		//TwiceTime = (double)((MaxCount - TwiceTime)*(ClockPeriod*1.0));
		Distance = (HalfSoundSpeed*((MaxCount - TwiceTime)*(ClockPeriod*1.0)) );//measured in cm
		Digits[2] = (Distance % 10);//ex 123%10=3;
		Digits[1] = (Distance / 10) % 10;//12%10=2;
		Digits[0] = (Distance / 100) % 10;//1%10=1;
		Display(Digits[0], Digits[1], Digits[2]);
	}
}