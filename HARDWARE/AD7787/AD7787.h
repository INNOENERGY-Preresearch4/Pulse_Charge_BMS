/*###########################*/
#ifndef _AD7787_H_
#define _AD7787_H_
//#define WARMING
#include "sys.h"
#define SPI1_CS_1()     (HAL_GPIO_WritePin(GPIOF,GPIO_PIN_6,GPIO_PIN_SET))	//SPI1_CS1��1
#define SPI1_CS_0()     (HAL_GPIO_WritePin(GPIOF,GPIO_PIN_6,GPIO_PIN_RESET))	//SPI1_CS1��0
uint8_t AD7787_Init(uint8_t chanle);//��ʼ������
int32_t AD7787_Read_Value(uint8_t chanle,uint8_t ain);//�ɼ�ֵ��ȡ����
uint8_t AD7787_Read_Status(uint8_t chanle);//�ɼ�ֵ��ȡ����
#endif

