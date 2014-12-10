//Seven Segment Display Controller Library
//Processor:  MC9S12XDP512
//Crystal:  16 MHz
//by P Ross Taylor
//May 2014

#include <hidef.h>
#include "derivative.h"
#include "SevSeg_Lib.h"

void SevSeg_Init(void)
{
	DDRA |= 0b00000011;		//A0:1 outputs
	DDRB = 0b11111111;		//all PORTB outputs
	PORTA |= 0b00000011;	//preset control lines high
	PORTA = 0b11111111;		//preset data lines high
	
	SevSeg_BlAll();
}

void SevSeg_BlAll(void)
{
	byte bCount;
	for(bCount=0;bCount<=7;bCount++)
	{
		SevSeg_BlChar(bCount);
	}
}

void SevSeg_BlChar(byte bDigit)
{
	bDigit &= 0b00001111;	//clean up in case of error
	bDigit |= 0b01110000;	//single digit, no decode
	PORTB = bDigit;
	PORTA = 0b00000010;		//mode high, strobe /Write
	PORTA = 0b00000001;
	
	PORTB = 0b10000000;		//burn off all segments and dp
	PORTA = 0b00000000;		//mode low, strobe /Write
	PORTA = 0b00000001;
}

void SevSeg_Char(byte bChar, byte bDigit)
{
	bDigit &= 0b00000111;	//clean up in case of error
	bDigit |= 0b01011000;	//single digit, hex decode, no SD, Bank A
	bChar &= 0b00001111;	//clean up
	bChar |= 0b10000000;	//dp off
	PORTB = bDigit;
	PORTA = 0b00000010;		//mode high, strobe /Write
	PORTA = 0b00000001;

	PORTB = bChar;
	PORTA = 0b00000000;		//mode low, strobe /Write
	PORTA = 0b00000001;
}

void SevSeg_dChar(byte bChar, byte bDigit)
{
	bDigit &= 0b00000111;	//clean up in case of error
	bDigit |= 0b01011000;	//single digit, hex decode, no SD, Bank A
	bChar &= 0b00001111;	//clean up
	bChar &= 0b01111111;	//dp on
	PORTB = bDigit;
	PORTA = 0b00000010;		//mode high, strobe /Write
	PORTA = 0b00000001;

	PORTB = bChar;
	PORTA = 0b00000000;		//mode low, strobe /Write
	PORTA = 0b00000001;
}

void SevSeg_Two(byte bChars, byte bDigit)
{
	byte bHiNib;
	byte bLoNib;
	
	bHiNib = bChars/16;
	bLoNib = bChars&0b00001111;
	
	SevSeg_Char(bHiNib,bDigit++);
	SevSeg_Char(bLoNib,bDigit);
}

void SevSeg_Twod(byte bChars, byte bDigit)
{
	byte bHiNib;
	byte bLoNib;
	
	bHiNib = bChars/16;
	bLoNib = bChars&0b00001111;
	
	SevSeg_Char(bHiNib,bDigit++);
	SevSeg_dChar(bLoNib,bDigit);
}

	
void SevSeg_Top4(int iChars)
{
	byte iNibble;
	iNibble = (byte)(iChars/4096);
	SevSeg_Char(iNibble,0);
	iNibble = (byte)(iChars/256);
	SevSeg_Char(iNibble,1);
	iNibble = (byte)(iChars/16);
	SevSeg_Char(iNibble,2);
	iNibble =(byte)(iChars);
	SevSeg_Char(iNibble,3);
}

void SevSeg_Low4(int iChars)
{
	byte iNibble;
	iNibble = (byte)(iChars/4096);
	SevSeg_Char(iNibble,4);
	iNibble = (byte)(iChars/256);
	SevSeg_Char(iNibble,5);
	iNibble = (byte)(iChars/16);
	SevSeg_Char(iNibble,6);
	iNibble =(byte)(iChars);
	SevSeg_Char(iNibble,7);
}