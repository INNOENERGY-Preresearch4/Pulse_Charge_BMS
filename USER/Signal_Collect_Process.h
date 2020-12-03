/*
*********************************************************************************************************
*
*                                     BOARD SUPPORT PACKAGE (BSP)
*
*                                       Keil Development Kits
*                                              on the
*
*                                              Terminus
*
* Filename      : Signal_Collect_Process.h
* Version       : V1.00
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                                 MODULE
*********************************************************************************************************
*/

#ifndef _Signal_Collect_Process_H_
#define _Signal_Collect_Process_H_

/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include "includes.h"

/*
*********************************************************************************************************
*                                           	Para DEFINES
*********************************************************************************************************
*/

//������
#define CHARG_BLANCE_ON              (2700)   //2.7V���Ͽ������� 2700mV,��Ϊ��ƽ̨�ڹյ㣬�յ����¾��� ���岻����Ϊ��ѹ�������仯�ܴ�
#define BALANCE_DIFF_VOLTAGE         (30)     //���⿪����ѹ��
#define BALANCE_STOP_DIFF_VOLTAGE    (10)     //����رյ�ѹ��
/*
*********************************************************************************************************
*                                              SERVICES
*********************************************************************************************************
*/


uint16_t CellMaxValue(uint16_t data[],uint16_t n,uint8_t*num);
uint16_t CellMinValue(uint16_t data[],uint16_t n,uint8_t*num);
int16_t TempMinValue(s16 data[],uint8_t n,uint8_t*num);
int16_t TempMaxValue(s16 data[],uint8_t n,uint8_t*num);
float Get_Pulse_Current(void);
float Get_Filter_Current(void);
void Cur_Ref_Update(void);
void Full_Capacity_Judgment(void);
void Battery_Status_Judgment(void);
void Balance_Ctrl(void);
void Temp_Collect_ByCycle(void);
void Vol_Collect_ByCycle(void);
void Pack_Cell_Balance(void);


#endif 

