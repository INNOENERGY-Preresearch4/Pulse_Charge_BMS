#include "key.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F7������
//KEY��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/11/27
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

//������ʼ������
void IO_INPUT_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOG_CLK_ENABLE();           //����GPIODʱ��
    
    GPIO_Initure.Pin=GPIO_PIN_13|GPIO_PIN_14;            //PA0
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //����
    GPIO_Initure.Pull=GPIO_NOPULL;        //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOG,&GPIO_Initure);
}

//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1��WKUP���� WK_UP
//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>KEY2>WK_UP!!
u8 IO_INPUT_Scan(void)
{
    static uint32_t io_cont=0;     //�����ɿ���־
    uint8_t io_value=0;
	uint32_t io_coutn=0;
	//////////////input1////////////////////////
	if(INPUT_SIG1()==GPIO_PIN_RESET)
	{
		io_coutn=io_cont&0xf;
		if(io_coutn<5)
		{
			io_coutn++;
		}else
		{
			io_coutn=5;
		}
		if(io_coutn>=5)
		{
			io_value|=0x01;
		}else
		{
			io_value&=(~(0x01));
		}
		io_cont&=0xfffffff0;
		io_cont|=io_coutn;
	}else
	{
		io_cont&=0xfffffff0;
		io_value&=(~(0x01));
	}
	//////////////input2////////////////////////
	if(INPUT_SIG2()==GPIO_PIN_RESET)
	{
		io_coutn=(io_cont&0xf0)>>4;
		if(io_coutn<5)
		{
			io_coutn++;
		}else
		{
			io_coutn=5;
		}
		if(io_coutn>=5)
		{
			io_value|=0x02;
		}else
		{
			io_value&=(~(0x02));
		}
		io_cont&=0xffffff0f;
		io_cont|=io_coutn<<4;
	}else
	{
		io_cont&=0xffffff0f;
		io_value&=(~(0x02));
	}
	return io_value; 
}
