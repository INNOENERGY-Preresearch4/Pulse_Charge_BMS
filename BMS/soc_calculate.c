#include "includes.h"

u32 soc0_ah = 0;        //������ʼ����,����.��
u32 soc0_bzah = 0;      //������׼��ʼ����,����.��
u32 soc_ah_remain = 0;  //�����ʣ������,����.��
u32 soc_cal = 0;  //�����ʵ����������
u32 soc_cal_read = 0;  //�����ʵ��������ȡ
u8  soc_cal_flag=0;    //�����ʵ��������־
s16 soc_pre = 0;        //��һ��SOCǧ�ֱ�
u32  time1_write = 0;          //д��ʣ������ľ�ʱ��
u32  time2_write = 0;          //д��ʣ���������ʱ��
u8 power_off=0;   //�µ��ʶ
u8 SOC_calflag=0;     //ʵ�����������־
s32 energy_costadd = 0;
u32 writesoc_remian_address=ADDRESS_SOC_REMAIN;
//��ȡ�����ʣ������,��λ����.��
u32 read_soc_remain(u32 address)
{//1000*36000*1000;
    u8 byte1 = 0;
    u8 byte2 = 0;
    u8 byte3 = 0;
    u8 byte4 = 0; 
    u32 soc_remain = 0;    
    byte1 = memory_read_byte(address,0);
    byte2 = memory_read_byte((address+1),0);
    byte3 = memory_read_byte((address+2),0);
    byte4 = memory_read_byte((address+3),0); 
    soc_remain |= byte1;
    soc_remain |= (((u32)byte2)<<8);
    soc_remain |= (((u32)byte3)<<16);
    soc_remain |= (((u32)byte4)<<24);    
    return soc_remain;  
}
//***************************************************************
//����ʣ������
//soc_remain��λ����.��
//***************************************************************
void write_soc_remain(u32 address,u32 soc_remain)
{
    u8 byte1 = 0;           
    u8 byte2 = 0;
    u8 byte3 = 0;
    u8 byte4 = 0;
 
    byte1 = soc_remain & 0x000000ff;
    byte2 = (soc_remain & (0x0000ff00))>>8;
    byte3 = (soc_remain & (0x00ff0000))>>16;
    byte4 = (soc_remain & (0xff000000))>>24;    
    memory_write_byte(address,byte1,0);
    delay_us(5); 
    memory_write_byte((address+1),byte2,0); 
    delay_us(5); 
    memory_write_byte((address+2),byte3,0);
    delay_us(5);
    memory_write_byte((address+3),byte4,0);
    delay_us(5); 
}
//ѭ������ʣ�����
u16 get_cycles_remain(void)
{
    u16 a = 0;
    u16 b = 0;
    
    a = memory_read_byte(PACK_CYCLES_REMAIN_ADDR,0);
    b = memory_read_byte((PACK_CYCLES_REMAIN_ADDR+1),0);    
    b = b<<8;
    b += a;   
    return b;
}
//��ȡʣ��ʹ������
u16 get_days_remain(void)
{
    u16 a = 0;
    u16 b = 0;
    
    a = memory_read_byte(PACK_DAYS_REMAIN_ADDR,0);
    delay_us(1);
    b = memory_read_byte((PACK_DAYS_REMAIN_ADDR+1),0);    
    b = b<<8;
    b += a;    
    return b;    
}
//��ȡʣ��ѭ��������־
u8 get_cycles_remain_flag(void)
{
    u8 a = 0;
    
    a = memory_read_byte(PACK_CYCLES_REMAIN_FLAG_ADDR,0);

    return a;
}
//ʣ��ʹ�ô���
void memory_save_cycle_remain(u16 data,u8 cyc_flag)   
{   
    memory_write_byte(PACK_CYCLES_REMAIN_ADDR,((u8)(data & 0x00ff)),0);
    delay_us(1);
    memory_write_byte(PACK_CYCLES_REMAIN_ADDR+1,((u8)((data & 0xff00)>>8)),0);
    delay_us(1);
    memory_write_byte(PACK_CYCLES_REMAIN_FLAG_ADDR,cyc_flag,0);  //��־λ���
      
}
//����ʣ������
void memory_save_days_remain(u16 data)    
{
    memory_write_byte(PACK_DAYS_REMAIN_ADDR,((u8)(data & 0x00ff)),0);
    delay_us(1);
    memory_write_byte(PACK_DAYS_REMAIN_ADDR+1,((u8)((data & 0xff00)>>8)),0);
    delay_us(1);
}
//*********************************************************
//˵��:SOC�����ʼ��ʧ�ܺ󣬳�ʼ��ֵ����
//������battery_capacity,��ʱ����ֵ��Χ0-1193����λ��ʱ
//********************************************************* 
void soc_history_init(u32 battery_capacity) 
{
   
    u8 soc_remain_mark=0;
    u8 socjz_flag=0;
    u8 cont=0;
    u32 address_midle=0;
    u16 count_mark=0;
    u32 SOC_max=0,SOC_min=0,SOC_middle=0;
    //��ʼ���Ƚϱ���
    SOC_max=0;
    SOC_min=(battery_capacity*3600);
    //��ȡʣ������
    address_midle= (ADDRESS_SOC_REMAIN-1);
    while(1) 
    {
       if(count_mark>=SOC_REMAIN_MAX) 
       {
           count_mark=0;
           break;
       }       
       if(address_midle>=(ADDRESS_SOC_REMAIN+(SOC_REMAIN_MAX*5)-1)) 
       {
            address_midle= (ADDRESS_SOC_REMAIN+4);
       } 
       else 
       {
            address_midle= (address_midle+5);
       }       
       soc_remain_mark = memory_read_byte(address_midle,0);
       delay_us(1);
       if(soc_remain_mark==0XAA)  //Ϊ��Ч��ʷ����
       {
           socjz_flag=1;
           writesoc_remian_address=(address_midle-4);   
           soc_ah_remain = read_soc_remain(writesoc_remian_address);//��ȡʣ����������λ����.��
           delay_us(10);
           if(SOC_max<soc_ah_remain) 
           {
              SOC_max=soc_ah_remain;
           }
           if(SOC_min>soc_ah_remain) 
           {
              SOC_min=soc_ah_remain;
           }
           SOC_middle+=soc_ah_remain/SOC_REMAIN_MAX;
           cont++;
       }
       count_mark++;         
    }
    if(socjz_flag==1)  //������ʷֵ   ��д�붨ֵ��if(0)
    {
       if(SOC_max==SOC_min) 
       {
            soc_ah_remain=SOC_max;  
       }else if(cont==1)
       {
            soc_ah_remain=SOC_max;
       }else if(cont==2) 
       {
            soc_ah_remain=(SOC_max/2)+(SOC_min/2);   
       }else if(cont>2) 
       {
           soc_ah_remain=((SOC_middle-(SOC_max/SOC_REMAIN_MAX)-(SOC_min/SOC_REMAIN_MAX))/(cont-2))*SOC_REMAIN_MAX;
       }else 
       {
           soc_ah_remain=(SOC_max/2)+(SOC_min/2);
       }
    
    }else //������ʷֵ
    {
        soc_ah_remain = battery_capacity*3240;//90%
    }
    
    ///////////SOC��ʷֵ��������///////////////////////////
     write_soc_remain(ADDRESS_SOC_REMAIN,soc_ah_remain);
     delay_us(10);
     memory_write_byte((ADDRESS_SOC_REMAIN+4),0x55,0);
     writesoc_remian_address=(ADDRESS_SOC_REMAIN+5);
     //////////////////////////////////////////////////////////////////// 
     soc_pre = (s16)(((float)soc_ah_remain)/soc0_ah*1000);
     if(soc_pre>1000)
     {
         soc_pre=1000;
     }    
    
}
//*********************************************************
//˵��:SOC�����ʼ��
//������battery_capacity,��ʱ����ֵ��Χ0-1193����λ��ʱ
//********************************************************* 
void soc_calculate_init(u32 battery_capacity)
{   
    u8 soc_remain_mark=0;
    u32 address_midle=0;
    u8 count_mark=1;
    //��ȡʣ������
    address_midle= (ADDRESS_SOC_REMAIN+4);
    soc_remain_mark=memory_read_byte((address_midle),0);
    delay_us(200);
    while(soc_remain_mark!=0x55) 
    {
       if(count_mark>=(SOC_REMAIN_MAX*2)) 
       {
           count_mark=0;
           break;
       }   	   
       if(address_midle>=(ADDRESS_SOC_REMAIN+(SOC_REMAIN_MAX*5)-1)) 
       {
            address_midle= (ADDRESS_SOC_REMAIN+4);
       } 
       else 
       {
            address_midle= (address_midle+5);
       }       
       soc_remain_mark=memory_read_byte((address_midle),0);
       delay_us(200);
       count_mark++;         
    } 
    if(count_mark==0) 
    {
        soc0_ah=0;
        soc_ah_remain = 0;
    } 
    else 
    {
        writesoc_remian_address=(address_midle-4);   
        soc_ah_remain = read_soc_remain(writesoc_remian_address);//��ȡʣ����������λ����.��      
    }
    //�Ƿ�洢�оɵ������������
    soc_cal_flag = memory_read_byte((ADDRESS_SOCSJ_CAL+4),0);
    if(soc_cal_flag==0xaa) 
    {
      
        soc_cal_read=read_soc_remain(ADDRESS_SOCSJ_CAL);
        if((soc_cal_read>(SOC0_CAPACITY*3600))||(soc_cal_read<(SOCBZ_CAPACITY*3600)))
         {
          
             soc0_ah = (battery_capacity*3600); //��λ����.��
         }else 
         {
             soc0_ah= soc_cal_read;
         }
    }else 
    {
        soc0_ah = (battery_capacity*3600); //��λ����.��
    } 
 //   soc0_ah = (battery_capacity*3600); //��λ����.��
    if(count_mark==0)
    {//������ 
        soc_history_init(battery_capacity);  
      /*  soc_ah_remain = soc0_ah/1000*950;
        soc_pre = 950;
        write_soc_remain(ADDRESS_SOC_REMAIN,soc_ah_remain);
        memory_write_byte((ADDRESS_SOC_REMAIN+4),0x55,0);
        writesoc_remian_address=(ADDRESS_SOC_REMAIN+5); */
    }
    else
    {//������
        if(soc_ah_remain > soc0_ah)  //���ʣ���������ڳ�ʼ��������˵������Ѿ����ͣ�ʣ������Ϊ��ʼ����
        {
           soc_ah_remain = soc0_ah;
           write_soc_remain(writesoc_remian_address,soc_ah_remain); //����ʣ������
           memory_write_byte((writesoc_remian_address+4),0x55,0);
        }
        soc_pre = (s16)(((float)soc_ah_remain)/soc0_ah*1000);
        if(soc_pre>1000)
        {
            soc_pre=1000;
        }      
    }        
}
//*********************************************************
//˵��:ʵ�ʵ����������
//����
//********************************************************* 
void cal_socrl_ah(u8 flag,s32 current,u32 time) 
{
    u32 energy_cost = 0;
    u32 test_soc_ah_remain = 0;  //����,����.��
   if(flag==0)
   {
       return;
   }else if(flag==1) 
   {
        if(current<0) 
        {
            current=(-current);
            energy_cost=(u32)((time*current)/1000);                    //��λ����.��
            if(soc_cal==0) 
            {
               soc_cal = 0;
            } else if(soc_cal <energy_cost) 
            {
                 soc_cal = 0;
            } 
            else 
            {
                 soc_cal =(soc_cal-energy_cost);
            }    
        } 
        else
        {
            energy_cost=(u32)((time*current)/1000); 

            soc_cal = (soc_cal+energy_cost);
        }
           
   }else if(flag==2) 
   {
        if(soc_cal<(SOCBZ_CAPACITY*1800))//�������� 
        {
           return;
        }
        if(soc_cal>(SOC0_CAPACITY*3600)) 
        {
            soc_cal=(SOC0_CAPACITY*3600);
        }
        if(soc_cal<(SOCBZ_CAPACITY*3600)) 
        {
            soc_cal=(SOCBZ_CAPACITY*3600);
        }
        write_soc_remain(ADDRESS_SOCSJ_CAL,soc_cal);      //����ʣ�����
        delay_us(200);
        memory_write_byte((ADDRESS_SOCSJ_CAL+4),0xaa,0);
        delay_us(200);
        test_soc_ah_remain=read_soc_remain(ADDRESS_SOCSJ_CAL);
        if(test_soc_ah_remain!=soc_cal) 
        {
             write_soc_remain(ADDRESS_SOCSJ_CAL,soc_cal);      //����ʣ�����
             delay_us(200);
             memory_write_byte((ADDRESS_SOCSJ_CAL+4),0xaa,0);
             delay_us(200);        
        }
   } else 
   {
   }
}
//*********************************************************
//˵��:SOC����
//����
//********************************************************* 
s16 calculate_soc_ah(u32 soc0_ah,s32 current,u32 time)
{
    s16 soc_value_temp = 0;   //SOCǧ�ֱ�
    u32  time_write_pass = 0;          //д��ʣ���������ʱ��
    u32 test_soc_ah_remain = 0;  //����,����.��
    u8     test_data=0;
    u32 energy_cost = 0;
    if(power_off==1)
    {
      if((writesoc_remian_address-ADDRESS_SOC_REMAIN)>=(SOC_REMAIN_MAX*5)) 
        {
             writesoc_remian_address=ADDRESS_SOC_REMAIN;
        }
        write_soc_remain(writesoc_remian_address,soc_ah_remain);      //����ʣ�����
        delay_us(200);
        test_soc_ah_remain=read_soc_remain(writesoc_remian_address);
        if(test_soc_ah_remain!=soc_ah_remain) 
        {
             write_soc_remain(writesoc_remian_address,soc_ah_remain);      //����ʣ�����
             delay_us(200);        
        }
        memory_write_byte((writesoc_remian_address+4),0x55,0);
        delay_us(200);
        test_data=memory_read_byte((writesoc_remian_address+4),0);
        if(test_data!=0x55) 
        {
             memory_write_byte((writesoc_remian_address+4),0x55,0);
             delay_us(200);        
        }
        if(writesoc_remian_address==ADDRESS_SOC_REMAIN) 
        {
             memory_write_byte((writesoc_remian_address+((SOC_REMAIN_MAX*5)-1)),0xAA,0);
             delay_us(200);
             test_data=0;
             test_data=memory_read_byte((writesoc_remian_address+((SOC_REMAIN_MAX*5)-1)),0);
             if(test_data!=0xAA) 
             {
                 memory_write_byte((writesoc_remian_address+((SOC_REMAIN_MAX*5)-1)),0xAA,0);
                 delay_us(200);                 
             }
        } 
        else 
        {
             memory_write_byte((writesoc_remian_address-1),0xAA,0);
             delay_us(200); 
             test_data=0;
             test_data=memory_read_byte((writesoc_remian_address-1),0);
             if(test_data!=0xAA) 
             {
                 memory_write_byte((writesoc_remian_address-1),0xAA,0);
                 delay_us(200);                 
             }             
        }
        writesoc_remian_address=(writesoc_remian_address+5);
        soc_value_temp = (s16)(((float)soc_ah_remain)/soc0_ah*1000);    //ǧ�ֱ���ֵ  
        power_off=2; 
        return soc_value_temp;   
    }
    //�������
    if((soc0_ah == 0))
    {
        return 0;      
    }
    //����SOC
    energy_costadd+=(s32)(current*time);
    ////////////////////////////////////////////////////////////////////////////////////  
    if(energy_costadd<(-SOC0_CAPACITY)) 
    {
        energy_cost=-energy_costadd;
        energy_cost=(u32)(energy_cost/1000);                    //��λ����.��
        if(soc_ah_remain==0) 
        {
           soc_ah_remain = 0;
        } else if(soc_ah_remain <energy_cost) 
        {
             soc_ah_remain = 0;
        } 
        else 
        {
             soc_ah_remain =(soc_ah_remain-energy_cost);
        }
        energy_costadd=0;    
    } 
    else if(energy_costadd>SOC0_CAPACITY)
    {
        energy_cost=energy_costadd;
        energy_cost=(u32)(energy_cost/1000);   
        soc_ah_remain =(soc_ah_remain+energy_cost);
        if(soc_ah_remain>soc0_ah) 
        {
             soc_ah_remain = soc0_ah;
        } 
        energy_costadd=0;
    }else 
    {
    }
    ////////////////////////////////////////////////////////////////////////////////
    if(soc_ah_remain > soc0_ah)
    {
        soc_ah_remain = soc0_ah;
    }
    soc_value_temp = (s16)(((float)soc_ah_remain)/soc0_ah*1000);    //ǧ�ֱ���ֵ    
    if(soc_value_temp > 1000)
    {
        soc_value_temp = 1000;
    }
    if(soc_value_temp<0) //�Ա���
    {
        soc_value_temp=0;
    }
    soc_pre = soc_value_temp;             //ǧ�ֱ���ֵ
    time2_write=get_current_time();
    time_write_pass = get_time_interval(time2_write,time1_write);
    if((time_write_pass>SOC_REMAIN_WRITETIME))
    {
        if((writesoc_remian_address-ADDRESS_SOC_REMAIN)>=(SOC_REMAIN_MAX*5)) 
        {
             writesoc_remian_address=ADDRESS_SOC_REMAIN;
        }
        write_soc_remain(writesoc_remian_address,soc_ah_remain);      //����ʣ�����
        delay_us(200);
        test_soc_ah_remain=read_soc_remain(writesoc_remian_address);
        if(test_soc_ah_remain!=soc_ah_remain) 
        {
             write_soc_remain(writesoc_remian_address,soc_ah_remain);      //����ʣ�����
             delay_us(200);        
        }
        memory_write_byte((writesoc_remian_address+4),0x55,0);
        delay_us(200);
        test_data=memory_read_byte((writesoc_remian_address+4),0);
        if(test_data!=0x55) 
        {
             memory_write_byte((writesoc_remian_address+4),0x55,0);
             delay_us(200);        
        }
        if(writesoc_remian_address==ADDRESS_SOC_REMAIN) 
        {
             memory_write_byte((writesoc_remian_address+((SOC_REMAIN_MAX*5)-1)),0xAA,0);
             delay_us(200);
             test_data=0;
             test_data=memory_read_byte((writesoc_remian_address+((SOC_REMAIN_MAX*5)-1)),0);
             if(test_data!=0xAA) 
             {
                 memory_write_byte((writesoc_remian_address+((SOC_REMAIN_MAX*5)-1)),0xAA,0);
                 delay_us(200);                 
             }
        } 
        else 
        {
             memory_write_byte((writesoc_remian_address-1),0xAA,0);
             delay_us(200); 
             test_data=0;
             test_data=memory_read_byte((writesoc_remian_address-1),0);
             if(test_data!=0xAA) 
             {
                 memory_write_byte((writesoc_remian_address-1),0xAA,0);
                 delay_us(200);                 
             }             
        }
        writesoc_remian_address=(writesoc_remian_address+5);       
        time1_write=time2_write; 
    }    
    return soc_value_temp; 
}
//*********************************************************
//˵��:��·SOC����
//����
//********************************************************* 
s16 OCV_Int(u16 cellmin,s8 temp) 
{
     s16 soc_value_temp = 0;   //SOCǧ�ֱ�
         if(cellmin>=3400) 
         {
            if(soc_pre<800) 
            {
                soc_value_temp=800;
                soc_ah_remain=(soc0_ah/5)*4;
            }
         }else if(cellmin>=3300)
         {
            if(soc_pre<500) 
            {
                soc_value_temp=500;
                soc_ah_remain=(soc0_ah/2);
            }
         
         }else if(cellmin>=3200) 
         {
         
               if(soc_pre<200) 
              {
                  soc_value_temp=200;
                  soc_ah_remain=soc0_ah/5;
              }else if(soc_pre>900) 
              {
                  soc_value_temp=900;
                  soc_ah_remain=(soc0_ah/10)*9;
              }
              
         }else if(cellmin>=3100)
         {
               if(soc_pre<50) 
              {
                  soc_value_temp=50;
                  soc_ah_remain=soc0_ah/20;
              }else if(soc_pre>600) 
              {
                  soc_value_temp=600;
                  soc_ah_remain=(soc0_ah/10)*6;
              }
         }
         else if(cellmin>=3000)
         {
               if(soc_pre>350) 
              {
                  soc_value_temp=350;
                  soc_ah_remain=(soc0_ah/20)*7;
              }
         }else  if(cellmin>=2900)
         {
             
               if(soc_pre>200) 
              {
                  soc_value_temp=200;
                  soc_ah_remain=(soc0_ah/5);
              }
         }else {
              if(soc_pre>100) 
              {
                  soc_value_temp=100;
                  soc_ah_remain=soc0_ah/10;
              }
         }
    
    if(soc_value_temp > 1000)
    {
        soc_value_temp = 1000;
    }
    if(soc_value_temp<0) //�Ա���
    {
        soc_value_temp=0;
    }
      return soc_value_temp;
}
void SOC_Handle(s32 cur,u32 passtime)
{
	if(rsp_bit_data.CEL_min_value<(CELL_LOWVOLTAGE+50)) 
    {
		  SOC_calflag=1;
    }
	rsp_bit_data.soc_value = calculate_soc_ah(soc0_ah,cur,passtime);
	if(SOC_calflag==1) 
	{
		 cal_socrl_ah(SOC_calflag,cur,passtime);
	}
}
//*********************************************************
//�������ƣSSOC_Handle
//����˵��:ʵʱ����SOC���ۼƼ���ʵ�ʵ������
//����
//********************************************************* 
void SOC_real_write(s32 cur,u32 passtime)
{
	 if(SOC_calflag==1) 
	 {
		 SOC_calflag=2;
		 cal_socrl_ah(SOC_calflag,cur,passtime);
		 SOC_calflag=0;
	 }
}







