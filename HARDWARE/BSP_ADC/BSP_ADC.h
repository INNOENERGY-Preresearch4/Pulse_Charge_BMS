/*
*********************************************************************************************************
*
*                                     BOARD SUPPORT PACKAGE (BSP)
*
* Filename      : BSP_ADC.h
* Version       : V1.00
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                                 MODULE
*********************************************************************************************************
*/

#ifndef _BSP_ADC_H
#define _BSP_ADC_H

/*
*********************************************************************************************************
*                                              INCLUDE FILES
*********************************************************************************************************
*/

#include "sys.h"

/*
*********************************************************************************************************
*                                           	Para DEFINES
*********************************************************************************************************
*/

#define ADC1_CONVER_Time      (4)     //ADC1转换几次，转换完成进回调函数
#define ADC3_CONVER_Time      (1)     //ADC3转换几次，转换完成进回调函数

extern u8 ADC1_Flag;
extern u8 ADC3_Flag;

extern uint32_t ADC1_ConvertValueRead[ADC1_CONVER_Time+20];
extern uint32_t ADC3_ConvertValueRead[ADC3_CONVER_Time+20];
extern uint32_t ADC3_ConvertValue[ADC3_CONVER_Time+20];
extern uint32_t ADC1_ConvertValue[ADC1_CONVER_Time+20];
extern  float ADC1_ConvertValueRead_Average;
extern ADC_HandleTypeDef ADC1_Handler,ADC2_Handler,ADC3_Handler;//ADC句柄
/*
*********************************************************************************************************
*                                              SERVICES
*********************************************************************************************************
*/

void ADC1_Init(void);
void ADC2_Init(void);
void ADC3_Init(void);

u16 Get_ADC2_CHANNEL0(void);
float Get_ADC2_CHANNEL0_Average(u8 times);

void Filter_ADC1_Start(void);
void Pulse_ADC3_Start(void);


#endif 
