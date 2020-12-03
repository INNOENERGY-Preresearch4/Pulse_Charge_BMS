/*
*********************************************************************************************************
*
*                                     BOARD SUPPORT PACKAGE (BSP)
*
* Filename      : BSP_CAN.h
* Version       : V1.00
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                                 MODULE
*********************************************************************************************************
*/

#ifndef __BSP_CAN_H
#define __BSP_CAN_H

/*
*********************************************************************************************************
*                                              INCLUDE FILES
*********************************************************************************************************
*/

#include "sys.h"

/*
*********************************************************************************************************
*                                           	Para DEFINES
*********************************************************************************************************
*/

#define Chanel_CAN1 1
#define Chanel_CAN2 2
#define Chanel_CAN3 3

/*
*********************************************************************************************************
*                                           	    STRUCT
*********************************************************************************************************
*/

typedef struct
{
  uint32_t Id;       /*!< Specifies the standard identifier.
                             This parameter must be a number between Min_Data = 0 and Max_Data = 0x7FF */

  uint32_t DLC;         /*!< Specifies the length of the frame that will be received.
                             This parameter must be a number between Min_Data = 0 and Max_Data = 8 */

  uint8_t Data[8];      /*!< Contains the data to be received.
                             This parameter must be a number between Min_Data = 0 and Max_Data = 0xFF */

}Can_Msg;


/*
*********************************************************************************************************
*                                           	   	EXTERN
*********************************************************************************************************
*/


extern 		Can_Msg           CAN_Rx_Message;    			//接收数据缓存
extern 		Can_Msg           CAN_Tx_Message;    			//接收数据缓存

/*
*********************************************************************************************************
*                                              SERVICES
*********************************************************************************************************
*/

u8 CAN1_Mode_Init(u32 tsjw,u32 tbs2,u32 tbs1,u16 brp,u32 mode);//CAN1初始化
u8 CAN2_Mode_Init(u32 tsjw,u32 tbs2,u32 tbs1,u16 brp,u32 mode);//CAN2初始化
u8 CAN3_Mode_Init(u32 tsjw,u32 tbs2,u32 tbs1,u16 brp,u32 mode);//CAN3初始化
void HAL_CAN_MspInit(CAN_HandleTypeDef* hcan);
void CAN1_RX0_IRQHandler(void);
void CAN2_RX0_IRQHandler(void);
void CAN3_RX0_IRQHandler(void);
void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef* hcan);
u8 CAN_Send_Msg(u8 chanle,Can_Msg* Tx_Message);
u8 CAN_Receive_Msg(CAN_HandleTypeDef* hcan,Can_Msg* Rx_Message);

#endif
