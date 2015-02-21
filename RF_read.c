#include "RF_read.h"
#include "Config.h"
#include <msp430.h>

void Delay(int timing)
{
	int i;
	for(i=timing;i>0;i--)
	{
		__delay_cycles(100);
	}
}
void Chase(unsigned char number,int timing)
{
	int i;
	for (i=0;i<number;i++)
	{
		P1OUT &= ~(LED3 + LED4 + LED5 + LED6 + LED7 + LED8);
		P2OUT &= ~(LED0 + LED1 + LED2 + LED9);
		Delay(timing);
		P2OUT |= LED0;
		Delay(timing);
		P2OUT |= LED1;
		Delay(timing);
		P2OUT |= LED2;
		Delay(timing);
		P1OUT |= LED3;
		Delay(timing);
		P1OUT |= LED4;
		Delay(timing);
		P1OUT |= LED5;
		Delay(timing);
		P1OUT |= LED6;
		Delay(timing);
		P1OUT |= LED7;
		Delay(timing);
		P1OUT |= LED8;
		Delay(timing);
		P2OUT |= LED9;
		Delay(timing);
	}
	P1OUT &= ~(LED3 + LED4 + LED5 + LED6 + LED7 + LED8);
	P2OUT &= ~(LED0 + LED1 + LED2 + LED9);
}
void resetBuffer(unsigned char* Array, unsigned char amount)
{
	int i ;
	for(i =0; i<(amount -1);i++)
	{
		Array[i] =0;
	}
}

void send_rf(unsigned char pin, unsigned char* data)
{
unsigned char i=0;

for(i=0;i<12;i++)
{
if(data[i] == 0)
{
	zero(pin);
}
else if(data[i] ==1)
{
	one(pin);
}
else
{
	f(pin);
}

}

P2OUT |= pin;
_delay_cycles(4000); //250*16 = 4000
P2OUT &= ~pin;
_delay_cycles(112000);// 7000 * 16 =112000
}

void zero(unsigned char pin)
{
	P2OUT |= pin;
	_delay_cycles(4000); //250 *16 = 4000
	P2OUT &= ~pin;
	_delay_cycles(11040); //690 *16 = 11040
	P2OUT |= pin;
	_delay_cycles(4000);
	P2OUT &= ~pin;
	_delay_cycles(11040);
}
void one(unsigned char pin)
{
	P2OUT |= pin;
	_delay_cycles(12960); //810 *16 =12960
	P2OUT &= ~pin;
	_delay_cycles(3360); //210 *16 =3360
	P2OUT |= pin;
	_delay_cycles(12960); //810 *16 =12960
	P2OUT &= ~pin;
	_delay_cycles(3360); //210 *16 =3360
}
void f(unsigned char pin)
{
	P2OUT |= pin;
	_delay_cycles(4000); //250 * 16 = 4000
	P2OUT &= ~pin;
	_delay_cycles(11200); //700 *16 = 11200
	P2OUT |= pin;
	_delay_cycles(12960);//810 *16 =12960
	P2OUT &= ~pin;
	_delay_cycles(3200); //200 * 16= 3200
}

unsigned char ArrayCompare(unsigned char* Array1, unsigned char* Array2, int amount, unsigned char characters)
{
	int i;
	unsigned char differences =0;
	for(i=0;i<(characters -1);i++ )
	{
		if(((int)Array1[i] - (int)Array2[i]) > amount)
		{
		differences++;
		}
		else if(((int)Array2[i] - (int)Array1[i]) > amount)
		{
			differences++;
		}
	}

	return (differences == 0 ? 1 : 0);
}

void ArrayCopy(unsigned char* Array1, unsigned char* Array2, unsigned char characters)
{
	int i;
	for(i=0;i<(characters -1);i++ )
	{
		Array2[i] = Array1[i];
	}
}

unsigned char readSwitchP1(unsigned char bit)
{
	int i =0;
	int count =0;

	for(i=10;i>0;i--)
	             {
	           	    if((P1IN & bit) == bit)
	           	    {
	           	    	count++;
	           	    }
	           	    _delay_cycles(10);
	             }
	if(count > 8)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void BlinkLED(unsigned char bit)
{
	P2OUT |= bit;
	_delay_cycles(100000);
	P2OUT &= ~bit;
	_delay_cycles(100000);
	P2OUT |= bit;
	_delay_cycles(100000);
	P2OUT &= ~bit;
	_delay_cycles(100000);
	P2OUT |= bit;
	_delay_cycles(100000);
	P2OUT &= ~bit;
	_delay_cycles(100000);
	P2OUT |= bit;
	_delay_cycles(100000);
	P2OUT &= ~bit;
	_delay_cycles(100000);
	P2OUT |= bit;
	_delay_cycles(100000);
	P2OUT &= ~bit;
	_delay_cycles(100000);




}
