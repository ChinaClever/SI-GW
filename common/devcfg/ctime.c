/*
 * ctime.c
 *
 *  Created on: 2019年6月6日
 *      Author: luozhiyong
 */
#include "ctime.h"

// 如果大于则返回1，如果小于返回0，如果等于返回2。
int dt_cmpDt(const char* time1,const char* time2)
{
	int year1,month1,day1,hour1,min1,sec1;
	int year2,month2,day2,hour2,min2,sec2;
	sscanf(time1,"%d-%d-%d %d:%d:%d",&year1,&month1,&day1,&hour1,&min1,&sec1);
	sscanf(time2,"%d-%d-%d %d:%d:%d",&year2,&month2,&day2,&hour2,&min2,&sec2);
	int tm1 = year1*10000+month1*100+day1;
	int tm2 = year2*10000+month2*100+day2;
	if(tm1!=tm2) return (tm1>tm2)?1:0;//如果相等，大返回1，小返回0
	tm1 = hour1*3600+min1*60+sec1;
	tm2 = hour2*3600+min2*60+sec2;//将时分秒转换为秒数
	if(tm1!=tm2) return (tm1>tm2)?1:0;//如果相等，大返回1，小返回0
	return 2;//到这里必然是相等了
}

int dt_cmpCurrentDt(const char* time1)
{
	time_t t;
	time (&t);//获取Unix时间戳。
	struct tm *lt = localtime (&t);//转为时间结构。
	if(strlen(time1)<12) return -1;

	char time2[42] = {0};
	sprintf (time2, "%d-%d-%d %d:%d:%d\n",lt->tm_year+1900, lt->tm_mon, lt->tm_mday, lt->tm_hour, lt->tm_min, lt->tm_sec);//输出结果
	return dt_cmpDt(time1, time2);
}

// 如果大于则返回1，如果小于返回0，如果等于返回2。
int dt_cmpTime(const char* time1,const char* time2)
{
	int hour1,min1,sec1;
	int hour2,min2,sec2;
	sscanf(time1,"%d:%d:%d",&hour1,&min1,&sec1);
	sscanf(time2,"%d:%d:%d",&hour2,&min2,&sec2);
	int tm1 = hour1*3600+min1*60+sec1;
	int tm2 = hour2*3600+min2*60+sec2;//将时分秒转换为秒数
	if(tm1!=tm2) return (tm1>tm2)?1:0;//如果相等，大返回1，小返回0
	return 2;//到这里必然是相等了
}

int dt_cmpCurrentTime(const char* time1)
{
	time_t t;
	time (&t);//获取Unix时间戳。
	struct tm *lt = localtime (&t);//转为时间结构。
	if(strlen(time1)<6) return -1;

	char time2[32] = {0};
	sprintf (time2, "%d:%d:%d\n",lt->tm_hour, lt->tm_min, lt->tm_sec);//输出结果
	return dt_cmpTime(time1, time2);
}


//将时间戳转成字符串 strfmt
void dt_datetimeToStr(time_t t, char *pcTime)
{
	struct tm *lt;
	lt = localtime(&t);
//	strftime(pcTime,128,"%F %T",lt);
	sprintf (pcTime, "%d-%02d-%02dT%02d:%02d:%02d\n",lt->tm_year+1900, lt->tm_mon, lt->tm_mday, lt->tm_hour, lt->tm_min, lt->tm_sec);//输出结果
}

// 将字符串转成时间戳
long dt_strToDatetime(const char *str_time)
{
	struct tm stm;
//	strptime(str_time, "%Y-%m-%d %H:%M:%S",&stm);
	int year, month, day, hour, minute,second;
	sscanf(str_time,"%d-%02d-%02dT%02d:%02d:%02d", &year, &month, &day, &hour, &minute, &second);
	stm.tm_year  = year-1900;
	stm.tm_mon   = month;
	stm.tm_mday  = day;
	stm.tm_hour  = hour;
	stm.tm_min   = minute;
	stm.tm_sec   = second;
	stm.tm_isdst = 0;

	long t = mktime(&stm);
	return t;
}

// 时间戳比较
int dt_cmpTimestamp(time_t t)
{
	time_t tm;
	time (&tm);//获取Unix时间戳

	return t-tm;
}

// 获取当天已过的秒数
int dt_secStartOfDay()
{
	time_t t;
	time (&t);//获取Unix时间戳。
	struct tm *lt = localtime (&t);//转为时间结构。

	int sec = lt->tm_hour*3600 + lt->tm_min*60 + lt->tm_sec;
	return sec;
}

// 时间转换成秒数
int dt_timeToSec(const char* time)
{
	int hour,min,sec;
	sscanf(time,"%02d:%02d:%02d",&hour,&min,&sec);
	int tm = hour*3600+min*60+sec;
	return tm;
}

// 秒数转换成时间
void dt_secToTime(int t, char *pcTime)
{
	int hour,min,sec;

	sec = (int)(t % 60); t /= 60;//取秒时间
	min =(int)(t % 60); t /= 60;//取分钟时间
	hour=(int)(t % 24);
	sprintf (pcTime, "%02d:%02d:%02d\n",hour, min, sec);//输出结果
}

// 秒数比较
int dt_cmpSec(int sec)
{
	int ret = dt_secStartOfDay();
	ret = sec - ret;

	return ret;
}
