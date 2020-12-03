#define WARMING_VARIABLE  //���ϼ���þֲ�����
#include "includes.h"
//��������ü���������
uint8_t vol_read_count=0;
uint8_t temp_false_num=0;    //�����´�����
uint8_t temp_read_count=0;
uint8_t master_com_count=0;

/*****************************************************/
// Warming_Handle
// �Ӱ�ɼ�������Ϻ���
// rsp_data�ṹ��
/****************************************************/
void Warming_Handle(struct RSP_BIT_DATA *rsp_data)
{
    unsigned int i=0;
    //��ѹ�ɼ�����
         if((rsp_data->pack_alarms[0]&ALARM_VOLTAGE_AD_BIT)==0) 
         {          
               if(rfalse_flag==1) 
               {
                    vol_read_count++; 
               } 
               else 
               {
                   vol_read_count=0; 
               }
               if(vol_read_count>=10) 
               {
                   vol_read_count=0;
                   rsp_data->pack_alarms[0] |= ALARM_VOLTAGE_AD_BIT; 
               } 
         } 
         else 
         {
               if(rfalse_flag==0) 
               {
                  rsp_data->pack_alarms[0] &= (~(ALARM_VOLTAGE_AD_BIT)); 
               } 
         } 
         //�¶Ȳɼ���·����
         temp_false_num=0;
         for(i=0;i<TEMP_NUM;i++) 
         {
            if(rsp_data->cellTemp[i]==INVALID_TEMP)
            temp_false_num++;  
         }
         if((rsp_data->pack_alarms[0]&ALARM_TEMP_AD_BIT)==0) 
         {          
               if(temp_false_num>=3) 
               {
                    temp_read_count++; 
               } 
               else 
               {
                   temp_read_count=0; 
               }
               if(temp_read_count>=25) 
               {
                   temp_read_count=0;
                   rsp_data->pack_alarms[0] |= ALARM_TEMP_AD_BIT; 
               } 
         } 
         else 
         {
               if(temp_false_num<=2) 
               {
                    temp_read_count++; 
               } 
               else 
               {
                   temp_read_count=0; 
               }
               if(temp_read_count>=10) 
               {
                  temp_read_count=0;
                  rsp_data->pack_alarms[0] &= (~(ALARM_TEMP_AD_BIT)); 
               } 
         }
         //����ͨѶ����
          if((rsp_data->pack_alarms[0]&ALARM_MASTER_COM_BIT)==0) 
         {          
               if(master_flag==0) 
               {
                    master_com_count++; 
               } 
               else 
               {
                   master_com_count=0; 
               }
               if(master_com_count>=25) 
               {
                   master_com_count=0;
                   rsp_data->pack_alarms[0] |= ALARM_MASTER_COM_BIT; 
               } 
         } 
         else 
         {
               if(master_flag==1) 
               {
                  rsp_data->pack_alarms[0] &= (~(ALARM_MASTER_COM_BIT)); 
               } 
         } 
         master_flag=0;
}

