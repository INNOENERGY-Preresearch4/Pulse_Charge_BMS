
//time.h
//ʱ�䴦��

#ifndef _TIME_H_
#define _TIME_H_

#include "sys.h"
#include "my_structs.h"
#define MAX_TIME_TICKS (120000000) //ʱ��������ֵ
#define TIME_PERIOD (1) //ms


u32 get_current_time(void);
u32 get_time_interval(u32 time1,u32 time2); //����ʱ����


//ʣ����������������ش���
u8 is_leap_year(u16 year);                  //�������
u8 get_month_days(u16 year,u8 month);  //�����·�����
u16 get_year_days(u16 year);                //��ȡ�������
u16 get_days(DATE_TIME date1,DATE_TIME date2);   //��������֮������

#endif //#if

