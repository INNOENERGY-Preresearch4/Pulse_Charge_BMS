/*
*********************************************************************************************************
*
*                                     BOARD SUPPORT PACKAGE (BSP)
*                                                                    
*
* Filename      : BSP_EEPROM.c
* Version       : V1.00
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include "BSP_EEPROM.h"

/*
*********************************************************************************************************
*                                          LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

SPI_HandleTypeDef g_EEPROMSPIHandle;

/*
*********************************************************************************************************
*                                                SERVICES
*********************************************************************************************************
*/

void Init_EEPROM_SPI(void)
{
	
		GPIO_InitTypeDef GPIO_InitStruct;
		EEPROM_SPI_CLK_ENABLE();                
		EEPROM_GPIO_CLK_ENABLE();               
		
		//CS PIN, WP PIN , HOLD PIN
		GPIO_InitStruct.Pin = EEPROM_SPI_CS_PIN | EEPROM_WP_PIN | EEPROM_HOLD_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
		HAL_GPIO_Init(EEPROM_SPI_GPIO_PORT, &GPIO_InitStruct);
	
		EEPROM_SPI_CS_HIGH();
		//WP PIN HIGH, HOLD Pin HIGH
		HAL_GPIO_WritePin(EEPROM_SPI_GPIO_PORT,EEPROM_WP_PIN ,GPIO_PIN_SET);
		HAL_GPIO_WritePin(EEPROM_SPI_GPIO_PORT,EEPROM_HOLD_PIN ,GPIO_PIN_SET);
		 
		//SCK Pin, MISO Pin, MOSI Pin.
		GPIO_InitStruct.Pin = EEPROM_SPI_SCK_PIN | EEPROM_SPI_MISO_PIN | EEPROM_SPI_MOSI_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
		GPIO_InitStruct.Alternate = EEPROM_SPI_PIN_AF;
		HAL_GPIO_Init(EEPROM_SPI_GPIO_PORT, &GPIO_InitStruct);
				
				
    g_EEPROMSPIHandle.Instance               = EEPROM_SPI;
		g_EEPROMSPIHandle.Init.Mode              = SPI_MODE_MASTER;
    g_EEPROMSPIHandle.Init.Direction         = SPI_DIRECTION_2LINES;	
    g_EEPROMSPIHandle.Init.DataSize          = SPI_DATASIZE_8BIT;
    g_EEPROMSPIHandle.Init.CLKPolarity       = SPI_POLARITY_HIGH;
    g_EEPROMSPIHandle.Init.CLKPhase          = SPI_PHASE_2EDGE;
    g_EEPROMSPIHandle.Init.NSS               = SPI_NSS_SOFT;
    g_EEPROMSPIHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
    g_EEPROMSPIHandle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
    g_EEPROMSPIHandle.Init.TIMode            = SPI_TIMODE_DISABLE;
    g_EEPROMSPIHandle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
    g_EEPROMSPIHandle.Init.CRCPolynomial     = 7; 

		HAL_SPI_Init(&g_EEPROMSPIHandle);
    __HAL_SPI_ENABLE(&g_EEPROMSPIHandle);                 //使能SPI4
}



void EEPROM_WRDI(SPI_HandleTypeDef *handle)
{
    uint8_t data = WRDI;
    EEPROM_SPI_CS_LOW();
 //   HAL_SPI_Transmit_IT(&g_EEPROMSPIHandle, &data, 1);
    HAL_SPI_Transmit(handle, &data, 1,1000);
    EEPROM_SPI_CS_HIGH();
    //回调函数中拉高CS
}


void EEPROM_WREN(SPI_HandleTypeDef *handle)
{
    uint8_t data = WREN;
//    uint8_t rx_data;
//    HAL_StatusTypeDef retStatus;

//    while(!(g_EEPROMSPIHandle.State == HAL_SPI_STATE_READY) ||
//	  (g_EEPROMSPIHandle.State == HAL_SPI_STATE_BUSY_RX))
//  {
//	  delay_ms(1);
//  }
     EEPROM_SPI_CS_LOW();
   //retStatus = HAL_SPI_TransmitReceive_IT(&g_EEPROMSPIHandle,(uint8_t*)data, (uint8_t*)rx_data, 1);
   //  retStatus =HAL_SPI_Transmit_IT(&g_EEPROMSPIHandle, &data, 1);
    HAL_SPI_Transmit(handle, &data, 1,1000);
    EEPROM_SPI_CS_HIGH();
     //回调函数中拉高CS
}


