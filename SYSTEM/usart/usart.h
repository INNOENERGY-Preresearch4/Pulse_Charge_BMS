#ifndef _USART_H
#define _USART_H
#include "sys.h"
#include "stdio.h"	
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//ALIENTEK STM32F7������
//����1��ʼ��		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.csom
//�޸�����:2015/6/23
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
//********************************************************************************
//V1.0�޸�˵�� 
////////////////////////////////////////////////////////////////////////////////// 	

#define EN_UART5_RX 			1		//ʹ�ܣ�1��/��ֹ��0������5����	
#define EN_UART8_RX 			1		//ʹ�ܣ�1��/��ֹ��0������5����


//����봮���жϽ��գ��벻Ҫע�����º궨��
void UART5_Init(u32 bound);
void UART8_Init(u32 bound);
#endif