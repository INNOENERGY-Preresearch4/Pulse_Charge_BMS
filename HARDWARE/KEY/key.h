#ifndef _KEY_H
#define _KEY_H
#include "sys.h"
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
#define INPUT_SIG1()        (HAL_GPIO_ReadPin(GPIOG,GPIO_PIN_13))  //KEY0����PH3
#define INPUT_SIG2()        (HAL_GPIO_ReadPin(GPIOG,GPIO_PIN_14))  //KEY1����PH2

void IO_INPUT_Init(void);
u8 IO_INPUT_Scan(void);
#endif