uint8_t EEPROM_Read_Status_Reg(SPI_HandleTypeDef *handle)
{
     uint8_t tx_data[2] = {RDSR, DUMMY_BYTE};
     uint8_t rx_data[2] = {0,0};    
     HAL_StatusTypeDef retStatus;
     EEPROM_SPI_CS_LOW();
  //   retStatus = HAL_SPI_TransmitReceive_IT(&g_EEPROMSPIHandle,(uint8_t*)tx_data, (uint8_t*)rx_data, 2);
     retStatus = HAL_SPI_TransmitReceive(handle,(uint8_t*)tx_data, (uint8_t*)rx_data, 2,1000);
     EEPROM_SPI_CS_HIGH();
     //回调函数中拉高CS
     return rx_data[1];
}


void EEPROM_Write_Status_Reg(SPI_HandleTypeDef *handle,uint8_t data)
{
    uint8_t tx_data[2] = {WRSR, 0};
    HAL_StatusTypeDef retStatus;    
    tx_data[1]  = data;
     
     EEPROM_SPI_CS_LOW();
   //  retStatus = HAL_SPI_Transmit_IT(&g_EEPROMSPIHandle,(uint8_t*)tx_data, 2);    
     retStatus = HAL_SPI_Transmit(handle,(uint8_t*)tx_data, 2,1000); 
     EEPROM_SPI_CS_HIGH();    
}


uint8_t EEPROM_waitWriteReady(SPI_HandleTypeDef *handle)
{
    uint8_t i = 0;
    while((0x01 & EEPROM_Read_Status_Reg(handle)) != 0)
    {
        if(i++ > 100) 
	     {
	      //  break;
	     }
    }
}


uint8_t EEPROM_Read_Byte(SPI_HandleTypeDef *handle,uint32_t addr)
{
    uint8_t tx_data[5] = {0};//{READ, (0x010000&addr)>>16, (0x00FF00&addr)>>8, 0x0000FF&addr,DUMMY_BYTE};
    uint8_t rx_data[5] = {0};
    
    tx_data[0] = READ;
    tx_data[1] = (uint8_t)((0x010000&addr)>>16);
    tx_data[2] = (uint8_t)((0x00FF00&addr)>>8);
    tx_data[3] = (uint8_t)(0x0000FF&addr);
    tx_data[4] = DUMMY_BYTE;
    EEPROM_waitWriteReady(handle);
    EEPROM_WREN(handle);
    
    EEPROM_SPI_CS_LOW();
    
 // HAL_SPI_TransmitReceive_IT(&g_EEPROMSPIHandle,(uint8_t*)tx_data, (uint8_t*)rx_data, 5);
   HAL_SPI_TransmitReceive(handle,(uint8_t*)tx_data, (uint8_t*)rx_data, 5,1000);
   EEPROM_SPI_CS_HIGH();
    return rx_data[4];
}


void EEPROM_Write_Byte(SPI_HandleTypeDef *handle,uint32_t addr, uint8_t data)
{
    uint8_t tx_data[5] = {0};//{WRITE, (0x010000&addr)>>16, (0x00FF00&addr)>>8, 0x0000FF&addr,data};
    tx_data[0] = WRITE;
    tx_data[1] = (uint8_t)((0x010000&addr)>>16);
    tx_data[2] = (uint8_t)((0x00FF00&addr)>>8);
    tx_data[3] = (uint8_t)(0x0000FF&addr);
    tx_data[4] = data;
    
    EEPROM_waitWriteReady(handle);
    EEPROM_WREN(handle);
    
    EEPROM_SPI_CS_LOW();
  //HHAL_SPI_Transmit_IT(&g_EEPROMSPIHandle, (uint8_t*)tx_data, 5);   
    HAL_SPI_Transmit(handle, (uint8_t*)tx_data, 5,1000);   
    EEPROM_SPI_CS_HIGH();
}
