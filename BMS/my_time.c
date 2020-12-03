//my_time.c
#include "my_time.h"
#include "includes.h"
u32 get_current_time(void)
{
    return HAL_GetTick()*TIME_PERIOD;
}
//计算时间间隔
//time1,当前时间，
//time2,上一次时间
//计算方法描述:正常time1>time2,当time1<time2时说明time1发生了溢出
u32 get_time_interval(u32 time1,u32 time2)
{
    if(time1 >= time2)
    {
        return time1 - time2;
    }
    else
    {
        return MAX_TIME_TICKS - (time2 - time1);
    }
}
//闰年计算
//返回值:1 闰年；0 平年
u8 is_leap_year(u16 year)
{
    if(year%4 == 0)
    {
        if(year%100 == 0)
        {
           if(year%400 == 0)
           {
               return 1;
           }
           else
           {
               return 0;
           }           
        }
        else
        {
            return 1;
        }
    }
    else
    {
        return 0;
    }  
}
//计算月份天数
//返回天数
u8 get_month_days(u16 year,u8 month)
{
    u8 days = 0;
    u8 ret = 0;
    
    if((year == 0) || (year > 3000))
    {
        return 0;
    }   
    switch(month)
    {
      case 1:
          days = 31;
          break;
      case 2:
          ret=is_leap_year(year);
          if(ret==1)
          {
              days = 29; 
          }
          else
          {
              days = 28;
          }
          break;
      case 3:
          days = 31;
          break;
      case 4:
          days = 30;
          break;
      case 5:
          days = 31;
          break;
      case 6:
          days = 30;
          break;
      case 7:
          days = 31;
          break;
      case 8:
          days = 31;
          break;
      case 9:
          days = 30;
          break;
      case 10:
          days = 31;
          break;
      case 11:
          days = 30;
          break;
      case 12:
          days = 31;
          break;
      default:
          break;         
    }   
    return days;  
}
//获取年份天数
u16 get_year_days(u16 year)
{
    u8 ret = 0;
    
    ret = is_leap_year(year);
    if(ret == 1)
    {
        return 366;
    }
    else
    {
        return 365; 
    }
}
/*
***************************************************************
//计算日期之间天数
//时间参数:
date1,
date2:
说明：date1比date2接近目前时间
***************************************************************
*/
u16 get_days(DATE_TIME date1,DATE_TIME date2)
{
    u16 days = 0;
    u16 temp_days = 0;
    u16 i = 0;
    //检查参数数据
    if((date1.month < 1) || (date1.month >12) )
    {
        return 0;
    }
    if((date1.day < 1) || (date1.day > 31) )
    {
        return 0;
    }
    if(date2.year==0) 
    {
        return 0;
    }
    if(date1.year < date2.year)
    {
        return MAX_BATTERY_DAYS;
    }
    else
    {
        if(date1.year == date2.year)
        {
            if(date1.month < date2.month)
            {
                return MAX_BATTERY_DAYS;
            }
            else
            {
                if(date1.month == date2.month)
                {
                    if(date1.day < date2.day)
                    {
                        return MAX_BATTERY_DAYS;
                    }
                    else
                    {
                        //计算date1与date2之间天数
                        days = (date1.day - date2.day);
                    }
                }
                else
                {
                    days = (u16)(get_month_days((u16)(2000+date2.year),date2.month) - date2.day + date1.day);
                    if((date1.month - date2.month) > 1)
                    {
                        for(i = (date2.month+1);i <= (date1.month-1);i++)
                        {
                            days += (u16)get_month_days((u16)(2000+date2.year),(u8)i);
                        }
                    }
                }
            }
        }
        else
        {
            temp_days = date2.day;
            for(i = 1;i <=(date2.month-1);i++)
            {
                temp_days += get_month_days((u16)(2000+date2.year),(u8)i);
            }
            days = (get_year_days((u16)(2000+date2.year)) - temp_days);
            for(i = (date2.year+1);i < (date1.year-1);i++)
            {
                days += get_year_days((u16)(2000+i));
            }
            for(i = 1;i <= (date1.month-1);i++)
            {
                days += get_month_days((u16)(2000+date1.year),(u8)i);
            }
            days += date1.day;          
        }
    } 
    return days;   
}
