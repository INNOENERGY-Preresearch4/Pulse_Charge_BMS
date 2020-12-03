/*
*********************************************************************************************************
*
*                                     BOARD SUPPORT PACKAGE (BSP)
*                                                                    
*
* Filename      : BSP_TIMER.c
* Version       : V1.00
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include "BSP_TIMER.h"
#include "includes.h"
#include  "Config.h"

/*
*********************************************************************************************************
*                                          LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

TIM_HandleTypeDef TIM2_Handler,TIM3_Handler,TIM4_Handler;      //定时器句柄 

/*
*********************************************************************************************************
*                                                SERVICES
*********************************************************************************************************
*/
//通用定时器3中断初始化
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器3!(定时器3挂在APB1上，时钟为HCLK/2)
void TIM3_Init(u16 arr,u16 psc)
{  
    TIM3_Handler.Instance=TIM3;                          //通用定时器3
    TIM3_Handler.Init.Prescaler=psc;                     //分频
    TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    TIM3_Handler.Init.Period=arr;                        //自动装载值
    TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
    HAL_TIM_Base_Init(&TIM3_Handler);
    
    HAL_TIM_Base_Start_IT(&TIM3_Handler); //使能定时器3更新中断：TIM_IT_UPDATE    
}

void TIM4_Init(u16 arr,u16 psc)
{  
    TIM4_Handler.Instance=TIM4;                          //通用定时器4
    TIM4_Handler.Init.Prescaler=psc;                     //分频
    TIM4_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    TIM4_Handler.Init.Period=arr;                        //自动装载值
    TIM4_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
    HAL_TIM_Base_Init(&TIM4_Handler);
    
    HAL_TIM_Base_Start_IT(&TIM4_Handler); //使能定时器4更新中断：TIM_IT_UPDATE    
}

void TIM2_Init(u32 arr,u16 psc)
{  
    TIM2_Handler.Instance=TIM2;                          //通用定时器2
    TIM2_Handler.Init.Prescaler=psc;                     //分频
    TIM2_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    TIM2_Handler.Init.Period=arr;                        //自动装载值
    TIM2_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
    HAL_TIM_Base_Init(&TIM2_Handler);
    
    //HAL_TIM_Base_Start_IT(&TIM2_Handler); //使能定时器2更新中断：TIM_IT_UPDATE    
}


//定时器底册驱动，开启时钟，设置中断优先级
//此函数会被HAL_TIM_Base_Init()函数调用
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
		if(htim->Instance==TIM3)
		{
				__HAL_RCC_TIM3_CLK_ENABLE();            //使能TIM3时钟
				HAL_NVIC_SetPriority(TIM3_IRQn,0,0);    //设置中断优先级，抢占优先级0，子优先级0,这里timer设置最高优先级，其他中断均低于此优先级，作为SOC计算基准
				HAL_NVIC_EnableIRQ(TIM3_IRQn);          //开启ITM3中断   
		}
		else if(htim->Instance==TIM4)
		{
				__HAL_RCC_TIM4_CLK_ENABLE();            //使能TIM4时钟
				HAL_NVIC_SetPriority(TIM4_IRQn,1,1);    //设置中断优先级，抢占优先级1，子优先级0,其他中断均低于此优先级，作为整个程序调度时钟
				HAL_NVIC_EnableIRQ(TIM4_IRQn);          //开启ITM4中断  	
		}	
		else if(htim->Instance==TIM2)
		{
				__HAL_RCC_TIM2_CLK_ENABLE();            //使能TIM2时钟
				HAL_NVIC_SetPriority(TIM2_IRQn,2,1);    //设置中断优先级，抢占优先级1，子优先级0,其他中断均低于此优先级，作为充放电计时，减轻TIM4中断中的工作量
				HAL_NVIC_EnableIRQ(TIM2_IRQn);          //开启ITM2中断  	
		}
		else
		{}
}


//定时器3中断服务函数，针对非滤波脉冲采样
void TIM3_IRQHandler(void)
{		
//		CH1_High;
		SCB_CleanInvalidateDCache_by_Addr((u32*)ADC3_ConvertValue,ADC3_CONVER_Time*4);
		ADC3_ConvertValueRead[0] = ADC3_ConvertValue[0];
		Cur_Measure_V_Pulse = ((float)(ADC3_ConvertValueRead[0]))/4096.0f*BMS_AVCC_Ref*Pulse_Facotr;
		Remain_SOC_mAms_Pulse_Update();
		HAL_TIM_IRQHandler(&TIM3_Handler);
//		CH1_Low;
}


//定时器4中断服务函数，时钟+针对滤波脉冲采样
void TIM4_IRQHandler(void)
{
		CH1_High;
		Run_Time++;
		Filter_ADC1_Start();
		Remain_SOC_mAms_Fliter_Update();
		Fault_Diagnose_Charge();
	
		if(Run_Time%5==0)//50ms事件
		{
				Flag_50ms=1;		
		}
		if((Run_Time+35)%10==0)//100ms事件,与50ms事件错开15ms，增加CPU利用率
		{
				Flag_100ms=1;
		}	
		if((Run_Time+20)%50==0)//500ms事件,与50ms事件错开30ms，增加CPU利用率
		{
				Flag_500ms=1;
		}
		HAL_TIM_IRQHandler(&TIM4_Handler);
		CH1_Low;
}


//定时器2中断服务函数
void TIM2_IRQHandler(void)
{		
		Battery_Mode_Time-=10;
	
		if(Battery_Mode_Time <= 0)
		{
//				if((CMD_Battery_Mode == CMD_Charge)||(CMD_Battery_Mode == CMD_Discharge))
//				{
						Relay_OFF;
						Relay_OFF;
						CMD_Battery_Mode=0;	
						HAL_TIM_Base_Stop_IT(&TIM2_Handler);
						TIM2->CNT = 0;					
//				}
		}
		
    HAL_TIM_IRQHandler(&TIM2_Handler);
}


//定时器中断服务函数调用
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

}

