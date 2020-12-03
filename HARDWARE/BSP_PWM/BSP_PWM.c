/*
*********************************************************************************************************
*
*                                     BOARD SUPPORT PACKAGE (BSP)
*
* Filename      : BSP_PWM.h
* Version       : V1.00
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include "BSP_PWM.h"

/*
*********************************************************************************************************
*                                          LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/ 

TIM_HandleTypeDef 	TIM9_Handler;         //定时器句柄 
TIM_OC_InitTypeDef 	TIM9_CH1Handler,TIM9_CH2Handler;     //定时器通道句柄
TIM_HandleTypeDef 	TIM4_Handler;         //定时器句柄 
TIM_OC_InitTypeDef 	TIM4_CH2Handler,TIM4_CH3Handler;     //定时器通道句柄

/*
*********************************************************************************************************
*                                              SERVICES
*********************************************************************************************************
*/

//当APB1分频系数不为1，通用定时器时钟是APB1的2倍，高级定时器及TIM9-11是APB2
//TIM9 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM9_PWM_Init(u16 arr,u16 psc)
{ 
    TIM9_Handler.Instance=TIM9;            //定时器9
    TIM9_Handler.Init.Prescaler=psc;       //定时器分频
    TIM9_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;//向上计数模式
    TIM9_Handler.Init.Period=arr;          //自动重装载值
    TIM9_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&TIM9_Handler);       //初始化PWM
    
    TIM9_CH1Handler.OCMode=TIM_OCMODE_PWM1; //模式选择PWM1
    TIM9_CH1Handler.Pulse=arr/2;            //设置比较值,此值用来确定占空比，默认比较值为自动重装载值的一半,即占空比为50%
    TIM9_CH1Handler.OCPolarity=TIM_OCPOLARITY_HIGH; //输出比较极性为低 
    HAL_TIM_PWM_ConfigChannel(&TIM9_Handler,&TIM9_CH1Handler,TIM_CHANNEL_1);//配置TIM9通道1
    HAL_TIM_PWM_Start(&TIM9_Handler,TIM_CHANNEL_1);//开启PWM通道4
	
	  TIM9_CH2Handler.OCMode=TIM_OCMODE_PWM1; //模式选择PWM1
    TIM9_CH2Handler.Pulse=arr/2;            //设置比较值,此值用来确定占空比，默认比较值为自动重装载值的一半,即占空比为50%
    TIM9_CH2Handler.OCPolarity=TIM_OCPOLARITY_HIGH; //输出比较极性为低 
    HAL_TIM_PWM_ConfigChannel(&TIM9_Handler,&TIM9_CH2Handler,TIM_CHANNEL_2);//配置TIM9通道2
    HAL_TIM_PWM_Start(&TIM9_Handler,TIM_CHANNEL_2);//开启PWM通道2
}


void TIM4_PWM_Init(u16 arr,u16 psc)
{ 
    TIM4_Handler.Instance=TIM4;            //定时器4
    TIM4_Handler.Init.Prescaler=psc;       //定时器分频
    TIM4_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;//向上计数模式
    TIM4_Handler.Init.Period=arr;          //自动重装载值
    TIM4_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&TIM4_Handler);       //初始化PWM
    
    TIM4_CH2Handler.OCMode=TIM_OCMODE_PWM1; //模式选择PWM1
    TIM4_CH2Handler.Pulse=arr/2;            //设置比较值,此值用来确定占空比，默认比较值为自动重装载值的一半,即占空比为50%
    TIM4_CH2Handler.OCPolarity=TIM_OCPOLARITY_HIGH; //输出比较极性为低 
    HAL_TIM_PWM_ConfigChannel(&TIM4_Handler,&TIM4_CH2Handler,TIM_CHANNEL_2);//配置TIM4通道2
    HAL_TIM_PWM_Start(&TIM4_Handler,TIM_CHANNEL_2);//开启PWM通道2
	
	  TIM4_CH3Handler.OCMode=TIM_OCMODE_PWM1; //模式选择PWM1
    TIM4_CH3Handler.Pulse=arr/2;            //设置比较值,此值用来确定占空比，默认比较值为自动重装载值的一半,即占空比为50%
    TIM4_CH3Handler.OCPolarity=TIM_OCPOLARITY_HIGH; //输出比较极性为低 
    HAL_TIM_PWM_ConfigChannel(&TIM4_Handler,&TIM4_CH3Handler,TIM_CHANNEL_3);//配置TIM4通道3
    HAL_TIM_PWM_Start(&TIM4_Handler,TIM_CHANNEL_3);//开启PWM通道3
}

//定时器底层驱动，时钟使能，引脚配置
//此函数会被HAL_TIM_PWM_Init()调用
//htim:定时器句柄
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
		GPIO_InitTypeDef GPIO_Initure;
	
		if(htim->Instance==TIM9)
		{
			__HAL_RCC_GPIOE_CLK_ENABLE();			//开启GPIOE时钟
			__HAL_RCC_TIM9_CLK_ENABLE();			//使能定时器9
		
			GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	//复用推完输出
			GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
			GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
		
			GPIO_Initure.Pin=GPIO_PIN_5|GPIO_PIN_6; //PE5
			GPIO_Initure.Alternate=GPIO_AF3_TIM9;	//PE5复用为TIM9_CH1
			HAL_GPIO_Init(GPIOE,&GPIO_Initure);
		}
		else if(htim->Instance==TIM4)
		{
			__HAL_RCC_GPIOB_CLK_ENABLE();			//开启GPIOB时钟
			__HAL_RCC_TIM4_CLK_ENABLE();			//使能定时器4
		
			GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	//复用推完输出
			GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
			GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
		
			GPIO_Initure.Pin=GPIO_PIN_7|GPIO_PIN_8;           	//PB7
			GPIO_Initure.Alternate=GPIO_AF2_TIM4;	//PB7复用为TIM4_CH2
			HAL_GPIO_Init(GPIOB,&GPIO_Initure);	
		}
		else
		{}
}


//设置TIM9通道1的占空比
//compare:比较值
void Set_TIM9Chanel1_Duty(u8 Duty)
{
	static u32 compare;
	Duty=range(Duty,0,100);//占空比0-100范围
	compare=TIM9_arr*Duty/100;
	TIM9->CCR1=compare; 
}

//设置TIM9通道2的占空比
//compare:比较值
void Set_TIM9Chanel2_Duty(u8 Duty)
{
	static u32 compare;
	Duty=range(Duty,0,100);//占空比0-100范围
	compare=TIM9_arr*Duty/100;
	TIM9->CCR2=compare;  
}

//设置TIM4通道2的占空比
//compare:比较值
void Set_TIM4Chanel2_Duty(u8 Duty)
{
	static u32 compare;
	Duty=range(Duty,0,100);//占空比0-100范围
	compare=TIM4_arr*Duty/100;
	TIM4->CCR2=compare; 
}

//设置TIM4通道3的占空比
//compare:比较值
void Set_TIM4Chanel3_Duty(u8 Duty)
{
	static u32 compare;
	Duty=range(Duty,0,100);//占空比0-100范围
	compare=TIM4_arr*Duty/100;
	TIM4->CCR3=compare; 
}
