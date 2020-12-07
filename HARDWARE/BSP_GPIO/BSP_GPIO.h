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
*                                                 MODULE
*********************************************************************************************************
*/

#ifndef _BSP_GPIO_H
#define _BSP_GPIO_H

/*
*********************************************************************************************************
*                                              INCLUDE FILES
*********************************************************************************************************
*/

#include "sys.h"


/*
*********************************************************************************************************
*                                                SERVICES
*********************************************************************************************************
*/

/****输出引脚定义****/

//LED端口定义
#define LED_ON 0
#define LED_OFF 1
#define LEDRed(n)			(n?HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET))
#define LEDRed_Toggle (HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0))



//Relay端口定义
#define Relay1_ON			(HAL_GPIO_WritePin(GPIOG,GPIO_PIN_9,GPIO_PIN_RESET))
#define Relay1_OFF		(HAL_GPIO_WritePin(GPIOG,GPIO_PIN_9,GPIO_PIN_SET))
#define Relay2_ON			(HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_RESET))
#define Relay2_OFF		(HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_SET))
#define Relay1_Toggle (HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_9))
#define Relay2_Toggle (HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3))
#define Relay_OFF			 Relay1_OFF
#define Relay_ON			 Relay1_ON


#define CH2_High       	(HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_SET))  
#define CH1_High        (HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET)) 
#define CH2_Low       	(HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_RESET))  
#define CH1_Low         (HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET)) 


#define CH2_Toggle       	(HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_12))  
#define CH1_Toggle        (HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13)) 

void GPIO_Init(void);
void RELAY_ON(void);
void RELAY_OFF(void);

#endif
