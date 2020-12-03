/*
***************************************************************************
SM28C040-EEPROM


***************************************************************************
*/
#include "includes.h"
//#include "my_structs.h"
//#include "config.h"
//////////////////////////////////
//³õÊ¼ÅäÖÃ
void memory_init(void)
{
   
    //EEPROM1ÅäÖÃ
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_6,GPIO_PIN_SET);	//WP1ÖÃ1	
	HAL_GPIO_WritePin(GPIOG,GPIO_PIN_12,GPIO_PIN_SET);	//HOLD1ÖÃ1	
	HAL_GPIO_WritePin(GPIOG,GPIO_PIN_10,GPIO_PIN_SET);	//CS1ÖÃ1	
    memory_write_status(0x02,0);
}
u8 spi_write_u8(u8 data)
{
    u8 ret;
    ret=SPI_ReadWriteByte(1,data); 
	return ret;
}

u8 spi_read_u8(u8 *data)
{
    u8 ret;
    ret=SPI_ReadWriteByte(1,*data); 
    return ret;
}
/*
**********************************************************
´æ´¢Æ÷Æ¬Ñ¡
**********************************************************
*/
void memory_Chip_Selection(u8 store_num,unsigned char val) 
{
    delay_us(1);
    switch(store_num)  //Ñ¡Ôñ´æ´¢Æ÷
    {
        case 0:
                 if(val)
		         HAL_GPIO_WritePin(GPIOG,GPIO_PIN_10,GPIO_PIN_SET);	//CS1ÖÃ1	
				 else
				 HAL_GPIO_WritePin(GPIOG,GPIO_PIN_10,GPIO_PIN_RESET);	//CS1ÖÃ0
                 break;
        default:
               break;             
    }
    delay_us(1);
} 
 
/*
**********************************************************
//Ð´Ê¹ÄÜ
//²ÎÊý£ºstore_num Æ¬Ñ¡£¬£¨Öµ0-3£©      
**********************************************************
*/
void SPI0_WREN(u8 store_num)
{
    unsigned char i;
    i=0;
	  while((Read_Status_Register(store_num)&0x01)==1)//¶ÁÈ¡×´Ì¬¼Ä´æÆ÷,ÅÐ¶ÏÊÇ·ñÎªÃ¦ 
	  {
	     i++;
	     if(i>100) 
	     {
	        break;
	     }
	  }      
    memory_Chip_Selection(store_num,0);    
    spi_write_u8(WREN);
    memory_Chip_Selection(store_num,1); 
} 

/*
**********************************************************
//¶Á×´Ì¬¼Ä´æÆ÷Öµ
//²ÎÊý£ºstore_num Æ¬Ñ¡£¬£¨Öµ0-3£©      
**********************************************************
*/
unsigned char Read_Status_Register(u8 store_num)
{ 
   unsigned char ChipStatus; 
   memory_Chip_Selection(store_num,0);
   spi_write_u8(RDSR);
   spi_read_u8(&ChipStatus);
   memory_Chip_Selection(store_num,1); 
   return ChipStatus;   
} 
/*
**********************************************************
Ð´×´Ì¬¼Ä´æÆ÷
**********************************************************
*/
void memory_write_status(u8 data,u8 store_num) 
{
    unsigned char i=0;
	  while((Read_Status_Register(store_num)&0x01)==1)//¶ÁÈ¡×´Ì¬¼Ä´æÆ÷,ÅÐ¶ÏÊÇ·ñÎªÃ¦ 
	  {
	     i++;
	     if(i>100) 
	     {
	        break;
	     }
	  } 
	  SPI0_WREN(store_num); 
	  i=0;
	  while((Read_Status_Register(store_num)&0x01)==1)//¶ÁÈ¡×´Ì¬¼Ä´æÆ÷,ÅÐ¶ÏÊÇ·ñÎªÃ¦ 
	  {
	     i++;
	     if(i>100) 
	     {
	        break;
	     }
	  } 
    memory_Chip_Selection(store_num,0);
    spi_write_u8(WRSR); 
    spi_write_u8(data);  //·¢ËÍ×´Ì¬Î»Êý¾Ý
    memory_Chip_Selection(store_num,1);     
}
          
