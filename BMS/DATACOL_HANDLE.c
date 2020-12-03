#define DATACOL_VARIABLE   //声明main函数用局部变量宏定义
#include "includes.h"
/*****************************************************/
// Data_Collect_Handle
// 电压温度数据采集程序
// rsp_data结构体
/****************************************************/
void Data_Collect_Handle(struct RSP_BIT_DATA *rsp_data)//电压温度数据采集程序
{
          if(tempc_cont<2) //20-30ms 
          {
               //读取6804数据
               read_cellvol(rsp_data->cellVoltage,&(rsp_data->pack_vol));
               if(rfalse_flag==1) 
               {
                   read_cellvol(rsp_data->cellVoltage,&(rsp_data->pack_vol));
                   if(rfalse_flag==1) 
                   {   
                       delay_ms(500);
                       LTC6804_Init();
                   }
               }
                //单体电压最值 
             rsp_data->CEL_min_value = CellMinValue(rsp_data->cellVoltage,(uint16_t)CELLBAT_NUM,&(rsp_data->CEL_min_num));
             rsp_data->CEL_max_value = CellMaxValue(rsp_data->cellVoltage,(uint16_t)CELLBAT_NUM,&(rsp_data->CEL_max_num));
               tempc_cont++;
           }  else     //<10ms
           {
               //温度读取
               rsp_data->cellTemp[0]=get_AD_temp(0);
               rsp_data->cellTemp[1]=get_AD_temp(1);
               rsp_data->cellTemp[2]=get_AD_temp(2);
               rsp_data->cellTemp[3]=get_AD_temp(3);
               rsp_data->cellTemp[4]=get_AD_temp(4);
               rsp_data->cellTemp[5]=get_AD_temp(5);
               rsp_data->cellTemp[6]=get_AD_temp(6);
               rsp_data->cellTemp[7]=get_AD_temp(7);
               //温度最值
               rsp_data->TEMP_min_value = TempMinValue(rsp_data->cellTemp,(uint8_t)TEMP_NUM,&(rsp_data->TEMP_min_num));
               rsp_data->TEMP_max_value = TempMaxValue(rsp_data->cellTemp,(uint8_t)TEMP_NUM,&(rsp_data->TEMP_max_num));
               tempc_cont=0;
           } 
}
//CAN????:16???
void CAN_SendCCU_Data(struct RSP_BIT_DATA rspdata) 
{
    static uint8_t send_cont=0;
    CanTxMsgTypeDef msgTx;
    msgTx.StdId = 0x00;        //标准标识符
	//msgRx->ExtId=;        //扩展标识符(29位)
	msgTx.IDE = CAN_ID_EXT;    //使用扩展帧
	msgTx.RTR = CAN_RTR_DATA;  //数据帧
	msgTx.DLC = 8;  //数据长度           
      if(send_flag==0) 
      {
         msgTx.ExtId=LMUBMCUID+0X00000; 
         msgTx.Data[0]=(rsp_bit_data.CEL_max_value&0x00ff); 
         msgTx.Data[1]=rsp_bit_data.CEL_max_value>>8;
         msgTx.Data[2]=rsp_bit_data.CEL_max_num;
         msgTx.Data[3]=(rsp_bit_data.CEL_min_value&0x00ff); 
         msgTx.Data[4]=rsp_bit_data.CEL_min_value>>8;
         msgTx.Data[5]=rsp_bit_data.CEL_min_num;
         msgTx.Data[6]=(rsp_bit_data.pack_vol&0x00ff);
         msgTx.Data[7]=rsp_bit_data.pack_vol>>8;
         send_flag=1; 
      }else if(send_flag==1) 
      {
         msgTx.ExtId=LMUBMCUID+0X10000; 
         msgTx.Data[0]=(uint8_t)rsp_bit_data.TEMP_max_value; 
         msgTx.Data[1]=rsp_bit_data.TEMP_max_num;
         msgTx.Data[2]=(uint8_t)rsp_bit_data.TEMP_min_value; 
         msgTx.Data[3]=rsp_bit_data.TEMP_min_num;
         msgTx.Data[4]=rsp_bit_data.balance_num;
         msgTx.Data[5]=rsp_bit_data.pack_alarms[0];
         msgTx.Data[6]=rsp_bit_data.PRO_num&0xff;
         msgTx.Data[7]=(rsp_bit_data.PRO_num&0xf00)>>8;
		 msgTx.Data[7]=send_cont&0x0f;///////
         send_flag=2; 
		 send_cont++;
		 if(send_cont>0x0f)
		 {
			 send_cont=0;
		 }
      }
      else if(send_flag==2) 
      {
         msgTx.ExtId=LMUBMCUID+0X20000; 
		 msgTx.Data[0]=((uint16_t)rsp_bit_data.current)&0xff; 
         msgTx.Data[1]=((uint16_t)rsp_bit_data.current)>>8;
         msgTx.Data[2]=rsp_bit_data.soc_value&0xff;
         msgTx.Data[3]=rsp_bit_data.soc_value>>8;
         msgTx.Data[4]=rsp_bit_data.pack_alarms[1];
         msgTx.Data[5]=rsp_bit_data.pack_alarms[2];
         msgTx.Data[6]=rsp_bit_data.rly_status;
         msgTx.Data[7]=rsp_bit_data.IO_status;
         send_flag=0;  
      }else 
      {   
         send_flag=0;
      }
	  CAN_Send_Msg(1,msgTx);
}

