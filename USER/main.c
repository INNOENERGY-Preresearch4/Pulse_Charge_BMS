/*
*********************************************************************************************************
*
*                                     				 APPLICATION
*
* Filename      : main.c
* Version       : V1.00
*********************************************************************************************************
*/

#include "includes.h"

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                              SERVICES
*********************************************************************************************************
*/

int main(void)
{	
		//系统库及外设初始化
		Cache_Enable();                 //打开L1-Cache
		HAL_Init();				        			//初始化HAL库
		Stm32_Clock_Init(432,25,2,9);   //设置时钟,SYSCLK 216MHz,PLL 216MHz,AHB 216MHz,APB1 54MHz,APB2 108MHz
		GPIO_Init();
		UART8_Init(115200);
		ADC1_Init();
		ADC3_Init();
		TIM2_Init(TIM2_arr-1,TIM2_psc-1);	//定时器中断周期10s,用于充放电计时，只是设置，不开始运行，收到充放电指令采开始运行
		SPI2_Init();//LTC6804 SPI初始化
		Init_EEPROM_SPI();//EEPROM SPI初始化
		
		//参数初始化	
		delay_ms(1000);
		LTC6804_Init(&g_LTC6804Handle,NULL);
		SOC_Calculate_Init();	//对初始容量Init_SOC_mAms_Filter赋值，查询是否有Remain_SOC_mAms_Filter数据，没有就将Remain_SOC_mAms_Filter=Init_SOC_mAms_Filter
//		//test
//		Init_SOC_mAms_Pulse = ((float)BATTERY_CAPACITY)*Hours_Split;
//		Remain_SOC_mAms_Pulse = 0;
//		Init_SOC_mAms_Filter = ((float)BATTERY_CAPACITY)*Hours_Split;
//		//Remain_SOC_mAms_Filter = Init_SOC_mAms_Filter/10;
//		Remain_SOC_mAms_Filter = 0;
		//
		HMI_BMS_Com_Init();//BMS与HMI通讯参数初始化
		Cur_Ref_Update();//电流采样基准更新
		InitRingbuffer_UART8Rx();//接收环形缓存区注册
		delay_ms(100);
		
		//系统调度时钟及非滤波电流采样时钟开始运行
		TIM4_Init(TIM4_arr-1,TIM4_psc-1);				//定时器中断周期10ms,用于程序调度
		TIM3_Init(TIM3_arr-1,TIM3_psc-1);				//定时器中断周期始终是脉冲频率的20倍

		while(1)
		{		
		 //*****************************************************//
		 //500ms周期任务，包含LED，SOC存储
		 //*****************************************************// 
				if(Flag_500ms==1)
				{
					 LEDRed_Toggle;
					 Temp_Collect_ByCycle();			//电池温度采样
					 Fault_Diagnose_Temp();		//电池温度相关故障诊断
					 SOC_Filter_Memory_Upate();	//滤波电流计算的SOC  更新存储
					 SOC_Pulse_Memory_Upate();		//非滤波电流计算的SOC  更新存储
					 Flag_500ms=0;
				}				
		 //*****************************************************//
		 //50ms周期任务，包含电压温度采集处理，SOC千分比计算更新
		 //*****************************************************//
				if(Flag_50ms==1)
				{					
					 Vol_Collect_ByCycle();	//单体电压、总电压采样
					 Fault_Diagnose_Vol();	//电池电压相关故障诊断
					 Full_Capacity_Judgment();	//判断电池是否满电,有问题，需思考？充满电以后，若没放电，静止，电压低了，要不要取消充满状态，是否需要增加放电判断
					 SOC_PerK_Update();					//SOC千分比更新
					 Fault_Protect();//故障保护
					 Flag_50ms=0;					
				}
		 //*****************************************************//
		 //100ms周期任务，包含电池状态判断，故障检测与保护，均衡控制
		 //*****************************************************//
				if(Flag_100ms==1)
				{		
					 Pack_Cell_Balance();//均衡判断
					 Balance_Ctrl();//均衡控制
					 Battery_Status_Judgment();//电池状态判断
					 Fault_Diagnose_Discharge_COMM();//放电及通讯相关故障检测
					 Flag_100ms=0;
				}

			//*****************************************************//
			//通讯模块，解析HMI向BMS发送的数据，并作出响应	
			//*****************************************************//
			  HMI_To_BMS_Analyze();				
		}  								  	       
}

