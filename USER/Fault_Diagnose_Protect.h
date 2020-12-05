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
//BIT检测
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//故障名称类型
//Pack_Alarms[0]
#define ALARM_EEPROM_BIT               (0x01)    //EE故障  *
#define ALARM_VOLTAGE_AD_BIT           (0x02)    //电压采集电路故障  *
#define ALARM_TEMP_AD_BIT              (0x04)    //温度采集电路故障  *
#define ALARM_CURRENT_AD_BIT           (0x08)    //电流采集电路故障  *
#define ALARM_OVERTEMP_PRE_BIT         (0x10)    //电池过温预警
#define ALARM_LOWTEMP_PRE_BIT          (0x20)    //电池低温故障预警
#define ALARM_PACKVOLTAGE_OVER_PRE_BIT (0x40)    //电池组过压预警故障
#define ALARM_PACKVOLTAGE_LOW_PRE_BIT  (0x80)    //电池组欠压预警故障
//Pack_Alarms[1]
#define ALARM_COVERCURRENT_PRE_BIT     (0x01)    //充电电流过大预警故障  *
#define ALARM_DCOVERCURRENT_PRE_BIT    (0x02)    //放电电流过大预警故障  *
#define ALARM_CELLVOLTAGE_LOW_PRE_BIT  (0x04)    //单体欠压预警故障
#define ALARM_CELLVOLTAGE_OVER_PRE_BIT (0x08)    //单体电池过压预警故障
#define ALARM_UNBLANCE_BIT       		(0x10)    //单体不平衡故障
//Pack_Alarms[2]
#define ALARM_CUTOFF_OVERTEMP_BIT      	 (0x01)    //过温切断故障
#define ALARM_CUTOFF_OVERPACKVOLTAGE_BIT (0x02)    //总过压切断故障
#define ALARM_CUTOFF_LOWPACKVOLTAGE_BIT  (0x04)    //总欠压切断故障
#define ALARM_CUTOFF_OVERCELLVOLTAGE_BIT (0x08)    //单体过压切断故障
#define ALARM_CUTOFF_LOWCELLVOLTAGE_BIT  (0x10)    //单体欠压切断故障
#define ALARM_CUTOFF_COVERCURRENT_BIT 	 (0x20)    //充电过流切断故障
#define ALARM_CUTOFF_DCOVERCURRENT_BIT   (0x40)    //放电过流切断故障



//保护值
//Pack_Alarms[0]
#define VOLTAGE_AD_ERR              	 (400)    	//电压采集故障阈值mV
#define VOLTAGE_AD_ERR_FREE          	 (1500)    //电压采集故障解除阈值mV

#define TEMP_AD_ERR              	 	 (250)    //温度采集故障阈值0.1℃
#define TEMP_AD_ERR_FREE         	 	 (150)   //温度采集故障解除阈值0.1℃

#define TEMP_HIGH_PRE                	 (450)      //放电过热故障预警门限值0.1℃  450
#define TEMP_HIGH_PRE_FREE           	 (400)      //放电过热故障预警解除门限值0.1℃  400

#define TEMP_LOW_PRE                    (50)     //放电低温故障预警门限值0.1℃   50
#define TEMP_LOW_PRE_FREE               (100)     //放电低温故障预警解除门限值0.1℃  100

#define OVERPACKVOLTAGE_PRE           	(2880)  //电池组过压预警限值  0.01V  单体3600mV
#define OVERPACKVOLTAGE_PRE_FREE      	(2840)  //电池组过压预警解除限值		0.01V	单体3550mV

#define LOWPACKVOLTAGE_PRE            	(2080)    //电池组欠压预警限值20.80V  0.01V  单体2600mV
#define LOWPACKVOLTAGE_PRE_FREE       	(2120)    //电池组欠压预警解除限值21.20V  0.01V	单体2650mV

//Pack_Alarms[1]
#define Charge_Current_Over_PRE            (-6500)     //充电电流过大预警门限值float mA
#define Charge_Current_Over_PRE_Free       (-6000)     //充电电流过大预警解除门限值float mA

#define Discharge_Current_Over_PRE          (5500)     //放电电流过大门预警限值float mA  5.5A
#define Discharge_Current_Over_PRE_Free     (5000)     //放电电流过大解除预警门限值float mA  5A
//
#define CELL_LOWVOLTAGE_PRE               (2600)    	//单体欠压预警限值2600mV
#define CELL_LOWVOLTAGE_PRE_FREE            (2650)    //单体欠压解除预警限值2650mV
//
#define CELL_OVERVOLTAGE_PRE               (3600)    	//单体过压预警限值3600mV
#define CELL_OVERVOLTAGE_PRE_FREE           (3550)    //单体过压解除预警限值 3550mV
//
#define CELL_DIFF_VOLTAGE_ERR        			(200)  //电芯模块不平衡故障压差值mV
#define CELL_DIFF_VOLTAGE_ERR_FREE   			(100)   //电芯模块不平衡故障解除压差?mV

//Pack_Alarms[2]
#define CUTOFF_OVERTEMP                 	(500)      //过热切断故障门限值0.1℃ 500
#define CUTOFF_OVERPACKVOLTAGE            (2920)    //总过压切断电压限值 29.20 3.65 0.01V
#define CUTOFF_LOWPACKVOLTAGE            	(2000)    //总欠压切断电压限值 20.00 2.5 0.01V
#define CUTOFF_OVERCELLVOLTAGE            (3650)    //单体过压切断限值3650mV
#define CUTOFF_LOWCELLVOLTAGE             (2500)    //单体欠压切断限值2500mV
#define CUTOFF_Charge_Current_Over        (-7000)    //充电电流过大切断float mA
#define CUTOFF_Discharge_Current_Over      (6000)    //放电电流过大切断float mA  6A

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
