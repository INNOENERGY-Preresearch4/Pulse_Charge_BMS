/**
  ******************************************************************************
  * @file    AD7787.c
  * @author  YanWenlong
  * @version V1.0.0
  * @date    may-2020
  * @brief   AD7787驱动文件.
  *          工作模式：SINGLE CONVERSION
  *          functionalities of the Universal Asynchronous Receiver Transmitter (UART) peripheral:
  *           + Initialization and de-initialization functions
  *           + IO operation functions
  *           + Peripheral Control functions
  *           + Peripheral State and Errors functions
  *
**/
#include "AD7787.h"
#define AD7787_VARIBLE
#include "includes.h"
//////////////////////////////////////
void dely_nop(void)
{
	u8 i=0;
	for(i=0;i<20;i++)
	{}
}
/////////////////////////////////////////////////////////////
//函数名称：AD7787_Init
//函数作用：AD7787上电初始化
//返回值  0成功 1失败
//参数 chanle 5 电流电压采集;
////////////////////////////////////////////////////////////
uint8_t AD7787_Init(uint8_t chanle)//初始化函数
{
	if(chanle==5)//电流电压采集
	{
		SPI1_CS_0();
		dely_nop();
		SPI_ReadWriteByte(5,0x10);//写状态
		SPI_ReadWriteByte(5,0x02);//连续模式
		dely_nop();
		SPI1_CS_1();
		return 0;
	}
	{
		return 1;
	}	
}
/////////////////////////////////////////////////////////////
//函数名称：AD7787_Read_Value
//函数作用：AD7787_Read_Value据读取
//返回值  读取的AD采集值
//参数 chanle 5 电流电压采集;
//参数ain 1-4 采集芯片1 1 AIN1(+)-AIN1(-);2 AIN2;3 AIN1(-)-AIN1(-); 4 VDD Monitor
////////////////////////////////////////////////////////////
int32_t AD7787_Read_Value(uint8_t chanle,uint8_t ain)//初始化函数
{
	int32_t re_value=0;
	u8 data[3]={0};
	if(chanle==5)//电流电压采集
	{
		if((ain>0)&&(ain<5))
		{
			SPI1_CS_0();
			dely_nop();
			SPI_ReadWriteByte(chanle,0x37+ain);//读AIN1数据
			dely_nop();
			data[2]=SPI_ReadWriteByte(chanle,0x00);//读AIN1数据
			data[1]=SPI_ReadWriteByte(chanle,0x00);//读AIN1数据
			data[0]=SPI_ReadWriteByte(chanle,0x00);//读AIN1数据
			re_value=((u32)data[2]<<16)+((u32)data[1]<<8)+data[0];
			dely_nop();
			SPI1_CS_1();
			return re_value;
		}else{return 0xffffffff;}
		
		
	}else
	{
		return 0xffffffff;
	}
}
/////////////////////////////////////////////////////////////
//函数名称：AD7787_Read_Value
//函数作用：AD7787读取状态寄存器
//返回值  读取的状态寄存器值
//参数 chanle 5 电流电压采集;
////////////////////////////////////////////////////////////
uint8_t AD7787_Read_Status(uint8_t chanle)//采集值读取函数	
{
	u8 data=0;
	if(chanle==5)//电流电压采集
	{
			SPI1_CS_0();
			dely_nop();
			SPI_ReadWriteByte(chanle,0x08);//读status数据
			dely_nop();
			data=SPI_ReadWriteByte(chanle,0x00);//读AIN1数据
			dely_nop();
			SPI1_CS_1();
			return data;	
	}else
	{
		return 0x00;
	}
}
	
