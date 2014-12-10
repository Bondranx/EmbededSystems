//IIC0 Bus Library
//Processor: MC9X12SCP512
//Crystal:   16MHz
//by:        Brandon Foote
//date:      November 25, 2014

void IIC0_Init(void);
void IIC0_Write(byte bAddr, byte bReg, byte bData);
byte IIC0_Read(byte bAddr, byte bReg);
void WriteDAC(byte bAddr, byte bCommand, int iData);