/*
*********************************************************************************************************
*
*                                     BOARD SUPPORT PACKAGE (BSP)
*                                                                    
*
* Filename      : BSP_LTC6804.c
* Version       : V1.00
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include "BSP_LTC6804.h"
#include  "Globals.h"

/*
*********************************************************************************************************
*                                         LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

LTC6804_handle 		g_LTC6804Handle;
SPI_HandleTypeDef SPI2_Handler;
s8 	volErr, tempErr;

/*
*********************************************************************************************************
*                                                SERVICES
*********************************************************************************************************
*/
//初始化SPI管脚和SPI功能
void SPI2_Init(void)
{
    //硬件初始化
    GPIO_InitTypeDef  GPIO_InitStruct;
    LTC6804SPI_CLK_ENABLE();
    LTC6804SPI_GPIO_CLK_ENABLE();  

    GPIO_InitStruct.Pin=LTC6804SPI_CS_PIN ; 
    GPIO_InitStruct.Mode=GPIO_MODE_OUTPUT_PP;  
    GPIO_InitStruct.Pull=GPIO_PULLUP;          
    GPIO_InitStruct.Speed=GPIO_SPEED_HIGH;      
    HAL_GPIO_Init(LTC6804SPI_GPIO_PORT,&GPIO_InitStruct);         

    GPIO_InitStruct.Pin       = LTC6804SPI_SCK_PIN|LTC6804SPI_MISO_PIN|LTC6804SPI_MOSI_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = LTC6804SPI_PIN_AF;
    HAL_GPIO_Init(LTC6804SPI_GPIO_PORT, &GPIO_InitStruct);

    HAL_GPIO_WritePin(LTC6804SPI_GPIO_PORT,LTC6804SPI_CS_PIN ,GPIO_PIN_SET);	
    
    //SPI初始化
    SPI2_Handler.Instance               = LTC6804SPI;
		SPI2_Handler.Init.Mode 						  = SPI_MODE_MASTER;
		SPI2_Handler.Init.Direction         = SPI_DIRECTION_2LINES;
		SPI2_Handler.Init.DataSize          = SPI_DATASIZE_8BIT;
		SPI2_Handler.Init.CLKPolarity       = SPI_POLARITY_HIGH;
		SPI2_Handler.Init.CLKPhase          = SPI_PHASE_2EDGE;
		SPI2_Handler.Init.NSS               = SPI_NSS_SOFT;
    SPI2_Handler.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
    SPI2_Handler.Init.FirstBit          = SPI_FIRSTBIT_MSB;
    SPI2_Handler.Init.TIMode            = SPI_TIMODE_DISABLE;
    SPI2_Handler.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
    SPI2_Handler.Init.CRCPolynomial     = 7; 

		HAL_SPI_Init(&SPI2_Handler);
		__HAL_SPI_ENABLE(&SPI2_Handler);       
}

//初始化6804
void LTC6804_Init(LTC6804_handle *handle, LTC6804ConfigReg *reg)
{
    int8_t currentIC = 0 ;
    
    for(currentIC = 0; currentIC < TOTAL_IC; currentIC++)
    {
        //GPIO5 GPIO4 GPIO3 GPIO2 GPIO1 REFON(基准上电) SWTRD
        //ADCOPT ADC模式选项位
        handle->chipConfig[currentIC][0] = 0xFE;
        
        handle->chipConfig[currentIC][1] = 0;
        handle->chipConfig[currentIC][2] = 0;
        handle->chipConfig[currentIC][3] = 0;
 
        //DCC[X] 对电池X进行放电 
        //1->接通 
        //0->断开
        //DCC8 DCC7 DCC6 DCC5 DCC4 DCC3 DCC2 DCC1
        handle->chipConfig[currentIC][4] = 0;
        
        //DCTO[3] DCTO[2] DCTO[1] DCTO[0] DCC12 DCC11 DCC10 DCC9
        handle->chipConfig[currentIC][5] = 0;
    }
    
    LTC6804_set_ADC(handle, /*MD_FAST*/MD_NORMAL,DCP_DISABLED,CELL_CH_ALL,AUX_CH_ALL);   
}

