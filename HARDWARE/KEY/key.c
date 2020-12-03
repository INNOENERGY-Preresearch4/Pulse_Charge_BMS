#include "key.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F7开发板
//KEY驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2015/11/27
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

//按键初始化函数
void IO_INPUT_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOG_CLK_ENABLE();           //开启GPIOD时钟
    
    GPIO_Initure.Pin=GPIO_PIN_13|GPIO_PIN_14;            //PA0
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //输入
    GPIO_Initure.Pull=GPIO_NOPULL;        //下拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOG,&GPIO_Initure);
}

//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//0，没有任何按键按下
//1，WKUP按下 WK_UP
//注意此函数有响应优先级,KEY0>KEY1>KEY2>WK_UP!!
u8 IO_INPUT_Scan(void)
{
    static uint32_t io_cont=0;     //按键松开标志
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
