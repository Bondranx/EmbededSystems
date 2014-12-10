//Seven Segment Display Controller Library
//Processor:  MC9S12XDP512
//Crystal:    16MHz
//by:         Brandon Foote
//December 2014

void SevSeg_Init(void);
void SevSeg_BlAll(void); 
void SevSeg_BlChar(byte bDigit);
void SevSeg_Char(byte bChar, byte bDigit);
void SevSeg_dChar(byte bChar, byte bDigit);
void SevSeg_Two(byte bChars, byte bDigit);
void SevSeg_Twod(byte bChars, byte bDigit);
void SevSeg_Top4(int iChars);
void SevSeg_Low4(int iChars);