/*
|command	|  15   |  14   |  13   |  12   |  11   |  10   |   9   |   8   |   7   |   6   |   5   |   4   |   3   |   2   |   1   |   0   | 
|-----------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|
|ADCV:	    |   0   |   0   |   0   |   0   |   0   |   0   |   1   | MD[1] | MD[0] |   1   |   1   |  DCP  |   0   | CH[2] | CH[1] | CH[0] | 
|ADAX:	    |   0   |   0   |   0   |   0   |   0   |   1   |   0   | MD[1] | MD[0] |   1   |   1   |  0    |   0   | CHG[2]| CHG[1]| CHG[0]| 
 ******************************************************************************************************************/
void LTC6804_set_ADC(LTC6804_handle * handle, uint8_t MD, uint8_t DCP, uint8_t CH, uint8_t CHG)
{
    uint8_t mb_bits=0;
    
    mb_bits = (MD & 0x02) >> 1; // MD[1]
    handle->ADCV[0] = mb_bits + 0x02; //|   0   |   0   |   0   |   0   |   0   |   0   |   1   | MD[1]
    mb_bits = (MD & 0x01) << 7;
    handle->ADCV[1] = mb_bits + 0x60 + (DCP << 4) + CH; //MD[0] |   1   |   1   |  DCP  |   0   | CH[2] | CH[1] | CH[0]
    
    mb_bits = (MD & 0x02) >> 1; // MD[1]
    handle->ADAX[0] = mb_bits + 0x04; //|   0   |   0   |   0   |   0   |   0   |   1   |   0   | MD[1]
    mb_bits = (MD & 0x01) << 7; 
    handle->ADAX[1] = mb_bits + 0x60 + CHG; //MD[0] |   1   |   1   |  0  |   0   | CH[2] | CH[1] | CH[0]
}

void wakeup_idle()
{
    LTC6804SPI_CS_LOW();
    delay_us(30);//delay us
    LTC6804SPI_CS_HIGH();
		// delay_us(30);
}
void wakeup_sleep()
{
    LTC6804SPI_CS_LOW();
    delay_us(700);//delay us
    LTC6804SPI_CS_HIGH();
		// delay_us(30);	
}

//校检值计算，查表
uint16_t pec15_calc(uint8_t len, //Number of bytes that will be used to calculate a PEC
					uint8_t *data //Array of data that will be used to calculate  a PEC
					)
{
	uint16_t remainder = 16,addr;
	uint8_t i;
//initialize the PEC
	for(i = 0; i<len;i++) // loops for each byte in data array
	{
		addr = ((remainder>>7)^data[i])&0xff;//calculate PEC table address 
		remainder = (remainder<<8)^crc15Table[addr]; 
	}
	return(remainder*2);//The CRC15 has a 0 in the LSB so the remainder must be multiplied by 2
}

//均衡
void LTC6804_balance_cell(LTC6804_handle *handle, uint16_t cell_num)
{
//    if ((cell_num >= 1) && (cell_num <= 8))
//    {
//        handle->chipConfig[0][4] = 1 << (cell_num-1);
//        handle->chipConfig[0][5] = 0;
////        handle->chipConfig[1][4] = 0;
////        handle->chipConfig[1][5] = 0;
//    }
//    else if ((cell_num >=9) && (cell_num <= 12))
//    {
//        handle->chipConfig[0][4] = 0;
//        handle->chipConfig[0][5] = 1 << (cell_num - 9);
////        handle->chipConfig[1][4] = 0;
////        handle->chipConfig[1][5] = 0;
//    }
////    else if ((cell_num >=13) && (cell_num <= 20))
////    {
////        handle->chipConfig[0][4] = 0;
////        handle->chipConfig[0][5] = 0;
////        handle->chipConfig[1][4] = 1 << (cell_num - 13);
////        handle->chipConfig[1][5] = 0;
////    }
////    else if ((cell_num >=21) && (cell_num <= 24))
////    {
////        handle->chipConfig[0][4] = 0;
////        handle->chipConfig[0][5] = 0;
////        handle->chipConfig[1][4] = 0;
////        handle->chipConfig[1][5] = 1 << (cell_num - 21);
////    }
//    else 
//    {
//        handle->chipConfig[0][4] = 0;
//        handle->chipConfig[0][5] = 0;
////        handle->chipConfig[1][4] = 0;
////        handle->chipConfig[1][5] = 0;
//    }
		handle->chipConfig[0][4] = (cell_num&0xFF);
		handle->chipConfig[0][5] = ((cell_num&0xFF00)>>8);
    LTC6804_wrcfg(handle);
}

