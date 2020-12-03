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

TIM_HandleTypeDef 	TIM9_Handler;         //��ʱ����� 
TIM_OC_InitTypeDef 	TIM9_CH1Handler,TIM9_CH2Handler;     //��ʱ��ͨ�����
TIM_HandleTypeDef 	TIM4_Handler;         //��ʱ����� 
TIM_OC_InitTypeDef 	TIM4_CH2Handler,TIM4_CH3Handler;     //��ʱ��ͨ�����

/*
*********************************************************************************************************
*                                              SERVICES
*********************************************************************************************************
*/

//��APB1��Ƶϵ����Ϊ1��ͨ�ö�ʱ��ʱ����APB1��2�����߼���ʱ����TIM9-11��APB2
//TIM9 PWM���ֳ�ʼ�� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM9_PWM_Init(u16 arr,u16 psc)
{ 
    TIM9_Handler.Instance=TIM9;            //��ʱ��9
    TIM9_Handler.Init.Prescaler=psc;       //��ʱ����Ƶ
    TIM9_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;//���ϼ���ģʽ
    TIM9_Handler.Init.Period=arr;          //�Զ���װ��ֵ
    TIM9_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&TIM9_Handler);       //��ʼ��PWM
    
    TIM9_CH1Handler.OCMode=TIM_OCMODE_PWM1; //ģʽѡ��PWM1
    TIM9_CH1Handler.Pulse=arr/2;            //���ñȽ�ֵ,��ֵ����ȷ��ռ�ձȣ�Ĭ�ϱȽ�ֵΪ�Զ���װ��ֵ��һ��,��ռ�ձ�Ϊ50%
    TIM9_CH1Handler.OCPolarity=TIM_OCPOLARITY_HIGH; //����Ƚϼ���Ϊ�� 
    HAL_TIM_PWM_ConfigChannel(&TIM9_Handler,&TIM9_CH1Handler,TIM_CHANNEL_1);//����TIM9ͨ��1
    HAL_TIM_PWM_Start(&TIM9_Handler,TIM_CHANNEL_1);//����PWMͨ��4
	
	  TIM9_CH2Handler.OCMode=TIM_OCMODE_PWM1; //ģʽѡ��PWM1
    TIM9_CH2Handler.Pulse=arr/2;            //���ñȽ�ֵ,��ֵ����ȷ��ռ�ձȣ�Ĭ�ϱȽ�ֵΪ�Զ���װ��ֵ��һ��,��ռ�ձ�Ϊ50%
    TIM9_CH2Handler.OCPolarity=TIM_OCPOLARITY_HIGH; //����Ƚϼ���Ϊ�� 
    HAL_TIM_PWM_ConfigChannel(&TIM9_Handler,&TIM9_CH2Handler,TIM_CHANNEL_2);//����TIM9ͨ��2
    HAL_TIM_PWM_Start(&TIM9_Handler,TIM_CHANNEL_2);//����PWMͨ��2
}


void TIM4_PWM_Init(u16 arr,u16 psc)
{ 
    TIM4_Handler.Instance=TIM4;            //��ʱ��4
    TIM4_Handler.Init.Prescaler=psc;       //��ʱ����Ƶ
    TIM4_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;//���ϼ���ģʽ
    TIM4_Handler.Init.Period=arr;          //�Զ���װ��ֵ
    TIM4_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&TIM4_Handler);       //��ʼ��PWM
    
    TIM4_CH2Handler.OCMode=TIM_OCMODE_PWM1; //ģʽѡ��PWM1
    TIM4_CH2Handler.Pulse=arr/2;            //���ñȽ�ֵ,��ֵ����ȷ��ռ�ձȣ�Ĭ�ϱȽ�ֵΪ�Զ���װ��ֵ��һ��,��ռ�ձ�Ϊ50%
    TIM4_CH2Handler.OCPolarity=TIM_OCPOLARITY_HIGH; //����Ƚϼ���Ϊ�� 
    HAL_TIM_PWM_ConfigChannel(&TIM4_Handler,&TIM4_CH2Handler,TIM_CHANNEL_2);//����TIM4ͨ��2
    HAL_TIM_PWM_Start(&TIM4_Handler,TIM_CHANNEL_2);//����PWMͨ��2
	
	  TIM4_CH3Handler.OCMode=TIM_OCMODE_PWM1; //ģʽѡ��PWM1
    TIM4_CH3Handler.Pulse=arr/2;            //���ñȽ�ֵ,��ֵ����ȷ��ռ�ձȣ�Ĭ�ϱȽ�ֵΪ�Զ���װ��ֵ��һ��,��ռ�ձ�Ϊ50%
    TIM4_CH3Handler.OCPolarity=TIM_OCPOLARITY_HIGH; //����Ƚϼ���Ϊ�� 
    HAL_TIM_PWM_ConfigChannel(&TIM4_Handler,&TIM4_CH3Handler,TIM_CHANNEL_3);//����TIM4ͨ��3
    HAL_TIM_PWM_Start(&TIM4_Handler,TIM_CHANNEL_3);//����PWMͨ��3
}

