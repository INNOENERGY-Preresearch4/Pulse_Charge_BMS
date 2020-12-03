/**
  ******************************************************************************
  * @file    AD7787.c
  * @author  YanWenlong
  * @version V1.0.0
  * @date    may-2020
  * @brief   AD7787�����ļ�.
  *          ����ģʽ��SINGLE CONVERSION
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
//�������ƣ�AD7787_Init
//�������ã�AD7787�ϵ��ʼ��
//����ֵ  0�ɹ� 1ʧ��
//���� chanle 5 ������ѹ�ɼ�;
////////////////////////////////////////////////////////////
uint8_t AD7787_Init(uint8_t chanle)//��ʼ������
{
	if(chanle==5)//������ѹ�ɼ�
	{
		SPI1_CS_0();
		dely_nop();
		SPI_ReadWriteByte(5,0x10);//д״̬
		SPI_ReadWriteByte(5,0x02);//����ģʽ
		dely_nop();
		SPI1_CS_1();
		return 0;
	}
	{
		return 1;
	}	
}
/////////////////////////////////////////////////////////////
//�������ƣ�AD7787_Read_Value
//�������ã�AD7787_Read_Value�ݶ�ȡ
//����ֵ  ��ȡ��AD�ɼ�ֵ
//���� chanle 5 ������ѹ�ɼ�;
//����ain 1-4 �ɼ�оƬ1 1 AIN1(+)-AIN1(-);2 AIN2;3 AIN1(-)-AIN1(-); 4 VDD Monitor
////////////////////////////////////////////////////////////
int32_t AD7787_Read_Value(uint8_t chanle,uint8_t ain)//��ʼ������
{
	int32_t re_value=0;
	u8 data[3]={0};
	if(chanle==5)//������ѹ�ɼ�
	{
		if((ain>0)&&(ain<5))
		{
			SPI1_CS_0();
			dely_nop();
			SPI_ReadWriteByte(chanle,0x37+ain);//��AIN1����
			dely_nop();
			data[2]=SPI_ReadWriteByte(chanle,0x00);//��AIN1����
			data[1]=SPI_ReadWriteByte(chanle,0x00);//��AIN1����
			data[0]=SPI_ReadWriteByte(chanle,0x00);//��AIN1����
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
//�������ƣ�AD7787_Read_Value
//�������ã�AD7787��ȡ״̬�Ĵ���
//����ֵ  ��ȡ��״̬�Ĵ���ֵ
//���� chanle 5 ������ѹ�ɼ�;
////////////////////////////////////////////////////////////
uint8_t AD7787_Read_Status(uint8_t chanle)//�ɼ�ֵ��ȡ����	
{
	u8 data=0;
	if(chanle==5)//������ѹ�ɼ�
	{
			SPI1_CS_0();
			dely_nop();
			SPI_ReadWriteByte(chanle,0x08);//��status����
			dely_nop();
			data=SPI_ReadWriteByte(chanle,0x00);//��AIN1����
			dely_nop();
			SPI1_CS_1();
			return data;	
	}else
	{
		return 0x00;
	}
}
	
