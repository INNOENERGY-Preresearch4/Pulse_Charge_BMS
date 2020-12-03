
//time.h
//时间处理

#ifndef _TIME_H_
#define _TIME_H_

#include "sys.h"
#include "my_structs.h"
#define MAX_TIME_TICKS (120000000) //时间计数最大值
#define TIME_PERIOD (1) //ms


u32 get_current_time(void);
u32 get_time_interval(u32 time1,u32 time2); //计算时间间隔


//剩余日历寿命计算相关代码
u8 is_leap_year(u16 year);                  //闰年计算
u8 get_month_days(u16 year,u8 month);  //计算月份天数
u16 get_year_days(u16 year);                //获取年份天数
u16 get_days(DATE_TIME date1,DATE_TIME date2);   //计算日期之间天数

#endif //#if

