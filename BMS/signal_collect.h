
#ifndef _SIG_COLLECT_H_
#define _SIG_COLLECT_H_
#include "sys.h"

#define FILTE_NUM  (20)

#define INVALID_VOLTAGE      (0xffff)     //��ѹ��Чֵ
#define INVALID_CURRENT      (0x7ffff)     //��Ч����ֵ


#define CHANNEL_AD_CURRENT        (8)      //��о���ܵ���
#define CHANNEL_AD_CURRENTB       (9)      //��о��������       h
#define CHANNEL_AD_TEMP1          (0)
#define CHANNEL_AD_TEMP2          (1)
#define CHANNEL_AD_TEMP3          (2)
#define CHANNEL_AD_TEMP4          (3)
#define CHANNEL_AD_TEMP5          (4)
#define CHANNEL_AD_TEMP6          (5)
#define CHANNEL_AD_TEMP7          (6)
#define CHANNEL_AD_TEMP8          (7)

#define VOLTAGE_REF              (3000)   //�ɼ���ѹ��׼mV  4096
#define VOLTAGE_BREF_25          (2495)  //mV,2.5V�ο���ѹʵ��ֵ���ǵø��ĵ����ɼ�����ֵ 2500
////////////////////////////////////////////////////////////////////////////////////////////////////////
// �ɼ�������ʼ��
//////////////////////////////////////////////////////////////////////////////////////////////////////// 
#define VOLTAGE_REF_25           (2490)  //mV,2.5V�ο���ѹʵ��ֵ���ǵø��ĵ����ɼ�����ֵ 2500
#define VOLTAGE_REF_2            (4096)   //�ɼ���ѹ��׼mV  4096    gai
#define PARAM_V_BAT              (2121)  //�ܵ�ѹ�Ŵ���
#define  MAX_TEMP      (125)
#define  MIN_TEMP      (-55)
#define  R22           (2200)

//�ܵ����ɼ�
s32 get_current(u16 AD_value);

//�ܵ����ɼ�����
s16 get_current_ref(u16 AD_value);
//�ܵ�ѹ�ɼ�
u16 get_voltage_bat(u16 vol);

 //�����������ѹ��Сֵ�Լ����
u16 CellMinValue(u16 data[],u16 n,u8*num);
//�����������ѹ���ֵ
u16 CellMaxValue(u16 data[],u16 n,u8*num);
//������Բ�ֵ
u8 Getabsvalue(s8 value1,s8 value2);
//�����������¶���Сֵ�Լ����
s8 TempMinValue(s8 data[],u8 n,u8*num);
//�����������¶����ֵ
s8 TempMaxValue(s8 data[],u8 n,u8*num);
//�¶ȶ�ȡ
s8 get_AD_temp(u8 channel);
//�����¶ȴ�������������¶�
s8 calculate_wendu(u16 resistor);
//��ȡ���ܵ�ѹ��������
void hprec_read(s32 *cur,u16 *vol);

#endif

