/*
*********************************************************************************************************
*
*                                     BOARD SUPPORT PACKAGE (BSP)
*
* Filename      : BSP_TIMER.h
* Version       : V1.00
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                                MODULE
*********************************************************************************************************
*/

#ifndef _BSP_TIMER_H
#define _BSP_TIMER_H

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include "sys.h"

/*
*********************************************************************************************************
*                                            	Para DEFINES
*********************************************************************************************************
*/

#define		Pulse_Freq  	1000														//������Ƶ��
#define		TIM3_psc			27															//TIM3ʱ�ӻ�׼Ƶ��108M/27=4M
#define		Simple_Accu		100															//һ���������ڲ���20����
#define		TIM3_arr			108000000/TIM3_psc/(Pulse_Freq*Simple_Accu)   	//��ʱ���ж�����ʼ��������Ƶ�ʵ�20��
//#define		TIM3_Freq			108000000/TIM3_psc/TIM3_arr								//TIM3�ж�Ƶ��
#define		TIM3_Freq			100000.0f							//TIM3�ж�Ƶ��

#define		TIM4_psc			108															//TIM4ʱ�ӻ�׼Ƶ��108M/108=1M
#define		TIM4_arr			10000   													//��ʱ������10000
#define		TIM4_Freq			108000000/TIM3_psc/TIM3_arr			//TIM4�ж�Ƶ��

#define		TIM2_psc			10800															//TIM4ʱ�ӻ�׼Ƶ��108M/10800=10k
#define		TIM2_arr			100000   													//��ʱ������10s


/*
*********************************************************************************************************
*                                            EXTERN
*********************************************************************************************************
*/

extern TIM_HandleTypeDef TIM2_Handler,TIM3_Handler,TIM4_Handler;      //��ʱ����� 

/*
*********************************************************************************************************
*                                              SERVICES
*********************************************************************************************************
*/

void TIM3_Init(u16 arr,u16 psc);    //��ʱ����ʼ��
void TIM4_Init(u16 arr,u16 psc);    //��ʱ����ʼ��
void TIM2_Init(u32 arr,u16 psc);		//��ʱ����ʼ��

#endif
