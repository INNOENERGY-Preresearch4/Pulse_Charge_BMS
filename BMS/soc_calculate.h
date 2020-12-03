//soc_calculate.h
//SOC计算
#include "sys.h"
extern u32 soc0_ah;                  //电池组初始容量,毫安.秒
extern u32 soc_ah_remain;            //电池组剩余容量,毫安.秒
extern s16 soc_pre;                  //上一次SOC千分比
//电量计算方法选择
//存储初始电量和剩余电量通过电量计算SOC,使用安时积分法计算能量
//系统工作时首先要对电池充满电然后才能准确
#define SOC_CAL_METHOD1 (1)   
#define SOC0_CAPACITY   (120000) //修改原160000
#define SOC_SC_CAPACITY (120000) 
#define SOCBZ_CAPACITY  (96000) 
//安时积分补偿参数
#define SOC_CAL_METHOD SOC_CAL_METHOD1
#define SOC_REMAIN_WRITETIME   (30000)      //两分钟写一次剩余电量
#define SOC_REMAIN_MAX   (64)      //剩余电量存储区存储个数最大值
s16 OCV_Int(u16 cellmin,s8 temp);
void soc_calculate_init(u32 battery_capacity);
//读取电池组剩余容量,单位毫安.秒
u32 read_soc_remain(u32 address);
void write_soc_remain(u32 address,u32 soc_remain);
//写入初始电量,单位毫安.秒
s16 calculate_soc_ah(u32 soc0_ah,s32 current,u32 time);
//计算实际容量
void cal_socrl_ah(u8 flag,s32 current,u32 time);
//历史容量读取初始化
void soc_history_init(u32 battery_capacity);
//循环寿命剩余次数
u16 get_cycles_remain(void);
//获取剩余使用天数
u16 get_days_remain(void);
//读取剩余循环次数标志
u8 get_cycles_remain_flag(void);
//剩余使用次数
void memory_save_cycle_remain(u16 data,u8 cyc_flag);
//日历剩余寿命
void memory_save_days_remain(u16 data);   
void SOC_Handle(s32 cur,u32 passtime);
void SOC_real_write(s32 cur,u32 passtime);
//
extern u16 cellmaxvalue;           //单体过压故障检测电池电压最大值
extern u16 cellminvalue;           //单体过压故障检测电池电压最小值
extern u8 power_off;   //下电标识

