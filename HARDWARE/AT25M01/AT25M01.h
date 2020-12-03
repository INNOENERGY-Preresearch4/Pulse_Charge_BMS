/*
***************************************************************************
�ӿڶ����Լ�˵��
***************************************************************************
*/

#ifndef _AT25M01_H_
#define _AT25M01_H_

#include "sys.h"


//�ֽ�д���ƣ�1��WE���ƣ�0��CE����
#define WRITE_CONTROL_WE (1)
#define WRITE_CONTROL_CE (0)
#define WRITE_CONTROL  WRITE_CONTROL_WE  //�ֽ�д��WE����


#define DIR_IN  (0)  //IO�ڷ���Ϊ����
#define DIR_OUT (1)  //IO�ڷ���Ϊ���

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//EEPROM
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//������
#define WREN                     (0x06)                    //дʹ��
#define WRDI                     (0x04)                    //��λдʹ��
#define RDSR                     (0x05)                    //��״̬�Ĵ���
#define WRSR                     (0x01)                    //д״̬�Ĵ���
#define READ                     (0x03)                    //���洢������
#define WRITE                    (0x02)                    //д�洢������
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define ADDRESS_FLASH_BIT_CHECK  (0x000000)                //EEPROM BIT���,����0xaa
#define ADDRESS_WATCHDOG_CHECK   (0x000003)                //���Ź������
#define WATCHDOG_CHECK_DATA      (0x5A)
#define BMS_INSTANT_INFO_ADDR    (0x000010)                //���˲̬��¼��Ϣ��ַ
#define MAX_BMS_INSTANT_RECORD       (1500)                //˲̬��¼�������1500��
#define FLASH_WRITE_FLAG_ADDR    (0x000004)                //д��־��ַ
#define FLASH_WRITE_FLAG         (0x56)                    //����Ƿ��Ѿ��洢����,����0x00000λ��
///////////////////////////////////////////////////////////////////////////////////////////////
 //�º���
 //�洢����ʼ��
 void memory_init(void);
 //�洢��Ƭѡʹ��
 void memory_Chip_Selection(u8 store_num,unsigned char val) ;
 //дʹ��
 void SPI0_WREN(u8 store_num);
 //�洢���ֽ�д����
 void memory_write_byte(u32 address,u8 data,u8 store_num);
 void memory_write_4byte(u32 address,uint32_t data,u8 store_num);
 //�洢��״̬������
 unsigned char Read_Status_Register(u8 store_num);
 //�洢��״̬д���� 
 void memory_write_status(u8 data,u8 store_num); 
 //��ȡ�ֽ�
u8 memory_read_byte(u32 address,u8 store_num);
uint32_t memory_read_4byte(u32 address,u8 store_num);
//ҳд����
void memory_write_page(u32 address,u8 *pdata,u32 DataLenth,u8 store_num);

#endif //#ifndef _SM28C040_H_