//写LTC6804配置寄存器
void LTC6804_wrcfg(LTC6804_handle * handle)
{
  uint16_t cfg_pec;
  uint8_t cmd_index; //command counter
  int8_t current_ic = 0;
  int8_t current_byte = 0;
  HAL_StatusTypeDef status;

  // WRCFG + pec 命令码及包错误检测码
  (handle->spiTxBuf)[0] = 0x00;
  (handle->spiTxBuf)[1] = 0x01;
  (handle->spiTxBuf)[2] = 0x3d;
  (handle->spiTxBuf)[3] = 0x6e;

  //两片 LTC6804
  cmd_index = 4;
  for (current_ic = TOTAL_IC; current_ic > 0; current_ic--) 			// executes for each LTC6804 in daisy chain, this loops starts with
  {																				// the last IC on the stack. The first configuration written is																		// received by the last IC in the daisy chain
    //寄存器6个字节 先发送堆叠器件的最后一个，最后发送第一个(SPI接口)
    for (current_byte = 0; current_byte < REG_BYTES; current_byte++) // executes for each of the 6 bytes in the CFGR register
    {																			// current_byte is the byte counter

    	(handle->spiTxBuf)[cmd_index] = (handle->chipConfig)[current_ic-1][current_byte]; 						//adding the config data to the array to be sent
        cmd_index = cmd_index + 1;
    }
	//包错误检测码 
    cfg_pec = (uint16_t)pec15_calc(REG_BYTES, &((handle->chipConfig)[current_ic-1][0]));		// calculating the PEC for each ICs configuration register data
    (handle->spiTxBuf)[cmd_index] = (uint8_t)(cfg_pec >> 8);
    (handle->spiTxBuf)[cmd_index + 1] = (uint8_t)cfg_pec;
    cmd_index = cmd_index + 2;
  }

  //4
  wakeup_idle(); 	//This will guarantee that the LTC6804 isoSPI port is awake.This command can be removed.


  // 中断发送
  LTC6804SPI_CS_LOW();
  //HAL_SPI_Transmit_IT(handle->ltc6804_hspi, handle->spiTxBuf, CMD_LEN+(8*TOTAL_IC));
  status = HAL_SPI_Transmit(&SPI2_Handler, handle->spiTxBuf, CMD_LEN+(8*TOTAL_IC),100);
  LTC6804SPI_CS_HIGH();;
  //片选信号发送完成回调函数中拉高(中断中用)
}

