#include <msp430.h>
#include "RF_read.h"
#include "Config.h"

unsigned char RFreceive[20];
unsigned char RFreceiveLAST[20];
unsigned char RFreceiveTEMP[20];
unsigned int RFreceiveDEBUG[100];
unsigned int i=0;
int timeout =0;
unsigned char place=0;
unsigned int placeholder=0;
unsigned char firstplace=0;
unsigned char secondplace=0;
unsigned char thirdplace=0;
unsigned char fourthplace=0;
int temp =0;
unsigned int loopcount =2000;
unsigned char RFreceiveArrayAmount =0;
unsigned char Goodvalue=0;
unsigned char match=0;
unsigned char repeat=0;



#define SHORT 0
#define LONG 1
#define NEITHER 2

#define SYL 660
#define SYH 950
#define SL 1
#define SH 50
#define LL 51
#define LH 110


#define amountOFbits 12

//bit 0 is 250uS high 695uS low 220uS high 695uS low
//bit 1 is 660uS High 220 Low 660uS High 220 Low
//bit 2 aka f if 220uS high 660uS low 660uShigh 220 low
//sync but is 6740uS

unsigned char TXdataE[12] = {0,2,2,2,2,2,0,0,1,0,0,0}; // channel E http://en.chiptrue.com/images/LX2260_en.pdf
unsigned char TXdataD[12] = {2,2,2,2,2,2,0,0,1,0,0,0}; // channel D (A1 A0 x x x x x x)
unsigned char TXdataOPENED[12] = {0,2,2,2,2,2,1,1,1,1,1,0}; // door opened
unsigned char TXdataCLOSED[12] = {2,2,2,2,2,2,1,1,1,1,1,0}; // door closed

