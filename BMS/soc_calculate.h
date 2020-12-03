//soc_calculate.h
//SOC����
#include "sys.h"
extern u32 soc0_ah;                  //������ʼ����,����.��
extern u32 soc_ah_remain;            //�����ʣ������,����.��
extern s16 soc_pre;                  //��һ��SOCǧ�ֱ�
//�������㷽��ѡ��
//�洢��ʼ������ʣ�����ͨ����������SOC,ʹ�ð�ʱ���ַ���������
//ϵͳ����ʱ����Ҫ�Ե�س�����Ȼ�����׼ȷ
#define SOC_CAL_METHOD1 (1)   
#define SOC0_CAPACITY   (120000) //�޸�ԭ160000
#define SOC_SC_CAPACITY (120000) 
#define SOCBZ_CAPACITY  (96000) 
//��ʱ���ֲ�������
#define SOC_CAL_METHOD SOC_CAL_METHOD1
#define SOC_REMAIN_WRITETIME   (30000)      //������дһ��ʣ�����
#define SOC_REMAIN_MAX   (64)      //ʣ������洢���洢�������ֵ
s16 OCV_Int(u16 cellmin,s8 temp);
void soc_calculate_init(u32 battery_capacity);
//��ȡ�����ʣ������,��λ����.��
u32 read_soc_remain(u32 address);
void write_soc_remain(u32 address,u32 soc_remain);
//д���ʼ����,��λ����.��
s16 calculate_soc_ah(u32 soc0_ah,s32 current,u32 time);
//����ʵ������
void cal_socrl_ah(u8 flag,s32 current,u32 time);
//��ʷ������ȡ��ʼ��
void soc_history_init(u32 battery_capacity);
//ѭ������ʣ�����
u16 get_cycles_remain(void);
//��ȡʣ��ʹ������
u16 get_days_remain(void);
//��ȡʣ��ѭ��������־
u8 get_cycles_remain_flag(void);
//ʣ��ʹ�ô���
void memory_save_cycle_remain(u16 data,u8 cyc_flag);
//����ʣ������
void memory_save_days_remain(u16 data);   
void SOC_Handle(s32 cur,u32 passtime);
void SOC_real_write(s32 cur,u32 passtime);
//
extern u16 cellmaxvalue;           //�����ѹ���ϼ���ص�ѹ���ֵ
extern u16 cellminvalue;           //�����ѹ���ϼ���ص�ѹ��Сֵ
extern u8 power_off;   //�µ��ʶ

