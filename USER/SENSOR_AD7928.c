/*
*********************************************************************************************************
*
*                                     					Application
*
* Filename      : SENSOR_AD7928.c
* Version       : V1.00
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include "SENSOR_AD7928.h"
#include "includes.h"


/*
*********************************************************************************************************
*                                          LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/ 
u16 Sensor_Buf[3][8];
float Sensor_ChanelVol[3][8];
u16	Chanel_Vin[8]={0,1,2,3,4,5,6,7};

/*
*********************************************************************************************************
*                                             	 SERVICES
*********************************************************************************************************
*/
void delay (long int length)
{
	while (length >0)
    	length--;
}

u16 AD7928_Write_Dummy(u8 Chanel_AD)
{
		u16 RxData=0xFFFF;
	
		if(Chanel_AD==Chanel_AD1)
		{
			SPI2_CS1_Low;                //使能SPI传输
			delay(9);
			RxData =SPI2_ReadWriteWord(0xFFFF);//发送寄存器号 
			SPI2_CS1_High;                //禁止SPI传输	
			delay(20);
		}
		else if(Chanel_AD==Chanel_AD2)
		{
			SPI2_CS2_Low;                //使能SPI传输
			delay(9);
			RxData =SPI2_ReadWriteWord(0xFFFF);//发送寄存器号 
			SPI2_CS2_High;                //禁止SPI传输	
			delay(20);			
		}
		else if(Chanel_AD==Chanel_AD3)
		{
			SPI2_CS3_Low;                //使能SPI传输
			delay(9);
			RxData =SPI2_ReadWriteWord(0xFFFF);//发送寄存器号 
			SPI2_CS3_High;                //禁止SPI传输
			delay(20);			
		}
		else{}
		
  	return(RxData);       			//返回状态值
}

u16 AD7928_ReadWrite_CtrlReg(u8 Chanel_AD,u16 Chanel_Vin)
{
		u16 RxData=0xFFFF;
	
		u16 CtrlReg=0x8310;
		Chanel_Vin=(Chanel_Vin&0x0007);
		CtrlReg=(CtrlReg|(Chanel_Vin<<10));
	
		if(Chanel_AD==Chanel_AD1)
		{
			SPI2_CS1_Low;                //使能SPI传输
			delay(9);
			RxData =SPI2_ReadWriteWord(CtrlReg);//发送寄存器号 
			SPI2_CS1_High;                //禁止SPI传输	
			delay(20);
		}
		else if(Chanel_AD==Chanel_AD2)
		{
			SPI2_CS2_Low;                //使能SPI传输
			delay(9);
			RxData =SPI2_ReadWriteWord(CtrlReg);//发送寄存器号 
			SPI2_CS2_High;                //禁止SPI传输	
			delay(20);
		}
		else if(Chanel_AD==Chanel_AD3)
		{
			SPI2_CS3_Low;                //使能SPI传输
			delay(9);
			RxData =SPI2_ReadWriteWord(CtrlReg);//发送寄存器号 
			SPI2_CS3_High;                //禁止SPI传输
			delay(20);
		}
		else{}
		
  	return(RxData);       			//返回状态值
}


void AD7928_ALL_DummyCommend(void)
{
		AD7928_Write_Dummy(Chanel_AD1);
		AD7928_Write_Dummy(Chanel_AD1);
		AD7928_ReadWrite_CtrlReg(Chanel_AD1,Chanel_Vin[0]);
	
		AD7928_Write_Dummy(Chanel_AD2);
		AD7928_Write_Dummy(Chanel_AD2);
		AD7928_ReadWrite_CtrlReg(Chanel_AD2,Chanel_Vin[0]);

		AD7928_Write_Dummy(Chanel_AD3);
		AD7928_Write_Dummy(Chanel_AD3);
		AD7928_ReadWrite_CtrlReg(Chanel_AD3,Chanel_Vin[0]);
}

void AD7928_Read_ConverRssult(u8 Chanel_AD)
{
		u8 i=0;
		u8 n=0;
		memset(&Sensor_Buf[Chanel_AD-1][0],0,8);
		for(i=0;i<7;i++)
		{
				Sensor_Buf[Chanel_AD-1][i]=AD7928_ReadWrite_CtrlReg(Chanel_AD,Chanel_Vin[i+1]);
		}
		Sensor_Buf[Chanel_AD-1][7]=AD7928_ReadWrite_CtrlReg(Chanel_AD,Chanel_Vin[0]);
		for(i=0;i<8;i++)
		{
				n=((Sensor_Buf[Chanel_AD-1][i]>>12)&0x0007);
				Sensor_ChanelVol[Chanel_AD-1][n]=((float)(Sensor_Buf[Chanel_AD-1][i]&0x0FFF))*5/4096;
		}
}



