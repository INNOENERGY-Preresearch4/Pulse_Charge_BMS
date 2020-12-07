/*
*********************************************************************************************************
*
*                                     				全局变量声明
*
* Filename      : _Globals.h
* Version       : V1.00
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                                 MODULE
*********************************************************************************************************
*/

#ifndef _Globals_H_
#define _Globals_H_

/*
*********************************************************************************************************
*                                              INCLUDE FILES
*********************************************************************************************************
*/

#include "sys.h"
#include  "Config.h"

/*
*********************************************************************************************************
*                                             Para DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                                 EXTERNS
*********************************************************************************************************
*/

////////////////////////////////////////////////////////
// 程序时间及调度相关
////////////////////////////////////////////////////////
extern 	u32 Run_Time;			//程序运行时间
extern 	u8 	Flag_500ms;		//500ms标志
extern 	u8 	Flag_50ms;		//100ms标志
extern 	u8 	Flag_100ms;		//200ms标志


////////////////////////////////////////////////////////
// 电池pack电压及温度测量相关
////////////////////////////////////////////////////////
extern uint32_t  PACK_Voltage_mV;    // PACK_Voltage为单体相加得出的总电压
extern uint16_t  pack_vol;         // pack_vol为采样总电压
extern uint8_t   Cell_Max_Num;         //单体电压最大节数
extern uint8_t   Cell_Min_Num;         //单体电压最小节数
extern uint16_t  Max_CellVol;          //单体最大电压
extern uint16_t  Min_CellVol;          //单体最小电压
extern s16  		 Pack_MaxTemp;          //最大温度
extern s16  		 Pack_MinTemp;          //最小温度
extern uint8_t   Temp_Min_Num,Temp_Max_Num;//最高及最低温度点数
extern u16 cellvol[TOTAL_IC*12];		//电池组单体采样电压
extern u16 LTC6804_GpioVol[TOTAL_IC*6];			//电池组温度采样电压
extern s16	Pack_TempArr[TEMP_NUM];								//电池组温度

/////////////////////////////////////////////////////////////////////////////////////////
// SOC测量计算相关变量
/////////////////////////////////////////////////////////////////////////////////////////
//滤波后的计算相关变量，只作为对比数据用
volatile extern uint16_t SOC_Value_Filter;                //当前SOC千分比
extern float Init_SOC_mAms_Filter;                  //电池组初始容量,毫安.秒
volatile extern float Remain_SOC_mAms_Filter;      //电池组剩余容量,毫安.秒
volatile extern float  Cur_Ref_V_Filter;           //霍尔电流校准值
volatile extern float  Cur_Measure_V_Filter;
volatile extern float 	SOC_Current_Filter;    			// 参与SOC计算的电流值
volatile extern float 	SOC_Current_Filter_Pre;    			// 参与SOC计算的电流值
extern uint32_t	WriteSOC_Remian_Address_Filter ;
#define   Cur_Filter   2

//没滤波的计算相关变量，参与保护值使用
volatile extern uint16_t  SOC_Value_Pulse;        				//当前SOC千分比
extern float  Init_SOC_mAms_Pulse;        	//电池组初始容量,毫安.秒
volatile extern float  Remain_SOC_mAms_Pulse;  			//电池组剩余容量,毫安.秒
volatile extern float   Cur_Ref_V_Pulse; 						//霍尔电流基准值
volatile extern float   Cur_Measure_V_Pulse;
volatile extern float	 SOC_Current_Pulse;							//霍参与SOC计算的电流值，mA
volatile extern float 	 SOC_Current_Pulse_Pre;    			// 参与SOC计算的电流值
extern uint32_t	 WriteSOC_Remian_Address_Pulse;//没滤波的SOC存储首地址
#define   Cur_Pulse   1

extern float SOC_TIM_Base;//SOC计算时间基准
extern u8 Full_Capacity_Flag;//是否充满，1为充满，0为未充满
extern u8 Full_Capacity_Num;//是否充满计算变量

/////////////////////////////////////////////////////////////////////////////////////////
// 充电状态
/////////////////////////////////////////////////////////////////////////////////////////

extern  u8 	OverCharge;

/////////////////////////////////////////////////////////////////////////////////////////
// 均衡状态
/////////////////////////////////////////////////////////////////////////////////////////
extern uint8_t     need_balance;         //是否需要均衡
extern uint8_t  	 blance_stop_cellcharge;

/////////////////////////////////////////////////////////////////////////////////////////
// 故障标识
/////////////////////////////////////////////////////////////////////////////////////////
extern uint8_t Host_COMM_error_flag;//与上位机通讯故障标识，5s内没收到上位机的信息
extern uint8_t Host_COMM_error_cont;//通讯故障计数
extern uint8_t HOST_COMM_ON;

extern uint8_t temp_error_cont; //温度采样错误次数
extern uint8_t vol_error_cont; //电压采样错误次数
extern uint8_t cur_ad_count;   //电流采样错误次数

extern uint8_t tempover_count;    //电池组放电过热报警计数变量
extern uint8_t  templow_count;          //电池组过冷报警计数变量

