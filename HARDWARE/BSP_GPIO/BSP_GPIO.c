/*
*********************************************************************************************************
*
*                                     BOARD SUPPORT PACKAGE (BSP)
*
* Filename      : BSP_GPIO.h
* Version       : V1.00
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include "BSP_GPIO.h"

/*
*********************************************************************************************************
*                                                SERVICES
*********************************************************************************************************
*/


void GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
		__HAL_RCC_GPIOC_CLK_ENABLE();			//����GPIOCʱ��
		__HAL_RCC_GPIOB_CLK_ENABLE();			//����GPIOBʱ��
		__HAL_RCC_GPIOA_CLK_ENABLE();			//����GPIOAʱ��
		__HAL_RCC_GPIOG_CLK_ENABLE();			//����GPIOGʱ��
	
		/*********************OUT************************/
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
	
		GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_3; //
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);

		GPIO_Initure.Pin=GPIO_PIN_9; //
		HAL_GPIO_Init(GPIOG,&GPIO_Initure);

		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);	//LEDRed��	
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_RESET);	//Relay2�ر�
		HAL_GPIO_WritePin(GPIOG,GPIO_PIN_9,GPIO_PIN_SET);	//Relay1�ر�
//		
//		/*********************IN************************/
//    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //����
//    GPIO_Initure.Pull=GPIO_NOPULL;        //����
//    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
			
		GPIO_Initure.Pin=GPIO_PIN_12;  //ʾ����ͨ��2
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
		
		GPIO_Initure.Pin=GPIO_PIN_13;  //ʾ����ͨ��1
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
		
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_SET);	//
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);	//

}


