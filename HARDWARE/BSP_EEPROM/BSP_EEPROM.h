/*
*********************************************************************************************************
*
*                                     BOARD SUPPORT PACKAGE (BSP)
*
* Filename      : BSP_EEPROM.h
* Version       : V1.00
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                                 MODULE
*********************************************************************************************************
*/

#ifndef __BSP_EEPROM__
#define __BSP_EEPROM__

/*
*********************************************************************************************************
*                                              INCLUDE FILES
*********************************************************************************************************
*/

#include "main.h"
#include "delay.h"

/*
*********************************************************************************************************
*                                           	Para DEFINES
*********************************************************************************************************
*/

#define EEPROM_SPI SPI4

#define EEPROM_SPI_CLK_ENABLE()                __HAL_RCC_SPI4_CLK_ENABLE()
#define EEPROM_GPIO_CLK_ENABLE()               __HAL_RCC_GPIOE_CLK_ENABLE()
#define EEPROM_SPI_GPIO_PORT                   GPIOE
#define EEPROM_WP_PIN                          GPIO_PIN_10
#define EEPROM_SPI_CS_PIN                      GPIO_PIN_11
#define EEPROM_SPI_SCK_PIN                     GPIO_PIN_12
#define EEPROM_SPI_MISO_PIN                    GPIO_PIN_13
#define EEPROM_SPI_MOSI_PIN                    GPIO_PIN_14
#define EEPROM_HOLD_PIN                        GPIO_PIN_15
#define EEPROM_SPI_PIN_AF                      GPIO_AF5_SPI4

#define EEPROM_SPI_CS_LOW()	 		HAL_GPIO_WritePin(EEPROM_SPI_GPIO_PORT,EEPROM_SPI_CS_PIN ,GPIO_PIN_RESET)
#define EEPROM_SPI_CS_HIGH() 		HAL_GPIO_WritePin(EEPROM_SPI_GPIO_PORT,EEPROM_SPI_CS_PIN ,GPIO_PIN_SET)

#define WREN                     (0x06)                    //Ð´Ê¹ÄÜ
#define WRDI                     (0x04)                    //¸´Î»Ð´Ê¹ÄÜ
#define RDSR                     (0x05)                    //¶Á×´Ì¬¼Ä´æÆ÷
#define WRSR                     (0x01)                    //Ð´×´Ì¬¼Ä´æÆ÷
#define READ                     (0x03)                    //¶Á´æ´¢Æ÷Êý¾Ý
#define WRITE                    (0x02)                    //Ð´´æ´¢Æ÷Êý¾Ý
#define DUMMY_BYTE               (0xFF)  

/*
*********************************************************************************************************
*                                              	EXTERNS
*********************************************************************************************************
*/
extern SPI_HandleTypeDef g_EEPROMSPIHandle;

/*
*********************************************************************************************************
*                                              SERVICES
*********************************************************************************************************
*/

void Init_EEPROM_SPI(void);

void EEPROM_WRDI(SPI_HandleTypeDef *handle);
void EEPROM_WREN(SPI_HandleTypeDef *handle);
uint8_t EEPROM_Read_Status_Reg(SPI_HandleTypeDef *handle);
uint8_t EEPROM_waitWriteReady(SPI_HandleTypeDef *handle);
uint8_t EEPROM_Read_Byte(SPI_HandleTypeDef *handle,uint32_t addr);
void EEPROM_Write_Byte(SPI_HandleTypeDef *handle,uint32_t addr, uint8_t data);
void EEPROM_Write_Status_Reg(SPI_HandleTypeDef *handle,uint8_t data);

#endif