extern uint8_t  overpackvoltage_count;  //电池组过压故障报警计数变量 
extern uint8_t  lowpackvoltage_count;   //电池组欠压故障报警计数变量

extern uint8_t  dcurrent_count;         //放电电流过大故障报警计数变量  
extern uint8_t  ccurrent_count;         //充电电流过大报警计数变量

extern uint8_t  overcellvoltage_count;      //单体过压故障报警计数变量
extern uint8_t  lowcellvoltage_count;       //单体欠压故障报警计数变量

extern uint8_t  cell_diff_blance;       //单体不平衡报警变量
extern uint8_t  cell_diff_blance_free;  //单体不平衡报警解除变量
extern uint8_t  unblance_count;         //单体不平衡故障报警计数变量

extern uint8_t  cutoff_tempover_count;    //电池组放电过热报警计数变量
extern uint8_t  cutoff_overpackvoltage_count;  //电池组过压故障报警计数变量 
extern uint8_t  cutoff_lowpackvoltage_count;   //电池组欠压故障报警计数变量
extern uint8_t  cutoff_overcellvoltage_count;      //单体过压故障报警计数变量
extern uint8_t  cutoff_lowcellvoltage_count;       //单体欠压故障报警计数变量
extern uint8_t  cutoff_ccurrent_count;         //充电电流过大报警计数变量
extern uint8_t  cutoff_dcurrent_count;         //放电电流过大故障报警计数变量 


/////////////////////////////////////////////////////////////////////////////////////////
// HMI指令相关
/////////////////////////////////////////////////////////////////////////////////////////
extern 	 u8 	CMD_Battery_Mode;//0x01为充电，0x02为放电，其他为空
extern 	 s32 	Battery_Mode_Time;//指令设置的时间属性，以s为单位
#define  CMD_Charge      (0x01)      //充电
#define  CMD_Discharge   (0x02)      //放电

/////////////////////////////////////////////////////////////////////////////////////////
// 继电器相关
/////////////////////////////////////////////////////////////////////////////////////////
extern 		u8 Relay_Flag;//
#define  	Flag_ON      (0x01)      //继电器吸合
#define  	Flag_OFF     (0x00)      //继电器断开

/*
*********************************************************************************************************
*                                           	STRUCTS
*********************************************************************************************************
*/

#define BMS_HMI_SyncHeader1 		0xEB
#define BMS_HMI_SyncHeader2 		0x90
#define BMS_To_HMI_Address			0x01
#define BMS_To_HMI_Message_ID		0x10
#define HMI_To_BMS_Address			0x02
#define HMI_To_BMS_Message_ID_CMD			0x20
#define HMI_To_BMS_Message_ID_NULL		0x30
#define HMI_To_BMS_CMD_Charge					0x01
#define HMI_To_BMS_CMD_Discharge			0x02
#define BMS_To_HMI_Num			50
#define HMI_To_BMS_Num			16

//与HMI屏幕通讯
#pragma  pack(push,1)   // 61
typedef struct {
	u8    SyncHeader1;  //帧同步字1  0xEB
	u8    SyncHeader2;  //帧同步字2  0x90
	u8    Address;			//地址  0x01
	u8    Message_ID; 	//帧识别码  0x10
	
	u8    Battery_Status; //电池组工作状态
	u8    Pack_Alarms[3];	//电池组故障信息
	u16   Total_Voltage; 	//电池组总电压
	s16 	Total_Current;  //电池组总电流
	
	u16 	Pack_SOC; 					//电池组剩余容量
	u16 	Cell_Voltage[12]; 	//电池单体电压
	u16 	CycledCount_Remain; //电池剩余使用次数
	u16 	Days_Remain; 				//电池剩余日历寿命
	s16 	Pack_Temp; 			//电池组温度
	
	u8 	  Rev[4]; //预留
	u8    Message_Num; //帧计数
	u8	  SumCheck;
} BMS_To_HMI_Typedef;
#pragma pack(pop)

//与HMI屏幕通讯
#pragma  pack(push,1)   // 61
typedef struct {
	u8    SyncHeader1;  //帧同步字1  0xEB
	u8    SyncHeader2;  //帧同步字2  0x90
	u8    Address;			//地址  0x02
	u8    Message_ID; 	//帧识别码  0x20=有指令，0x30=空指令
	
	u8    Cmd_Mode; 		//0x01=充电，0x02=放电，其他=无动作
	u8    Cmd_ParamL;		//LSB=1min  范围0min~1440min
	u8    Cmd_ParamH; 	//LSB=1min  范围0min~1440min
	u8 	  Cmd_STOP; 			//位0：1为急停  0为无操作
	
	u8 	  Rev[6]; //预留
	u8    Message_Num; //帧计数
	u8	  SumCheck;
} HMI_To_BMS_Typedef;
#pragma pack(pop)

/*
*********************************************************************************************************
*                                           	EXTERN  STRUCTS
*********************************************************************************************************
*/

extern 	BMS_To_HMI_Typedef BMS_To_HMI;
extern 	HMI_To_BMS_Typedef HMI_To_BMS;


#endif


