
#ifndef _MY_STRUSTS_H_
#define _MY_STRUSTS_H_
#include "sys.h"

//时间结构体
typedef struct
{
    uint8_t  year;   //年,0-99
    uint8_t  month;  //月,1-12
    uint8_t  day;    //日,1-31
    uint8_t  hour;   //小时,0-23
    uint8_t  minute; //分钟,0-59
    uint8_t  second; //秒,0-59
}DATE_TIME;
//**********************************************//
//BIT检测信息
//**********************************************//
struct RSP_BIT_DATA
{

	u16 CEL_max_value;//单体最大电压  LSB=1MV
    u8 CEL_max_num;//单体最大电压编号
    u16 CEL_min_value;//单体最小电压
    u8 CEL_min_num;//单体最小电压编号
     //
    s8 TEMP_max_value;//单体最大温度  LSB=1℃
    u8 TEMP_max_num;//单体最大温度编号
    s8 TEMP_min_value;//单体最小温度
    u8 TEMP_min_num;//单体最小温度编号
	u8  balance_num;//均衡电池编号 
	u8  pack_alarms[3]; //电池组故障状态
	u32 PRO_num;//产品硬件编号 
	u16 pack_vol;   //电池组总电压 LSB=1MV
	s16  current;           //电流，单位1mA，范围-32767mA-32768A
	u16 soc_value;         //剩余容量，单位0.1%，0-1000
	u8 rly_status;     //接触器控制状态
	u8 IO_status;      //IO检测状态
	//
	u16 cellVoltage[8];  //单体电压  LSB=1MV
    s8 cellTemp[8];       //单体温度  LSB=1℃
    u16 batteryStatus;  //电池组状态信息   
	DATE_TIME Time;     //时间标识
};

#endif

