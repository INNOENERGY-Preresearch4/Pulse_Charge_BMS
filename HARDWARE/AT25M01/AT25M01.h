/*
***************************************************************************
接口定义以及说明
***************************************************************************
*/

#ifndef _AT25M01_H_
#define _AT25M01_H_

#include "sys.h"


//字节写控制，1由WE控制，0由CE控制
#define WRITE_CONTROL_WE (1)
#define WRITE_CONTROL_CE (0)
#define WRITE_CONTROL  WRITE_CONTROL_WE  //字节写由WE控制


#define DIR_IN  (0)  //IO口方向为输入
#define DIR_OUT (1)  //IO口方向为输出

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//EEPROM
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//新数据
#define WREN                     (0x06)                    //写使能
#define WRDI                     (0x04)                    //复位写使能
#define RDSR                     (0x05)                    //读状态寄存器
#define WRSR                     (0x01)                    //写状态寄存器
#define READ                     (0x03)                    //读存储器数据
#define WRITE                    (0x02)                    //写存储器数据
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define ADDRESS_FLASH_BIT_CHECK  (0x000000)                //EEPROM BIT检测,数据0xaa
#define ADDRESS_WATCHDOG_CHECK   (0x000003)                //看门狗检测用
#define WATCHDOG_CHECK_DATA      (0x5A)
#define BMS_INSTANT_INFO_ADDR    (0x000010)                //电池瞬态记录信息地址
#define MAX_BMS_INSTANT_RECORD       (1500)                //瞬态记录最大条数1500条
#define FLASH_WRITE_FLAG_ADDR    (0x000004)                //写标志地址
#define FLASH_WRITE_FLAG         (0x56)                    //标记是否已经存储数据,放在0x00000位置
///////////////////////////////////////////////////////////////////////////////////////////////
 //新函数
 //存储器初始化
 void memory_init(void);
 //存储器片选使能
 void memory_Chip_Selection(u8 store_num,unsigned char val) ;
 //写使能
 void SPI0_WREN(u8 store_num);
 //存储器字节写函数
 void memory_write_byte(u32 address,u8 data,u8 store_num);
 void memory_write_4byte(u32 address,uint32_t data,u8 store_num);
 //存储器状态读函数
 unsigned char Read_Status_Register(u8 store_num);
 //存储器状态写函数 
 void memory_write_status(u8 data,u8 store_num); 
 //读取字节
u8 memory_read_byte(u32 address,u8 store_num);
uint32_t memory_read_4byte(u32 address,u8 store_num);
//页写操作
void memory_write_page(u32 address,u8 *pdata,u32 DataLenth,u8 store_num);

#endif //#ifndef _SM28C040_H_