int8_t LTC6804_rdcfg(LTC6804_handle * handle)
{
    int8_t pec_error = 0;
    uint16_t data_pec;
    uint16_t received_pec;
    int8_t current_ic = 0;
    int8_t current_byte = 0;
    //1
    // RDCFG + pec15
    (handle->spiTxBuf)[0] = 0x00;
    (handle->spiTxBuf)[1] = 0x02;
    (handle->spiTxBuf)[2] = 0x2b;
    (handle->spiTxBuf)[3] = 0x0A;

    //2
    wakeup_idle(); //This will guarantee that the LTC6804 isoSPI port is awake. This command can be removed.

    LTC6804SPI_CS_LOW();
    //BYTES_IN_REG = 8
    HAL_SPI_TransmitReceive(&SPI2_Handler, handle->spiTxBuf, handle->spiRxBuf, CMD_LEN + BYTES_IN_REG * TOTAL_IC,100);
    LTC6804SPI_CS_HIGH();

    for (current_ic = 0; current_ic < TOTAL_IC; current_ic++) 			
    { 																			
																				
        for (current_byte = 0; current_byte < BYTES_IN_REG; current_byte++)
        {
            (handle->chipConfig)[current_ic][current_byte] = (handle->spiRxBuf)[CMD_LEN + (current_ic*BYTES_IN_REG) + current_byte];
        }
        received_pec = ((handle->chipConfig)[current_ic][6]<<8) + (handle->chipConfig)[current_ic][7];	// Extract the last 2 bytes of the received data buffer
        data_pec = pec15_calc(6, &((handle->chipConfig)[current_ic][0]));
        if(received_pec != data_pec)
        {
            pec_error = -1;
        }
    }

    return(pec_error);
}
//启动LTC6804 ADC转换
void LTC6804_adcv(LTC6804_handle * handle)
{
    uint16_t cmd_pec;

    //1
    (handle->spiTxBuf)[0] = (handle->ADCV)[0];
    (handle->spiTxBuf)[1] = (handle->ADCV)[1];

    //2
    cmd_pec = pec15_calc(2, handle->ADCV);
    (handle->spiTxBuf)[2] = (uint8_t)(cmd_pec >> 8);
    (handle->spiTxBuf)[3] = (uint8_t)(cmd_pec);

    //3
    wakeup_idle (); //This will guarantee that the LTC6804 isoSPI port is awake. This command can be removed.

    LTC6804SPI_CS_LOW();
    //HAL_SPI_Transmit_IT(handle->ltc6804_hspi, handle->spiTxBuf, CMD_LEN);
    HAL_SPI_Transmit(&SPI2_Handler, handle->spiTxBuf, CMD_LEN,100);
    LTC6804SPI_CS_HIGH();
}
//启动LTC6804的GPIO ADC转换
void LTC6804_adax(LTC6804_handle * handle)
{
    uint16_t cmd_pec;

    (handle->spiTxBuf)[0] = handle->ADAX[0];
    (handle->spiTxBuf)[1] = handle->ADAX[1];
    cmd_pec = pec15_calc(2, handle->ADAX);
    (handle->spiTxBuf)[2] = (uint8_t)(cmd_pec >> 8);
    (handle->spiTxBuf)[3] = (uint8_t)(cmd_pec);

    wakeup_idle (); //This will guarantee that the LTC6804 isoSPI port is awake. This command can be removed.

    LTC6804SPI_CS_LOW();
//   HAL_SPI_Transmit_IT(handle->ltc6804_hspi, handle->spiTxBuf, CMD_LEN);
    HAL_SPI_Transmit(&SPI2_Handler, handle->spiTxBuf, CMD_LEN,100); 
    LTC6804SPI_CS_HIGH();
}
void LTC6804_rdcv_reg(LTC6804_handle * handle, uint8_t reg)
{
    uint16_t cmd_pec;
    if (1 == reg) //寄存器A
    {
        (handle->spiTxBuf)[0] = 0x00;
        (handle->spiTxBuf)[1] = 0x04;
    }
    else if (2 == reg) //寄存器B
    {
        (handle->spiTxBuf)[0] = 0x00;
        (handle->spiTxBuf)[1] = 0x06;
    }
    else if (3 == reg)
    {
        (handle->spiTxBuf)[0] = 0x00;
        (handle->spiTxBuf)[1] = 0x08;
    }
        else if (4 == reg)
    {
        (handle->spiTxBuf)[0] = 0x00;
        (handle->spiTxBuf)[1] = 0x0A;
    }
    
    cmd_pec = pec15_calc(2, handle->spiTxBuf);
    (handle->spiTxBuf)[2] = (uint8_t)(cmd_pec >> 8);
    (handle->spiTxBuf)[3] = (uint8_t)(cmd_pec);
    
    wakeup_idle();
    
    LTC6804SPI_CS_LOW();
//    HAL_SPI_TransmitReceive_IT(handle->ltc6804_hspi, handle->spiTxBuf, 
//                                handle->spiRxBuf, CMD_LEN + (BYTES_IN_REG * TOTAL_IC));
     HAL_SPI_TransmitReceive(&SPI2_Handler, handle->spiTxBuf, 
                                handle->spiRxBuf, CMD_LEN + (BYTES_IN_REG * TOTAL_IC),100);
    LTC6804SPI_CS_HIGH();
    
    
}
int8_t LTC6804_rdcv(LTC6804_handle * handle, uint8_t reg)
{
    int8_t pec_error = 0;
    uint16_t  parse_cell = 0;
    uint16_t  receive_pec = 0;
    uint16_t  data_pec = 0;
    int8_t  data_counter = 0;
    int8_t cell_reg = 0; //A B C D
    int8_t current_ic = 0;
    int8_t current_cell = 0;
    
    //
    if (0 == reg)
    {
        for (cell_reg = 1; cell_reg < 5; ++cell_reg)
        {
            data_counter = 0;
            LTC6804_rdcv_reg(handle, cell_reg);
            
            for (current_ic = 0; current_ic < TOTAL_IC; ++current_ic)
            {
                for (current_cell = 0; current_cell < CELL_IN_REG; ++current_cell)
                {
                    parse_cell = (handle->spiRxBuf)[data_counter+CMD_LEN] +
                                    (handle->spiRxBuf[data_counter + 1 + CMD_LEN] << 8);
                    (handle->cellVolts)[current_ic][current_cell + (cell_reg-1)*CELL_IN_REG] = parse_cell;
                    data_counter += 2;
                    
                }
                receive_pec = ((handle->spiRxBuf)[data_counter+CMD_LEN] << 8) + 
                                    handle->spiRxBuf[data_counter + 1 + CMD_LEN];
                data_pec = pec15_calc(REG_BYTES, &((handle->spiRxBuf)[current_ic*BYTES_IN_REG+CMD_LEN]));
                if (receive_pec != data_pec)
                {
                    pec_error = -1;
                }
                data_counter += 2;
            }
        }
    }
    else
    {
        LTC6804_rdcv_reg(handle, cell_reg);

        for (current_ic = 0; current_ic < TOTAL_IC; ++current_ic)
        {
            for (current_cell = 0; current_cell < CELL_IN_REG; ++current_cell)
            {
                parse_cell = (handle->spiRxBuf)[data_counter+CMD_LEN] | 
                                (handle->spiRxBuf[data_counter + 1 + CMD_LEN] << 8);
                (handle->cellVolts)[current_ic][current_cell + (cell_reg-1)*CELL_IN_REG] = parse_cell/10;
                data_counter += 2;
                
            }
            receive_pec = ((handle->spiRxBuf)[data_counter+CMD_LEN] << 8)| 
                                handle->spiRxBuf[data_counter + 1 + CMD_LEN];
            data_pec = pec15_calc(REG_BYTES, &((handle->spiRxBuf)[current_ic*BYTES_IN_REG+CMD_LEN]));
            if (receive_pec != data_pec)
            {
                pec_error = -1;
            }
            data_counter += 2;
        }
    }
    
    return pec_error;   
}
void LTC6804_rdaux_reg(LTC6804_handle * handle, uint8_t reg)
{
  uint16_t cmd_pec;

  //1
  if(reg == 2)		// Read back auxiliary group B
  {
	  (handle->spiTxBuf)[1] = 0x0E;
	  (handle->spiTxBuf)[0] = 0x00;
  }
  else				// Read back auxiliary group A
  {
	  (handle->spiTxBuf)[1] = 0x0C;
	  (handle->spiTxBuf)[0] = 0x00;
  }

  //2
  cmd_pec = pec15_calc(2, (handle->spiTxBuf));
  (handle->spiTxBuf)[2] = (uint8_t)((cmd_pec >> 8) & 0xFF);
  (handle->spiTxBuf)[3] = (uint8_t)((cmd_pec) & 0xFF);

  //3
  wakeup_idle(); //This will guarantee that the LTC6804 isoSPI port is awake, this command can be removed.

  LTC6804SPI_CS_LOW();
//HAL_SPI_TransmitReceive_IT(handle->ltc6804_hspi, handle->spiTxBuf, handle->spiRxBuf, CMD_LEN + (BYTES_IN_REG*TOTAL_IC));
  HAL_SPI_TransmitReceive(&SPI2_Handler, handle->spiTxBuf, handle->spiRxBuf, CMD_LEN + (BYTES_IN_REG*TOTAL_IC),100);
  LTC6804SPI_CS_HIGH();
}
int8_t LTC6804_rdaux(LTC6804_handle* handle, uint8_t reg)
{
    uint8_t data_counter = 0;
    int8_t pec_error = 0;
    uint16_t parsed_aux;
    uint16_t received_pec;
    uint16_t data_pec;
    int8_t gpio_reg = 0;
    int8_t current_ic = 0;
    int8_t current_gpio = 0;
  //1.a
    if (reg == 0)
    {
	//a.i
        for(gpio_reg = 1; gpio_reg <= NUM_AUX_REG; gpio_reg++)		 	   		 			//executes once for each of the LTC6804 aux voltage registers
        {
            data_counter = 0;
            LTC6804_rdaux_reg(handle, gpio_reg);											//Reads the raw auxiliary register data into the data[] array

            for ( current_ic = 0 ; current_ic < TOTAL_IC; current_ic++) 			// executes for every LTC6804 in the daisy chain
            {																 	  			// current_ic is used as the IC counter
            //a.ii
                for(current_gpio = 0; current_gpio< GPIO_IN_REG; current_gpio++)	// This loop parses the read back data into GPIO voltages, it
                {														   		  			// loops once for each of the 3 gpio voltage codes in the register
                    parsed_aux = (handle->spiRxBuf)[data_counter + CMD_LEN] + ((handle->spiRxBuf)[data_counter+CMD_LEN+1]<<8);
                    (handle->auxVolts)[current_ic][current_gpio +((gpio_reg-1)*GPIO_IN_REG)] = parsed_aux;
                    data_counter += 2;
                }
                //a.iii
                received_pec = ((handle->spiRxBuf)[data_counter + CMD_LEN]<<8) + (handle->spiRxBuf)[data_counter+CMD_LEN+1];
                data_pec = pec15_calc(REG_BYTES, &(handle->spiRxBuf)[current_ic*BYTES_IN_REG + CMD_LEN]);

                if(received_pec != data_pec)
                {
                    pec_error = -1;
                }
                data_counter += 2;
            }
        }
    }
    else
    {
        //b.i
        LTC6804_rdaux_reg(handle, reg);

        for (current_ic = 0 ; current_ic < TOTAL_IC; current_ic++) 			  		// executes for every LTC6804 in the daisy chain
        {							   									          		// current_ic is used as an IC counter
        //b.ii
            for(current_gpio = 0; current_gpio<GPIO_IN_REG; current_gpio++)  	 	// This loop parses the read back data. Loops
            {						 											  		// once for each aux voltage in the register
                parsed_aux = ((handle->spiRxBuf)[data_counter+CMD_LEN] + ((handle->spiRxBuf)[data_counter+CMD_LEN+1]<<8));
                (handle->auxVolts)[current_ic][current_gpio +((reg-1)*GPIO_IN_REG)] = parsed_aux/10;
                data_counter += 2;
            }
            //b.iii
            received_pec = ((handle->spiRxBuf)[data_counter+CMD_LEN]<<8) + (handle->spiRxBuf)[data_counter+CMD_LEN+1];
            data_pec = pec15_calc(REG_BYTES, &((handle->spiRxBuf)[current_ic*BYTES_IN_REG + CMD_LEN]));
            if(received_pec != data_pec)
            {
              pec_error = -1;
            }

            data_counter += 2;
        }
    }
    return (pec_error);
}

