/*###########################*/
#ifndef _CSM300A_H_
#define _CSM300A_H_
//#define WARMING
#include "includes.h"

#define CAN4_CFG_1()  (HAL_GPIO_WritePin(GPIOG,GPIO_PIN_10,GPIO_PIN_SET))
#define CAN4_CFG_0()  (HAL_GPIO_WritePin(GPIOG,GPIO_PIN_10,GPIO_PIN_RESET))
#define CAN4_RST_1()  (HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_SET))
#define CAN4_RST_0()  (HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_RESET))

#define START_MARK   (0XA5)
#define STOP_MARK   (0X5A)
uint8_t Csm300A_Init(void);//初始化函数
uint8_t Csm100TL_Init(void);
void CAN4_DATA_TRANSIMIT(u8 flag);//CSM300A数据解析
void CAN4_Send(u32 id,u8 *data,u8 length);//CSM300A的CAN发送
#endif

