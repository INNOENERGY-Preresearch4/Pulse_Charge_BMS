
#ifndef _SIG_COLLECT_H_
#define _SIG_COLLECT_H_
#include "sys.h"

#define FILTE_NUM  (20)

#define INVALID_VOLTAGE      (0xffff)     //电压无效值
#define INVALID_CURRENT      (0x7ffff)     //无效电流值


#define CHANNEL_AD_CURRENT        (8)      //电芯组总电流
#define CHANNEL_AD_CURRENTB       (9)      //电芯组均衡电流       h
#define CHANNEL_AD_TEMP1          (0)
#define CHANNEL_AD_TEMP2          (1)
#define CHANNEL_AD_TEMP3          (2)
#define CHANNEL_AD_TEMP4          (3)
#define CHANNEL_AD_TEMP5          (4)
#define CHANNEL_AD_TEMP6          (5)
#define CHANNEL_AD_TEMP7          (6)
#define CHANNEL_AD_TEMP8          (7)

#define VOLTAGE_REF              (3000)   //采集电压基准mV  4096
#define VOLTAGE_BREF_25          (2495)  //mV,2.5V参考电压实际值，记得更改电流采集处的值 2500
////////////////////////////////////////////////////////////////////////////////////////////////////////
// 采集参数初始化
//////////////////////////////////////////////////////////////////////////////////////////////////////// 
#define VOLTAGE_REF_25           (2490)  //mV,2.5V参考电压实际值，记得更改电流采集处的值 2500
#define VOLTAGE_REF_2            (4096)   //采集电压基准mV  4096    gai
#define PARAM_V_BAT              (2121)  //总电压放大倍数
#define  MAX_TEMP      (125)
#define  MIN_TEMP      (-55)
#define  R22           (2200)

//总电流采集
s32 get_current(u16 AD_value);

//总电流采集矫正
s16 get_current_ref(u16 AD_value);
//总电压采集
u16 get_voltage_bat(u16 vol);

 //从数组中求电压最小值以及编号
u16 CellMinValue(u16 data[],u16 n,u8*num);
//从数组中求电压最大值
u16 CellMaxValue(u16 data[],u16 n,u8*num);
//计算绝对差值
u8 Getabsvalue(s8 value1,s8 value2);
//从数组中求温度最小值以及编号
s8 TempMinValue(s8 data[],u8 n,u8*num);
//从数组中求温度最大值
s8 TempMaxValue(s8 data[],u8 n,u8*num);
//温度读取
s8 get_AD_temp(u8 channel);
//根据温度传感器电阻计算温度
s8 calculate_wendu(u16 resistor);
//读取精密电压电流数据
void hprec_read(s32 *cur,u16 *vol);

#endif

