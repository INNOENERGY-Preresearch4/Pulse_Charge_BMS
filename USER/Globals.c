/*
*********************************************************************************************************
*
*                                     				全局变量声明
*
*
* Filename      : Globals.c
* Version       : V1.00
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include  "Globals.h"
#include "includes.h"

/*
*********************************************************************************************************
*                                            GLOBAL VARIABLES
*********************************************************************************************************
*/


////////////////////////////////////////////////////////
// 程序时间及调度相关
////////////////////////////////////////////////////////
u32 	Run_Time=0;			//程序运行时间
u8 		Flag_500ms=0;		//500ms标志
u8 		Flag_50ms=0;		//100ms标志
u8 		Flag_100ms=0;		//200ms标志


////////////////////////////////////////////////////////
// 电池pack电压及温度测量相关
////////////////////////////////////////////////////////
uint32_t  PACK_Voltage_mV =0;    // PACK_Voltage为单体相加得出的总电压
uint16_t  pack_vol=0;         // pack_vol为采样总电压
uint8_t   Cell_Max_Num=0;         //单体电压最大节数
uint8_t   Cell_Min_Num=0;         //单体电压最小节数
uint16_t  Max_CellVol=0;          //单体最大电压
uint16_t  Min_CellVol=0;          //单体最小电压
s16  			Pack_MaxTemp=0;          //最大温度
s16  			Pack_MinTemp=0;          //最小温度
uint8_t   Temp_Min_Num=0,Temp_Max_Num=0;//最高及最低温度点数
u16 cellvol[TOTAL_IC*12];		//电池组单体采样电压
u16 LTC6804_GpioVol[TOTAL_IC*6];		//电池组温度采样电压
s16	Pack_TempArr[TEMP_NUM];							//电池组各点温度，精度0.1℃


/////////////////////////////////////////////////////////////////////////////////////////
// SOC测量计算相关变量
/////////////////////////////////////////////////////////////////////////////////////////
//滤波后的计算相关变量，只作为对比数据用
volatile uint16_t  SOC_Value_Filter=0;        				//当前SOC千分比
float  		Init_SOC_mAms_Filter = 0;        	//电池组初始容量,毫安.毫秒
volatile float  		Remain_SOC_mAms_Filter = 0;  			//电池组剩余容量,毫安.毫秒
//float   	Cur_Ref_V_Filter=Current_REF_V_Filter; 	//霍尔电流传感器基准电压值
volatile float   		Cur_Ref_V_Filter=0; 	//霍尔电流传感器基准电压值
volatile float   		Cur_Measure_V_Filter=0; 	//霍尔电流传感器测量值电压值
volatile float			SOC_Current_Filter=0;							//参与SOC计算的电流值，mA
volatile float			SOC_Current_Filter_Pre=0;							//上次参与SOC计算的电流值，mA
uint32_t	WriteSOC_Remian_Address_Filter = Address_SOC_Remain_Filter;//滤波后的SOC存储首地址

//没滤波的计算相关变量，参与保护值使用
volatile uint16_t  SOC_Value_Pulse=0;        				//当前SOC千分比
float  		Init_SOC_mAms_Pulse = 0;        	//电池组初始容量,毫安.毫秒
volatile float  		Remain_SOC_mAms_Pulse = 0;  			//电池组剩余容量,毫安.毫秒
//float   	Cur_Ref_V_Pulse=Current_REF_V_Pulse; 	//霍尔电流传感器基准电压值
volatile float   		Cur_Ref_V_Pulse=0; 	//霍尔电流传感器基准电压值
volatile float   		Cur_Measure_V_Pulse=0; 						//霍尔电流传感器测量值电压值
volatile float			SOC_Current_Pulse=0;							//参与SOC计算的电流值，mA
volatile float			SOC_Current_Pulse_Pre=0;							//上次参与SOC计算的电流值，mA
uint32_t	WriteSOC_Remian_Address_Pulse = Address_SOC_Remain_Pulse;//没滤波的SOC存储首地址

float 		SOC_TIM_Base=0;//SOC计算时间基准

u8 Full_Capacity_Flag = 0;//是否充满，1为充满，0为未充满
u8 Full_Capacity_Num = 0;//是否充满计算变量

/////////////////////////////////////////////////////////////////////////////////////////
// 充电状态
/////////////////////////////////////////////////////////////////////////////////////////
u8 	OverCharge=0;

/////////////////////////////////////////////////////////////////////////////////////////
// 均衡状态
/////////////////////////////////////////////////////////////////////////////////////////
uint8_t  need_balance = 0;         //是否需要均衡，1为需要均衡，0为不需要均衡

/////////////////////////////////////////////////////////////////////////////////////////
// 故障相关
/////////////////////////////////////////////////////////////////////////////////////////
uint8_t Host_COMM_error_flag=0;//与上位机通讯故障标识，5s内没收到上位机的信息
uint8_t Host_COMM_error_cont=0;//与上位机通讯故障计数
uint8_t HOST_COMM_ON=0;        //上位机是否在线标志，0为不在线，1为在线

uint8_t temp_error_cont=0; //温度采样错误次数
uint8_t vol_error_cont=0; //电压采样错误次数
uint8_t cur_ad_count=0;   //电流采样错误次数

uint8_t tempover_count=0;    //电池组放电过热报警计数变量
uint8_t templow_count=0;          //电池组过冷报警计数变量

uint8_t  overpackvoltage_count=0;  //电池组过压故障报警计数变量 
uint8_t  lowpackvoltage_count=0;   //电池组欠压故障报警计数变量

uint8_t  dcurrent_count=0;         //放电电流过大故障报警计数变量  
uint8_t  ccurrent_count=0;         //充电电流过大报警计数变量

uint8_t  overcellvoltage_count=0;      //单体过压故障报警计数变量
uint8_t  lowcellvoltage_count=0;       //单体欠压故障报警计数变量

uint8_t  cell_diff_blance=0;       //单体不平衡报警变量
uint8_t  cell_diff_blance_free=0;  //单体不平衡报警解除变量
uint8_t  unblance_count=0;         //单体不平衡故障报警计数变量 

uint8_t  cutoff_tempover_count=0;    //电池组放电过热报警计数变量
uint8_t  cutoff_overpackvoltage_count=0;  //电池组过压故障报警计数变量 
uint8_t  cutoff_lowpackvoltage_count=0;   //电池组欠压故障报警计数变量
uint8_t  cutoff_overcellvoltage_count=0;      //单体过压故障报警计数变量
uint8_t  cutoff_lowcellvoltage_count=0;       //单体欠压故障报警计数变量 
uint8_t  cutoff_ccurrent_count=0;         //充电电流过大报警计数变量
uint8_t  cutoff_dcurrent_count=0;         //放电电流过大故障报警计数变量 


/////////////////////////////////////////////////////////////////////////////////////////
// HMI指令相关
/////////////////////////////////////////////////////////////////////////////////////////
u8 		CMD_Battery_Mode=0;//0x01为充电，0x02为放电，其他为空
s32 	Battery_Mode_Time=0;//指令设置的时间属性，以s为单位

/////////////////////////////////////////////////////////////////////////////////////////
// 继电器相关
/////////////////////////////////////////////////////////////////////////////////////////
u8 		Relay_Flag=Flag_OFF;//0x01为充电，0x02为放电，其他为空

/*
*********************************************************************************************************
*                                           	STRUCTS
*********************************************************************************************************
*/
BMS_To_HMI_Typedef BMS_To_HMI;
HMI_To_BMS_Typedef HMI_To_BMS;