int main(void) {

	WDTCTL = WDTPW | WDTHOLD;		// Stop watchdog timer
	  BCSCTL1 = CALBC1_16MHZ;                   // Set range
	  DCOCTL = CALDCO_16MHZ;                    // Set DCO step + modulation*/



	  P2DIR |= (LED0 + LED1 + LED2 + LED9 + TX + RedLED);                            // Set all pins but RXD to output
	  P2OUT &= ~(LED0 + LED1 + LED2 + LED9 + TX + RedLED);
	  P1DIR |= (LED3 + LED4 + LED5 + LED6 + LED7 + LED8);                            // Set all pins but RXD to output
	  P1OUT &= ~(LED3 + LED4 + LED5 + LED6 + LED7 + LED8);
	  P1IFG =0;
	  __delay_cycles(10);
	  P1IE |= RX0; // P1.3 interrupt enabled
	  P1IES &= ~RX0; // P1.3 interrupt enabled
	  P1IFG &= ~RX0; // P1.3 IFG cleared
	  P1IE |= RX1; // P1.3 interrupt enabled
	  P1IES |= RX1; // P1.3 interrupt enabled
	  P1IFG &= ~RX1; // P1.3 IFG cleared

	  TA0CTL = TASSEL_2 + MC_2 + ID_3;           // SMCLK/8, upmode

	  P2OUT |= RedLED;
	  Chase(5,400);
	  P2OUT &= ~RedLED;
	 // _BIS_SR(LPM4_bits + GIE); // Enter LPM3 w/interrupt

	  __enable_interrupt(); // enable all interrupts                                   // Enable CPU interrupts
	while(1)
	{

		_delay_cycles(1000);


if(Goodvalue == 1)
{
	P2OUT^= RedLED;
	if(loopcount>1950)
	{
	match = ArrayCompare(RFreceive,TXdataD,0,RFreceiveArrayAmount);
	if(match)
	{
		P2OUT^=LED0;
	}
	match = ArrayCompare(RFreceive,TXdataE,0,RFreceiveArrayAmount);
	if(match)
	{
		P2OUT^=LED1;
	}
	match = ArrayCompare(RFreceive,TXdataOPENED,0,RFreceiveArrayAmount);
	if(match)
	{
		P2OUT^=LED2;
		_delay_cycles(200000);
		send_rf(TX, TXdataD);
		send_rf(TX, TXdataD);
		send_rf(TX, TXdataD);
		send_rf(TX, TXdataD);
		send_rf(TX, TXdataD);
		send_rf(TX, TXdataD);
		send_rf(TX, TXdataD);

	}
	match = ArrayCompare(RFreceive,TXdataCLOSED,0,RFreceiveArrayAmount);
	if(match)
	{
		P1OUT^=LED3;
		_delay_cycles(100000);
		send_rf(TX, TXdataE);
		send_rf(TX, TXdataE);
		send_rf(TX, TXdataE);
		send_rf(TX, TXdataE);
		send_rf(TX, TXdataE);
		send_rf(TX, TXdataE);

	}
	}
	Goodvalue =0;
	ArrayCopy(RFreceive,RFreceiveLAST,RFreceiveArrayAmount);
	resetBuffer(RFreceive,RFreceiveArrayAmount);
	loopcount=0;
//	_delay_cycles(1000000);
//	 P1IE |= (RX0 + RX1); // P1.3 interrupt enabled
}
loopcount++;
if(loopcount >2000)
{
	loopcount = 2000;
}
	}
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{

	//_BIC_SR(LPM4_EXIT); // wake up from low power mode

    temp = ((TAR >> 1)/10);

    	if(temp >SYL && temp <= SYH)
    	{
    		//we made it through the sync bit
    		placeholder=1;
    		place=0;
    		i=0;
    	}
    	RFreceiveDEBUG[i] = temp;

    	if(placeholder ==1 && temp <LH)
    {
        if(temp >SL && temp <SH)
        {
        	firstplace = SHORT;
        }
        else if(temp >LL && temp <LH)
        {
        	firstplace = LONG;
        }
        else
        {
        	firstplace = NEITHER;
        	placeholder = 6;
        }
        placeholder++;
    }
    else if(placeholder ==2)
    {
        if(temp >SL && temp <SH)
        {
        	secondplace = SHORT;
        }
        else if(temp >LL && temp <LH)
        {
        	secondplace = LONG;
        }
        else
        {
        	secondplace = NEITHER;
        	placeholder =6;
        }
        placeholder++;
    }
    else if(placeholder ==3)
    {
        if(temp >SL && temp <SH)
        {
        	thirdplace = SHORT;
        }
        else if(temp >LL && temp <LH)
        {
        	thirdplace = LONG;
        }
        else
        {
        	thirdplace = NEITHER;
        	placeholder =6;
        }
        placeholder++;
    }
    else if(placeholder ==4)
    {
        if(temp >SL && temp <SH)
        {
        	fourthplace = SHORT;
        }
        else if(temp >LL && temp <LH)
        {
        	fourthplace = LONG;
        }
        else
        {
        	fourthplace = NEITHER;
        	placeholder =6;
        }
        placeholder++;

    }
    if(placeholder ==5)
    {
       if(firstplace == SHORT && secondplace == LONG && thirdplace == SHORT && fourthplace == LONG)
       {
    	   RFreceiveTEMP[place] = 0;//0
    	   placeholder =1;
    	   place++;
       }
       else if(firstplace == LONG && secondplace == SHORT && thirdplace == LONG && fourthplace == SHORT)
       {
    	   RFreceiveTEMP[place] = 1;//1
    	   placeholder =1;
    	   place++;
       }
       else if(firstplace == SHORT && secondplace == LONG && thirdplace == LONG && fourthplace == SHORT)
       {
    	   RFreceiveTEMP[place] = 2;//f
    	   placeholder =1;
    	   place++;
       }
       else
       {
    	   RFreceiveTEMP[place] = 3;//neither
    	   placeholder++;
       }
if(place>=19)
{
	place=19;
}
if(place == amountOFbits)
{
RFreceiveArrayAmount= place;
ArrayCopy(RFreceiveTEMP,RFreceive,RFreceiveArrayAmount);
Goodvalue =1;
}
    }


    	i++;
    	if(i>99)
    	{
    		i=0;
    	}

  TA0CTL |= TACLR;
  P1IFG &= ~RX0; // P1.3 IFG cleared
  P1IFG &= ~RX1; // P1.3 IFG cleared
}