//��ʱ���ײ�������ʱ��ʹ�ܣ���������
//�˺����ᱻHAL_TIM_PWM_Init()����
//htim:��ʱ�����
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
		GPIO_InitTypeDef GPIO_Initure;
	
		if(htim->Instance==TIM9)
		{
			__HAL_RCC_GPIOE_CLK_ENABLE();			//����GPIOEʱ��
			__HAL_RCC_TIM9_CLK_ENABLE();			//ʹ�ܶ�ʱ��9
		
			GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	//�����������
			GPIO_Initure.Pull=GPIO_PULLUP;          //����
			GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
		
			GPIO_Initure.Pin=GPIO_PIN_5|GPIO_PIN_6; //PE5
			GPIO_Initure.Alternate=GPIO_AF3_TIM9;	//PE5����ΪTIM9_CH1
			HAL_GPIO_Init(GPIOE,&GPIO_Initure);
		}
		else if(htim->Instance==TIM4)
		{
			__HAL_RCC_GPIOB_CLK_ENABLE();			//����GPIOBʱ��
			__HAL_RCC_TIM4_CLK_ENABLE();			//ʹ�ܶ�ʱ��4
		
			GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	//�����������
			GPIO_Initure.Pull=GPIO_PULLUP;          //����
			GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
		
			GPIO_Initure.Pin=GPIO_PIN_7|GPIO_PIN_8;           	//PB7
			GPIO_Initure.Alternate=GPIO_AF2_TIM4;	//PB7����ΪTIM4_CH2
			HAL_GPIO_Init(GPIOB,&GPIO_Initure);	
		}
		else
		{}
}


//����TIM9ͨ��1��ռ�ձ�
//compare:�Ƚ�ֵ
void Set_TIM9Chanel1_Duty(u8 Duty)
{
	static u32 compare;
	Duty=range(Duty,0,100);//ռ�ձ�0-100��Χ
	compare=TIM9_arr*Duty/100;
	TIM9->CCR1=compare; 
}

//����TIM9ͨ��2��ռ�ձ�
//compare:�Ƚ�ֵ
void Set_TIM9Chanel2_Duty(u8 Duty)
{
	static u32 compare;
	Duty=range(Duty,0,100);//ռ�ձ�0-100��Χ
	compare=TIM9_arr*Duty/100;
	TIM9->CCR2=compare;  
}

//����TIM4ͨ��2��ռ�ձ�
//compare:�Ƚ�ֵ
void Set_TIM4Chanel2_Duty(u8 Duty)
{
	static u32 compare;
	Duty=range(Duty,0,100);//ռ�ձ�0-100��Χ
	compare=TIM4_arr*Duty/100;
	TIM4->CCR2=compare; 
}

//����TIM4ͨ��3��ռ�ձ�
//compare:�Ƚ�ֵ
void Set_TIM4Chanel3_Duty(u8 Duty)
{
	static u32 compare;
	Duty=range(Duty,0,100);//ռ�ձ�0-100��Χ
	compare=TIM4_arr*Duty/100;
	TIM4->CCR3=compare; 
}