/*
//////////////////////////////////////////////////////////
//´æ´¢Æ÷¶Á×Ö½ÚÊý¾Ý
//////////////////////////////////////////////////////////
*/
u8 memory_read_byte(u32 address,u8 store_num) 
{
    u8 data;
    u8 pdata;
    u8 i=0;
    u8 write_buffer=0;  
    while((Read_Status_Register(store_num)&0x01)==1)//¶ÁÈ¡×´Ì¬¼Ä´æÆ÷,ÅÐ¶ÏÊÇ·ñÎªÃ¦ 
	  {
	     i++;
	     if(i>100) 
	     {
	        break;
	     }
	  } 
	  SPI0_WREN(store_num); 
	  i=0;
	  while((Read_Status_Register(store_num)&0x01)==1)//¶ÁÈ¡×´Ì¬¼Ä´æÆ÷,ÅÐ¶ÏÊÇ·ñÎªÃ¦ 
	  {
	     i++;
	     if(i>100) 
	     {
	        break;
	     }
	  } 
	  memory_Chip_Selection(store_num,0);
    spi_write_u8(READ); //¶Á´æ´¢Æ÷Êý¾Ý
    for(i=3;i>0;i--) //·¢ËÍµØÖ·
    {
        write_buffer=(u8)(address>>((i-1)*8));
        spi_write_u8(write_buffer);
    }
    spi_read_u8(&pdata);  //¶ÁÈ¡Êý¾Ý
    memory_Chip_Selection(store_num,1);
    data=pdata; 
    return data;    
}
/*
//////////////////////////////////////////////////////////
//´æ´¢Æ÷¶Á4×Ö½ÚÊý¾Ý
//////////////////////////////////////////////////////////
*/
uint32_t memory_read_4byte(u32 address,u8 store_num) 
{
    uint8_t data[4];
    uint32_t re_vaule=0;
    data[0]=memory_read_byte(address,store_num);
    data[1]=memory_read_byte(address+1,store_num);
    data[2]=memory_read_byte(address+2,store_num);
    data[3]=memory_read_byte(address+3,store_num);
    re_vaule=data[0];
    re_vaule+=((uint32_t)data[1])<<8;
    re_vaule+=((uint32_t)data[2])<<16;
    re_vaule+=((uint32_t)data[3])<<24;
    return re_vaule;
}
/*
**********************************************************
´æ´¢Æ÷Ð´×Ö½ÚÊý¾Ý
**********************************************************
*/
void memory_write_byte(u32 address,u8 data,u8 store_num)  
{
    u8 write_buffer=0;
    u8 i=0;
    while((Read_Status_Register(store_num)&0x01)==1)//¶ÁÈ¡×´Ì¬¼Ä´æÆ÷,ÅÐ¶ÏÊÇ·ñÎªÃ¦ 
	  {
	     i++;
	     if(i>100) 
	     {
	        break;
	     }
	  } 
	  SPI0_WREN(store_num); 
	  i=0;
	  while((Read_Status_Register(store_num)&0x01)==1)//¶ÁÈ¡×´Ì¬¼Ä´æÆ÷,ÅÐ¶ÏÊÇ·ñÎªÃ¦ 
	  {
	     i++;
	     if(i>100) 
	     {
	        break;
	     }
	  } 
	  memory_Chip_Selection(store_num,0);
	  spi_write_u8(WRITE);
    for(i=3;i>0;i--) //·¢ËÍµØÖ·
    {
        write_buffer=(u8)(address>>((i-1)*8));
        spi_write_u8(write_buffer);
    }
    spi_write_u8(data);  //·¢ËÍÊý¾Ý
    memory_Chip_Selection(store_num,1);        
}
/*
**********************************************************
´æ´¢Æ÷Ð´4×Ö½ÚÊý¾Ý
**********************************************************
*/
void memory_write_4byte(u32 address,uint32_t data,u8 store_num) 
{
    uint8_t sdata[4];
    sdata[0]=data&0XFF;
    sdata[1]=(data&0XFF00)>>8;
    sdata[2]=(data&0XFF0000)>>16;
    sdata[3]=(data&0XFF000000)>>24;
    memory_write_byte(address,sdata[0],store_num);
 //   delay100(1);
    memory_write_byte(address+1,sdata[1],store_num);
//    dely100(1);
    memory_write_byte(address+2,sdata[2],store_num);
//    dely100(1);
    memory_write_byte(address+3,sdata[3],store_num);
}
/*
*****************************************************************
Ò³Ð´²Ù×÷
×¢Òâ£º×î¶àÒ»´ÎÒ³Ð´256¸ö×Ö½Ú
*****************************************************************
*/
void memory_write_page(u32 address,u8 *pdata,u32 DataLenth,u8 store_num)   
{
    uint16_t i = 0;
    u8 write_buffer=0;
    u32 size=0;   
    size=DataLenth;
    if((pdata!=NULL)&&(size<=256))
    {        
        while((Read_Status_Register(store_num)&0x01)==1)//¶ÁÈ¡×´Ì¬¼Ä´æÆ÷,ÅÐ¶ÏÊÇ·ñÎªÃ¦ 
	      {
	         i++;
	         if(i>100) 
	         {
	            break;
	         }
	      } 
	      SPI0_WREN(store_num); 
	      i=0;
	      while((Read_Status_Register(store_num)&0x01)==1)//¶ÁÈ¡×´Ì¬¼Ä´æÆ÷,ÅÐ¶ÏÊÇ·ñÎªÃ¦ 
	      {
	         i++;
	         if(i>100) 
	         {
	            break;
	         }
	      } 
	      memory_Chip_Selection(store_num,0);
	      spi_write_u8(WRITE);
        for(i=3;i>0;i--) //·¢ËÍµØÖ·
        {
           write_buffer=(u8)(address>>((i-1)*8));
           spi_write_u8(write_buffer);
        }  
        for(i = 0;i < size;i++)//·¢ËÍÊý¾Ý 
        {
           spi_write_u8(*(pdata+i));
        } 
        memory_Chip_Selection(store_num,1);
   }
}

































