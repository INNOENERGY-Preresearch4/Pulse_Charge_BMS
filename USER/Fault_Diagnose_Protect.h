/*
*********************************************************************************************************
*																								APPLICATION
*
* Filename      : Fault_Diagnose_Protect.h
* Version       : V1.00
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                                 MODULE
*********************************************************************************************************
*/
#ifndef _Fault_Diagnose_Protect_H__
#define _Fault_Diagnose_Protect_H__

/*
*********************************************************************************************************
*                                              INCLUDE FILES
*********************************************************************************************************
*/

#include "stm32f7xx_hal.h"
#include "includes.h"

/*
*********************************************************************************************************
*                                           	Para DEFINES
*********************************************************************************************************
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//BIT���
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//������������
//Pack_Alarms[0]
#define ALARM_EEPROM_BIT               (0x01)    //EE����  *
#define ALARM_VOLTAGE_AD_BIT           (0x02)    //��ѹ�ɼ���·����  *
#define ALARM_TEMP_AD_BIT              (0x04)    //�¶Ȳɼ���·����  *
#define ALARM_CURRENT_AD_BIT           (0x08)    //�����ɼ���·����  *
#define ALARM_OVERTEMP_PRE_BIT         (0x10)    //��ع���Ԥ��
#define ALARM_LOWTEMP_PRE_BIT          (0x20)    //��ص��¹���Ԥ��
#define ALARM_PACKVOLTAGE_OVER_PRE_BIT (0x40)    //������ѹԤ������
#define ALARM_PACKVOLTAGE_LOW_PRE_BIT  (0x80)    //�����ǷѹԤ������
//Pack_Alarms[1]
#define ALARM_COVERCURRENT_PRE_BIT     (0x01)    //����������Ԥ������  *
#define ALARM_DCOVERCURRENT_PRE_BIT    (0x02)    //�ŵ��������Ԥ������  *
#define ALARM_CELLVOLTAGE_LOW_PRE_BIT  (0x04)    //����ǷѹԤ������
#define ALARM_CELLVOLTAGE_OVER_PRE_BIT (0x08)    //�����ع�ѹԤ������
#define ALARM_UNBLANCE_BIT       		(0x10)    //���岻ƽ�����
//Pack_Alarms[2]
#define ALARM_CUTOFF_OVERTEMP_BIT      	 (0x01)    //�����жϹ���
#define ALARM_CUTOFF_OVERPACKVOLTAGE_BIT (0x02)    //�ܹ�ѹ�жϹ���
#define ALARM_CUTOFF_LOWPACKVOLTAGE_BIT  (0x04)    //��Ƿѹ�жϹ���
#define ALARM_CUTOFF_OVERCELLVOLTAGE_BIT (0x08)    //�����ѹ�жϹ���
#define ALARM_CUTOFF_LOWCELLVOLTAGE_BIT  (0x10)    //����Ƿѹ�жϹ���
#define ALARM_CUTOFF_COVERCURRENT_BIT 	 (0x20)    //�������жϹ���
#define ALARM_CUTOFF_DCOVERCURRENT_BIT   (0x40)    //�ŵ�����жϹ���



//����ֵ
//Pack_Alarms[0]
#define VOLTAGE_AD_ERR              	 (400)    	//��ѹ�ɼ�������ֵmV
#define VOLTAGE_AD_ERR_FREE          	 (1500)    //��ѹ�ɼ����Ͻ����ֵmV

#define TEMP_AD_ERR              	 	 (250)    //�¶Ȳɼ�������ֵ0.1��
#define TEMP_AD_ERR_FREE         	 	 (150)   //�¶Ȳɼ����Ͻ����ֵ0.1��

#define TEMP_HIGH_PRE                	 (450)      //�ŵ���ȹ���Ԥ������ֵ0.1��  450
#define TEMP_HIGH_PRE_FREE           	 (400)      //�ŵ���ȹ���Ԥ���������ֵ0.1��  400

#define TEMP_LOW_PRE                    (50)     //�ŵ���¹���Ԥ������ֵ0.1��   50
#define TEMP_LOW_PRE_FREE               (100)     //�ŵ���¹���Ԥ���������ֵ0.1��  100

#define OVERPACKVOLTAGE_PRE           	(2880)  //������ѹԤ����ֵ  0.01V  ����3600mV
#define OVERPACKVOLTAGE_PRE_FREE      	(2840)  //������ѹԤ�������ֵ		0.01V	����3550mV

#define LOWPACKVOLTAGE_PRE            	(2080)    //�����ǷѹԤ����ֵ20.80V  0.01V  ����2600mV
#define LOWPACKVOLTAGE_PRE_FREE       	(2120)    //�����ǷѹԤ�������ֵ21.20V  0.01V	����2650mV

//Pack_Alarms[1]
#define Charge_Current_Over_PRE            (-6500)     //����������Ԥ������ֵfloat mA
#define Charge_Current_Over_PRE_Free       (-6000)     //����������Ԥ���������ֵfloat mA

#define Discharge_Current_Over_PRE          (5500)     //�ŵ����������Ԥ����ֵfloat mA  5.5A
#define Discharge_Current_Over_PRE_Free     (5000)     //�ŵ����������Ԥ������ֵfloat mA  5A
//
#define CELL_LOWVOLTAGE_PRE               (2600)    	//����ǷѹԤ����ֵ2600mV
#define CELL_LOWVOLTAGE_PRE_FREE            (2650)    //����Ƿѹ���Ԥ����ֵ2650mV
//
#define CELL_OVERVOLTAGE_PRE               (3600)    	//�����ѹԤ����ֵ3600mV
#define CELL_OVERVOLTAGE_PRE_FREE           (3550)    //�����ѹ���Ԥ����ֵ 3550mV
//
#define CELL_DIFF_VOLTAGE_ERR        			(200)  //��оģ�鲻ƽ�����ѹ��ֵmV
#define CELL_DIFF_VOLTAGE_ERR_FREE   			(100)   //��оģ�鲻ƽ����Ͻ��ѹ��?mV

//Pack_Alarms[2]
#define CUTOFF_OVERTEMP                 	(500)      //�����жϹ�������ֵ0.1�� 500
#define CUTOFF_OVERPACKVOLTAGE            (2920)    //�ܹ�ѹ�жϵ�ѹ��ֵ 29.20 3.65 0.01V
#define CUTOFF_LOWPACKVOLTAGE            	(2000)    //��Ƿѹ�жϵ�ѹ��ֵ 20.00 2.5 0.01V
#define CUTOFF_OVERCELLVOLTAGE            (3650)    //�����ѹ�ж���ֵ3650mV
#define CUTOFF_LOWCELLVOLTAGE             (2500)    //����Ƿѹ�ж���ֵ2500mV
#define CUTOFF_Charge_Current_Over        (-7000)    //�����������ж�float mA
#define CUTOFF_Discharge_Current_Over      (6000)    //�ŵ���������ж�float mA  6A

/*
*********************************************************************************************************
*                                              SERVICES
*********************************************************************************************************
*/

void Fault_Diagnose_Discharge_COMM(void);
void Fault_Protect(void);
void Fault_Diagnose_Vol(void);
void Fault_Diagnose_Temp(void);
void Fault_Diagnose_Charge(void);

#endif
