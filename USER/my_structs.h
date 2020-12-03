
#ifndef _MY_STRUSTS_H_
#define _MY_STRUSTS_H_
#include "sys.h"

//ʱ��ṹ��
typedef struct
{
    uint8_t  year;   //��,0-99
    uint8_t  month;  //��,1-12
    uint8_t  day;    //��,1-31
    uint8_t  hour;   //Сʱ,0-23
    uint8_t  minute; //����,0-59
    uint8_t  second; //��,0-59
}DATE_TIME;
//**********************************************//
//BIT�����Ϣ
//**********************************************//
struct RSP_BIT_DATA
{

	u16 CEL_max_value;//��������ѹ  LSB=1MV
    u8 CEL_max_num;//��������ѹ���
    u16 CEL_min_value;//������С��ѹ
    u8 CEL_min_num;//������С��ѹ���
     //
    s8 TEMP_max_value;//��������¶�  LSB=1��
    u8 TEMP_max_num;//��������¶ȱ��
    s8 TEMP_min_value;//������С�¶�
    u8 TEMP_min_num;//������С�¶ȱ��
	u8  balance_num;//�����ر�� 
	u8  pack_alarms[3]; //��������״̬
	u32 PRO_num;//��ƷӲ����� 
	u16 pack_vol;   //������ܵ�ѹ LSB=1MV
	s16  current;           //��������λ1mA����Χ-32767mA-32768A�u
	u16 soc_value;         //ʣ����������λ0.1%��0-1000
	u8 rly_status;     //�Ӵ�������״̬
	u8 IO_status;      //IO���״̬
	//
	u16 cellVoltage[8];  //�����ѹ  LSB=1MV
    s8 cellTemp[8];       //�����¶�  LSB=1��
    u16 batteryStatus;  //�����״̬��Ϣ   
	DATE_TIME Time;     //ʱ���ʶ
};

#endif