int8_t read_cell_vol(LTC6804_handle *handle, uint16_t *cellvol)
{
    int8_t ret,i,j;
    //delay_ms(5);
    wakeup_idle();
    LTC6804_adcv(handle);
		delay_ms(3);
		wakeup_sleep();
    wakeup_idle();
    ret = LTC6804_rdcv(handle,0);
    if (0 == ret)
    {
        for (i = 0; i < TOTAL_IC; i++)
        {
            for (j = 0; j < 12; j++)
            {
                //mv
//                cellvol[i*12+j] = handle->cellVolts[i][j]/10;
								cellvol[j] = handle->cellVolts[i][j]/10;
            }
        }
       
    }
   return ret;    
}
int8_t read_aux_vol(LTC6804_handle *handle,uint16_t *gpiovol)
{
    int8_t ret,i,j;
    //delay_ms(10);
   // wakeup_sleep();
    wakeup_idle();
    LTC6804_adax(handle);
		delay_ms(3);
		wakeup_sleep();
    wakeup_idle();
    
    ret = LTC6804_rdaux(handle,0);
    if (0 == ret)
    {
         for (i = 0; i < TOTAL_IC; ++i)
        {
            for (j = 0; j < 6; ++j)
            {
                //mv
                gpiovol[i*6+j] = handle->auxVolts[i][j]/10;
            }
        }

    }

    return ret;

}

void write_config(LTC6804_handle *handle)
{
    wakeup_sleep();
    wakeup_idle();
    LTC6804_wrcfg(handle);
}
