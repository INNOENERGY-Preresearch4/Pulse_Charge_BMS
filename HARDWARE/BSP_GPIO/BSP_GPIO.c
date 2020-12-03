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
		__HAL_RCC_GPIOC_CLK_ENABLE();			//开启GPIOC时钟
		__HAL_RCC_GPIOB_CLK_ENABLE();			//开启GPIOB时钟
		__HAL_RCC_GPIOA_CLK_ENABLE();			//开启GPIOA时钟
		__HAL_RCC_GPIOG_CLK_ENABLE();			//开启GPIOG时钟
	
		/*********************OUT************************/
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
	
		GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_3; //
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);

		GPIO_Initure.Pin=GPIO_PIN_9; //
		HAL_GPIO_Init(GPIOG,&GPIO_Initure);

		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);	//LEDRed亮	
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_RESET);	//Relay2关闭
		HAL_GPIO_WritePin(GPIOG,GPIO_PIN_9,GPIO_PIN_SET);	//Relay1关闭
//		
//		/*********************IN************************/
//    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //输入
//    GPIO_Initure.Pull=GPIO_NOPULL;        //下拉
//    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
			
		GPIO_Initure.Pin=GPIO_PIN_12;  //示波器通道2
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
		
		GPIO_Initure.Pin=GPIO_PIN_13;  //示波器通道1
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
		
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_SET);	//
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);	//

}


