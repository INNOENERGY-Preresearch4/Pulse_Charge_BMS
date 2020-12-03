/*
*********************************************************************************************************
*
*                                     				 APPLICATION
*
* Filename      : Memory.c
* Version       : V1.00
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include "Memory.h"

/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

////test eeprom
//u8 testByte[256];
//u16 t=0;
////test eeprom


/*
*********************************************************************************************************
*                                              SERVICES
*********************************************************************************************************
*/

//**********************************************************
//读取电池组剩余容量,单位毫安.ms
//**********************************************************
float Read_SOC_Remain(uint32_t address)
{//1000*36000*1000;
    u8 byte1 = 0;           
    u8 byte2 = 0;
    u8 byte3 = 0;
    u8 byte4 = 0;
	  u8 byte5 = 0;           
    u8 byte6 = 0;
    u8 byte7 = 0;
    u8 byte8 = 0;
	
    uint64_t SOC_Remain_EE = 0;
	
    byte1 = EEPROM_Read_Byte(&g_EEPROMSPIHandle,address);
    byte2 = EEPROM_Read_Byte(&g_EEPROMSPIHandle,address+1);
    byte3 = EEPROM_Read_Byte(&g_EEPROMSPIHandle,address+2);
    byte4 = EEPROM_Read_Byte(&g_EEPROMSPIHandle,address+3); 
		byte5 = EEPROM_Read_Byte(&g_EEPROMSPIHandle,address+4);
    byte6 = EEPROM_Read_Byte(&g_EEPROMSPIHandle,address+5);
    byte7 = EEPROM_Read_Byte(&g_EEPROMSPIHandle,address+6);
    byte8 = EEPROM_Read_Byte(&g_EEPROMSPIHandle,address+7);
	
    SOC_Remain_EE |= (uint64_t)byte1;
    SOC_Remain_EE |= (((uint64_t)byte2)<<8);
    SOC_Remain_EE |= (((uint64_t)byte3)<<16);
    SOC_Remain_EE |= (((uint64_t)byte4)<<24);
		SOC_Remain_EE |= (((uint64_t)byte5)<<32);
    SOC_Remain_EE |= (((uint64_t)byte6)<<40);
    SOC_Remain_EE |= (((uint64_t)byte7)<<48); 
		SOC_Remain_EE |= (((uint64_t)byte8)<<56);
		
    return ((float)SOC_Remain_EE);    
}


//***************************************************************
//保存剩余容量
//soc_remain单位毫安.秒
//***************************************************************
void Write_SOC_Remain(uint32_t address,float SOC_Remain)
{
    u8 byte1 = 0;           
    u8 byte2 = 0;
    u8 byte3 = 0;
    u8 byte4 = 0;
	  u8 byte5 = 0;           
    u8 byte6 = 0;
    u8 byte7 = 0;
    u8 byte8 = 0;
		uint64_t	SOC_Remain_EE = (uint64_t)SOC_Remain;
      
    byte1 = (u8)(SOC_Remain_EE  & 0x00000000000000ff);
    byte2 = (u8)((SOC_Remain_EE & (0x000000000000ff00))>>8);
    byte3 = (u8)((SOC_Remain_EE & (0x0000000000ff0000))>>16);
    byte4 = (u8)((SOC_Remain_EE & (0x00000000ff000000))>>24); 
	  byte5 = (u8)((SOC_Remain_EE & (0x000000ff00000000))>>32);
    byte6 = (u8)((SOC_Remain_EE & (0x0000ff0000000000))>>40);
    byte7 = (u8)((SOC_Remain_EE & (0x00ff000000000000))>>48);
    byte8 = (u8)((SOC_Remain_EE & (0xff00000000000000))>>56);
	
		EEPROM_Write_Byte(&g_EEPROMSPIHandle,address, byte1);
		EEPROM_Write_Byte(&g_EEPROMSPIHandle,address+1, byte2);
		EEPROM_Write_Byte(&g_EEPROMSPIHandle,address+2, byte3);
		EEPROM_Write_Byte(&g_EEPROMSPIHandle,address+3, byte4);
		EEPROM_Write_Byte(&g_EEPROMSPIHandle,address+4, byte5);
		EEPROM_Write_Byte(&g_EEPROMSPIHandle,address+5, byte6);
		EEPROM_Write_Byte(&g_EEPROMSPIHandle,address+6, byte7);
		EEPROM_Write_Byte(&g_EEPROMSPIHandle,address+7, byte8);
		EEPROM_Write_Byte(&g_EEPROMSPIHandle,address+8, 0x55);
}


void SOC_Filter_Memory_Upate(void)
{
		//Filter
		if((WriteSOC_Remian_Address_Filter-Address_SOC_Remain_Filter)>=(SOC_Storage_MAX_Num*9)) 
		{
				 WriteSOC_Remian_Address_Filter=Address_SOC_Remain_Filter;
		}
		Write_SOC_Remain(WriteSOC_Remian_Address_Filter,Remain_SOC_mAms_Filter);      //保存剩余电量
		if(WriteSOC_Remian_Address_Filter == Address_SOC_Remain_Filter)
		{	
				 EEPROM_Write_Byte(&g_EEPROMSPIHandle,Address_SOC_Remain_Filter+SOC_Storage_MAX_Num*9-1, 0xAA);							
		}
		else
		{
				 EEPROM_Write_Byte(&g_EEPROMSPIHandle,WriteSOC_Remian_Address_Filter-1, 0xAA);		
		}
		WriteSOC_Remian_Address_Filter += 9;
}



void SOC_Pulse_Memory_Upate(void)
{	
		//Pulse
		if((WriteSOC_Remian_Address_Pulse-Address_SOC_Remain_Pulse)>=(SOC_Storage_MAX_Num*9)) 
		{
				 WriteSOC_Remian_Address_Pulse=Address_SOC_Remain_Pulse;
		}
		Write_SOC_Remain(WriteSOC_Remian_Address_Pulse,Remain_SOC_mAms_Pulse);      //保存剩余电量
		if(WriteSOC_Remian_Address_Pulse == Address_SOC_Remain_Pulse)
		{	
				 EEPROM_Write_Byte(&g_EEPROMSPIHandle,Address_SOC_Remain_Pulse+SOC_Storage_MAX_Num*9-1, 0xAA);							
		}
		else
		{
				 EEPROM_Write_Byte(&g_EEPROMSPIHandle,WriteSOC_Remian_Address_Pulse-1, 0xAA);		
		}
		WriteSOC_Remian_Address_Pulse += 9;
}




//void EEPROM_TEST(void)
//{	
//	u16 i=0;
//	 //test eeprom
//		 for (i = 0; i < 233; ++i)
//			EEPROM_Write_Byte(&g_EEPROMSPIHandle,0x10000+i, i+t);
//			delay_ms(10);
//		 for (i = 0; i < 233; ++i)
//			testByte[i] = EEPROM_Read_Byte(&g_EEPROMSPIHandle,0x10000+i);
//			t++;
//}
